#pragma once
// Qt
#include <QThread>
#include <QMetaType>
#include <QColor>
// OSG
#include <osg/ref_ptr>
#include <osg/ShapeDrawable>
#include <osgGA/GUIEventAdapter>
#include <osg/Group>
#include <osgViewer/ViewerEventHandlers>
// troen
//order is important, has to be imported before windows.h
#include "forwarddeclarations.h"
#include "constants.h"
#include "gameeventhandler.h"
#include "resourcepool.h"
#include "view/skydome.h"



#define MAX_BIKES 6
namespace troen
{

	/*! The TroenGame class contains the logic for the game loop and basic functionality such as pausing the game, resizing the game window or switching to fullscreen.*/
	class GameConfig
	{
	public:
		int numberOfPlayers;
		int timeLimit;
		int* playerInputTypes;
		QColor* playerColors;
		QString* playerNames;
		bool fullscreen;
		bool usePostProcessing;
		bool useDebugView;
		bool testPerformance;
		bool ownView[MAX_BIKES];
		std::string levelName;
		//user study
		int difficulty;
		int participantNumber;
		QString studySetup;
		bool exportCSV;
		// TODO: add Destructor to avoid memory leak
	};
}
Q_DECLARE_METATYPE(troen::GameConfig)

namespace troen
{
	class TroenGame : public QObject
	{
		Q_OBJECT

		friend class TroenGameBuilder;
		friend class GameLogic;

	public:
		TroenGame(QThread* thread = nullptr);
		//
		// getters
		//
		osg::ref_ptr<GameEventHandler> gameEventHandler()
		{
			return m_gameEventHandler;
		};
		osg::ref_ptr<osgViewer::StatsHandler> statsHandler()
		{
			return m_statsHandler;
		};
		std::shared_ptr<LevelController> levelController()
		{
			return m_levelController;
		};
		std::shared_ptr<CityModel> cityModel();
		std::shared_ptr<PhysicsWorld> physicsWorld()
		{
			return m_physicsWorld;
		}
		std::vector<std::shared_ptr<Player>> players()
			{ return m_players; };

		std::shared_ptr<tracking::TrackBike> bikeTracker() 
		{
			return m_bikeTracker;
		}
		//convenience
		BikeModel* activeBikeModel();

		osg::ref_ptr<SkyDome> skyDome()
			{ return m_skyDome; };
		ResourcePool* resourcePool(){ return &m_resourcePool; };

		//
		// Events
		//
		void switchSoundVolumeEvent();
		void pauseEvent();
		void pauseSimulation();
		void unpauseSimulation();
		void resize(int width, int height, int windowType=BOTH_WINDOWS);
		void reloadLevel();


		SplineDeformationRendering* getBendedViews() {
			return m_deformationRendering;
		}

		void enableBendedViews() { m_deformationEnd = BENDED_VIEWS_ACTIVATED; }
		void disableBendedViews() { m_deformationEnd = BENDED_VIEWS_DEACTIVATED; }
		
		double m_deformationEnd = BENDED_VIEWS_DEACTIVATED;

		void toggleHUDVisibility();


	public slots:
		void prepareAndStartGame(const GameConfig& config);

	private:
		TroenGameBuilder *m_builder;
		//
		// Game Loop
		//
		void startGameLoop();
		void fixCulling(osg::ref_ptr<osgViewer::View> view);
		void handleBending(double interpolationSkalar);

		//
		// fullscreen handling
		//
		void setupForFullScreen();
		void returnFromFullScreen();
		uint m_originalWidth;
		uint m_originalHeight;

		//
		// OSG Components
		//
		osg::ref_ptr<GameEventHandler>		m_gameEventHandler;
		osg::ref_ptr<osg::Group>			m_rootNode;
		osg::ref_ptr<SkyDome>               m_skyDome;
		osg::ref_ptr<osgViewer::StatsHandler> m_statsHandler;
		std::shared_ptr<PostProcessing>		m_postProcessing;
		osg::ref_ptr<osg::Group>			m_sceneNode;
		osg::ref_ptr<osg::Group>			m_sceneWithSkyboxNode;

		//
		// Game Components
		//
		std::shared_ptr<GameConfig>					m_gameConfig;
		std::shared_ptr<LevelController>			m_levelController;
		std::vector<std::shared_ptr<Player>>		m_players;
		std::vector<std::shared_ptr<Player>>		m_playersWithView;
		std::shared_ptr<util::ChronoTimer>			m_gameloopTimer;
		std::shared_ptr<util::ChronoTimer>			m_gameTimer;
		std::shared_ptr<PhysicsWorld>				m_physicsWorld;
		std::shared_ptr<GameLogic>					m_gameLogic;
		std::shared_ptr<sound::AudioManager>		m_audioManager;
		std::shared_ptr<tracking::TrackBike>		m_bikeTracker;

		ResourcePool m_resourcePool;

		// BendedViews
		SplineDeformationRendering* m_deformationRendering;

		// Startup Options
		QThread* m_gameThread;
	};
}