#pragma once
// STD
#include <memory>
// OSG
#include <osgViewer/View>
// troen
#include "forwarddeclarations.h"
#include "troengame.h"

#include "sampleosgviewer.h"

#include "controller/bikecontroller.h"
#include "controller/bikecontroller.h"
#include "controller/routecontroller.h"
#include "controller/hudcontroller.h"
#include "navigation/navigationwindow.h"
#include "navigation/routeparser.h"

namespace troen
{

	/*! Each player (no matter if human player, AI or network enemy) is represented by a player instance. This class holds player specific information like an ID, name, points, health, color.*/
	class Player
	{
	public:
		Player(
			TroenGame* game,
			const std::shared_ptr<GameConfig>& config, 
			const int id);
		void createHUDController(const std::vector<std::shared_ptr<Player>>& players);
		virtual ~Player();

		//
		// getters
		//
		const int id() 			{ return m_id; };
		osg::Vec3 color()			{ return m_color; };
		const std::string name()	{ return m_name; };
		int deathCount() 			{ return m_deathCount; };
		int killCount() 			{ return m_killCount; };
		const float health()		{ return m_health; };
		const float points()		{ return m_points; };
		TroenGame* getTroenGame()	{ return m_troenGame; }
		//
		// controllers
		//
		std::shared_ptr<BikeController>	bikeController()
			{ return m_bikeController; };
		std::shared_ptr<RouteController> routeController()
			{ return m_routeController; };
		std::shared_ptr<HUDController>	hudController()
			{ return m_HUDController; };
		//
		// osg related
		//
		bool hasGameView()									{ return m_hasGameView; }
		osg::ref_ptr<SampleOSGViewer> viewer()				{ return m_viewer; };
		osg::ref_ptr<osgViewer::View> gameView()			{ return m_gameView; };
		osg::ref_ptr<osg::Group> playerNode()				{ return m_playerNode; };
		std::shared_ptr<NavigationWindow> navigationWindow(){ return m_navigationWindow; };
		osg::ref_ptr<SampleOSGViewer> navigationViewer();

		//
		// setters
		//
		void setHealth(float health)	{ m_health = health; };
		void setPoints(float points)	{ m_points = points; };
		void setKillCount(int killCount) { m_killCount = killCount; };

		//
		// logic methods
		//
		void increaseDeathCount() { m_deathCount++; };
		void decreaseDeathCount() { m_deathCount--; };
		void increaseKillCount() { m_killCount++; };
		void decreaseKillCount() { m_killCount--; };
		float increaseHealth(const float diff);
		float increasePoints(const float diff);
		void update(int g_gameTime);
		void setBendingUniform(troen::windowType window, bool value);
		bool isDead();
		std::shared_ptr<std::vector<osg::Camera*>> cameras(){ return m_cameras; };
		void setCameraSpecificUniforms();
		void setOnNextTrack();
		bool hasNextTrack();
	private:
		TroenGame*						m_troenGame;
		//
		// player attributes
		//
		int			m_id;
		osg::Vec3	m_color;
		std::string m_name;
		float		m_health;
		float		m_points;
		int			m_killCount;
		int			m_deathCount;
		//
		// controllers
		//
		std::shared_ptr<BikeController>	m_bikeController;
		std::shared_ptr<RouteController> m_routeController;
		std::shared_ptr<HUDController>	m_HUDController;

		//
		// osg elements
		//
		bool							m_hasGameView;
		osg::ref_ptr<SampleOSGViewer>	m_viewer;
		osg::ref_ptr<osgViewer::View>	m_gameView;
		osg::ref_ptr<osg::Group>		m_playerNode;
		std::shared_ptr<NavigationWindow> m_navigationWindow;
		std::vector<Route>				m_routes;

		std::shared_ptr<std::vector<osg::Camera*>> m_cameras;
		
		typedef std::vector<osg::ref_ptr<osg::Uniform>> uniformVec;
		uniformVec		m_bendingActivatedUs;
		uniformVec		m_isReflectingUs;
		uniformVec		m_playerPositionUs;
		int m_currentRoute;
	};
}