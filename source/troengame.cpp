#include "troengame.h"
// OSG
#include <osg/LineWidth>

// todo bended:
// #include <osg/BoundingSphere>
// #include <osgViewer/ViewerEventHandlers>
// #include <osgDB/ReadFile>
#include <osgUtil/Optimizer>
// qt
#include <qcoreapplication>
// troen
#include "troengamebuilder.h"
#include "constants.h"
#include "globals.h"
#include "player.h"
#include "gamelogic.h"
#include "sampleosgviewer.h"

#include "controller/levelcontroller.h"
#include "controller/bikecontroller.h"
#include "controller/hudcontroller.h"

#include "model/physicsworld.h"

#include "BendedViews/src/SplineDeformationRendering.h"
#include "view/postprocessing.h"

#include "util/chronotimer.h"
#include "util/gldebugdrawer.h"
#include "util/countdowntimer.h"
#include "sound/audiomanager.h"

#include "tracking/trackbike.h"

#include <thread>
#include <mutex>



using namespace troen;
extern long double g_currentTime;

TroenGame::TroenGame(QThread* thread /*= nullptr*/) :
QObject(),
m_gameConfig(nullptr),
m_gameThread(thread)
{
	if (m_gameThread == nullptr) {
		m_gameThread = new QThread(this);
	}

	moveToThread(m_gameThread);
	m_gameThread->start(QThread::HighestPriority);
}




////////////////////////////////////////////////////////////////////////////////
//
// Game Loop
//
////////////////////////////////////////////////////////////////////////////////

void TroenGame::prepareAndStartGame(const GameConfig& gameConfig)
{
	m_gameConfig = std::make_shared<GameConfig>(gameConfig);
	startGameLoop();
}

void TroenGame::startGameLoop()
{
	// adaptive game loop from here:
	// http://entropyinteractive.com/2011/02/game-engine-design-the-game-loop/

	// INITIALIZATION
	m_builder = new TroenGameBuilder(this);
	m_builder->build();

	if (m_gameConfig->useDebugView)
		m_sceneNode->addChild(m_physicsWorld->m_debug->getSceneGraph());

	if (m_gameConfig->fullscreen)
		setupForFullScreen();

	m_gameloopTimer->start();
	m_gameTimer->start();
	m_gameTimer->pause();
	g_gameRunning = true;
	g_timeSinceLastUpdate = 0;
	long double lastUpdateTime = m_gameloopTimer->elapsed();


	// GAME LOOP VARIABLES
	long double nextTime = m_gameloopTimer->elapsed();
	const double minMillisecondsBetweenFrames = 16.7; // vSync to 60 fps
	const double maxMillisecondsBetweenFrames = 4 * minMillisecondsBetweenFrames + 1;
	int skippedFrames = 0;
	const int maxSkippedFrames = 4;

	bool nearPlaneAdapted = false;

	// GAME LOOP
	// - AI
	// - network
	// - checkForUserInput and updateModels
	// - physics + updateViews
	// - render;
	m_audioManager->PlayEngineSound();
	m_audioManager->SetMasterVolume(1.0);


	// terminates when first viewer is closed
	while (!m_players[0]->viewer()->done()&&g_gameRunning)
	{
		g_gameLoopTime = m_gameloopTimer->elapsed();
		g_gameTime = m_gameTimer->elapsed();
		

		QCoreApplication::processEvents();

		for (auto player : m_playersWithView)
		{
			player->viewer()->frame();
			player->navigationViewer()->frame();
		}

		// are we significantly behind? if yes, "resync", force rendering
		if ((g_gameLoopTime - nextTime) > maxMillisecondsBetweenFrames)
			nextTime = g_gameLoopTime;
		// is it time to render the next frame?
		if (m_gameConfig->testPerformance || g_gameLoopTime >= nextTime)
		{
			g_timeSinceLastUpdate = g_gameLoopTime - lastUpdateTime;
			lastUpdateTime = g_gameLoopTime;
			// assign the time for the next update
			nextTime += minMillisecondsBetweenFrames;

			// LOOP REALLY STARTS HERE:
			m_gameLogic->step(g_gameLoopTime, g_gameTime);
			if (!m_gameTimer->paused())
			{
				for (auto player : m_players)
				{
					player->update(g_gameTime);
				}
				m_physicsWorld->stepSimulation(g_gameTime);
			}

			m_bikeTracker->update(g_gameTime);
			m_countdownTimers->update();

			m_audioManager->Update(g_gameLoopTime / 1000);
			m_audioManager->setMotorSpeed(m_players[0]->bikeController()->getEngine());

			// Hack: normalize and use the speed to control the deformation
			float bikeSpeed = m_players[0]->bikeController()->speed();
			float maxSpeed = 400.f;

			handleBending(double(bikeSpeed / maxSpeed));

			// do we have extra time (to draw the frame) or did we skip too many frames already?
			if (g_gameLoopTime < nextTime || (skippedFrames > maxSkippedFrames))
			{
				for (auto player : m_playersWithView)
				{
					player->hudController()->update(
						g_gameLoopTime,
						g_gameTime,
						m_gameConfig->timeLimit,
						m_gameLogic->getGameState(),
						m_players);
				}


				// TODO: find a way to eleminate this workaround
				// doesn't work if it's executed earlier
				if (!nearPlaneAdapted)
				{
					for (auto player : m_playersWithView)
					{
						fixCulling(player->gameView());
						//fixCulling(player->navigationViewer()->getView(0));
					}
				}
				skippedFrames = 0;
			}
			else
				skippedFrames++;
		}
		else // WAIT
		{
			// calculate the time to sleep
			long double sleepTime = (nextTime - g_gameLoopTime);
			if (sleepTime > 0)	// sanity check, sleep until nextTime
			if (!m_gameConfig->testPerformance) m_gameThread->msleep(sleepTime);
		}
	}

	if (m_gameConfig->fullscreen)
		returnFromFullScreen();

	// SHUTDOWN
	m_builder->destroy();
	delete m_builder;
}

void TroenGame::fixCulling(osg::ref_ptr<osgViewer::View> view)
{
	double fovy, aspect, znear, zfar;
	view->getCamera()->getProjectionMatrixAsPerspective(fovy, aspect, znear, zfar);
	view->getCamera()->setComputeNearFarMode(osg::CullSettings::DO_NOT_COMPUTE_NEAR_FAR);
	znear = 1.0;
	view->getCamera()->setProjectionMatrixAsPerspective(fovy, aspect, znear, zfar);
}

void TroenGame::handleBending(double interpolationSkalar)
{
	if (!use_bendedViews)
		return;

	m_deformationRendering->setInterpolationSkalar(1.0);

	double currentBending = m_deformationRendering->getDeformationEnd();
	const double targetBending = m_deformationEnd;
	const double bendedStep = (BENDED_VIEWS_DEACTIVATED - BENDED_VIEWS_ACTIVATED) / 100;
	
	if (targetBending == BENDED_VIEWS_ACTIVATED)
		currentBending -= bendedStep;
	else
		currentBending += bendedStep;
	currentBending = clamp(BENDED_VIEWS_ACTIVATED, BENDED_VIEWS_DEACTIVATED, currentBending);

	m_deformationRendering->setDeformationStartEnd(0.05, currentBending);
	m_levelController->setBendingFactor(1.0 - currentBending / BENDED_VIEWS_DEACTIVATED);
	

}

////////////////////////////////////////////////////////////////////////////////
//
// Full Screen Handling
//
////////////////////////////////////////////////////////////////////////////////

void TroenGame::setupForFullScreen()
{
	osg::GraphicsContext::WindowingSystemInterface* wsi =
		osg::GraphicsContext::getWindowingSystemInterface();
	if (!wsi)
	{
		std::cout << "[TroenGame::setupForFullScreen] error ..." << std::endl;
		return;
	}
	wsi->getScreenResolution(osg::GraphicsContext::ScreenIdentifier(0), m_originalWidth, m_originalHeight);

	//wsi->setScreenResolution(osg::GraphicsContext::ScreenIdentifier(0), 1024, 768);
	this->resize(m_originalWidth, m_originalHeight);
}

void TroenGame::returnFromFullScreen()
{
	osg::GraphicsContext::WindowingSystemInterface* wsi =
		osg::GraphicsContext::getWindowingSystemInterface();
	if (!wsi)
	{
		std::cout << "[TroenGame::returnFromFullScreen] error ..." << std::endl;
		return;
	}
	wsi->setScreenResolution(osg::GraphicsContext::ScreenIdentifier(0), m_originalWidth, m_originalHeight);
}



void TroenGame::switchSoundVolumeEvent()
{
	m_audioManager->SetMasterVolume(1 - m_audioManager->GetMasterVolume());
}

void TroenGame::pauseEvent()
{
	if (m_gameLogic->getGameState() == GameLogic::GAMESTATE::GAME_START)
		return;
	else if (!m_gameTimer->paused())
		pauseSimulation();
	else
		unpauseSimulation();
}

void TroenGame::pauseSimulation()
{
	if (!m_gameTimer->paused()) m_gameTimer->pause();
}

void TroenGame::unpauseSimulation()
{
	if (m_gameTimer->paused()) m_gameTimer->start();
}

void TroenGame::resize(int width, int height, int windowType){
	if (m_postProcessing){
		m_postProcessing->setupTextures(width, height);
	}

	if (windowType == MAIN_WINDOW)
	{
		for (auto player : m_playersWithView)
		{
			player->hudController()->resize(width, height);
		}
	} 
}


void TroenGame::reloadLevel()
{
	m_levelController->reload();
}


void TroenGame::toggleHUDVisibility()
{
	m_players[0]->hudController()->toggleVisibility();
}

BikeModel* TroenGame::activeBikeModel()
{
	return m_players[0]->bikeController()->getModel().get();
}

std::shared_ptr<CityModel> TroenGame::cityModel()
{
	return m_levelController->getAsCityModel();
}