#pragma once

// STD includes we need everywhere
#include <memory>
#include <iostream>

// This file contains forward declarations of classes

// Qt forward declarations

// OSG forward declarations
namespace osgViewer
{
	class Viewer;
	class CompositeViewer;
	class View;
	class StatsHandler;
}

namespace osgQt
{
	class GraphicsWindowQt;
}

namespace osg
{
	class Camera;
	class Node;
	class Group;
	class Geode;
	class GraphicsContext;
	class ShapeDrawable;
	class Vec3f;
	class Image;
}

namespace osgText
{
	class Text;
}


namespace osgGA
{
	class CameraManipulator;
	class NodeTrackerManipulator;
}

// Bullet
class btDiscreteDynamicsWorld;
class btSequentialImpulseConstraintSolver;
class btDefaultCollisionConfiguration;
class btCollisionDispatcher;
class btBroadphaseInterface;
class btRigidBody;
class btCollisionObject;
class btCollisionShape;
class btPersistentManifold;
class btVector3;
class btTransform;

namespace scriptzeug {
	class ScriptEnvironment;
}

// own classes
namespace troen
{
	class TroenGame;
	class TroenGameBuilder;
	class SampleOSGViewer;
	class GameEventHandler;
	class GameLogic;
	class GameConfig;
	class Player;

	class BikeModel;
	class LevelModel;
	class CityModel;
	class RouteModel;
	class AbstractModel;
	class PhysicsWorld;

	class BikeController;
	class LevelController;
	class RouteController;
	class AbstractController;
	class HUDController;


	class BikeView;
	class LevelView;
	class CityView;
	class RouteView;
	class AbstractView;
	class HUDView;
	class SkyDome;
	class PostProcessing;
	class NodeFollowCameraManipulator;
	class ResourcePool;
	class SplineDeformationRendering;
	class NavigationWindow;


	namespace util
	{
		class ChronoTimer;
		class GLDebugDrawer;
	}
	namespace input
	{
		class Keyboard;
		class Gamepad;
		class BikeInputState;
		class PollingDevice;
	}

	namespace sound
	{
		class AudioManager;
	}
	

	namespace tracking
	{
		struct CurrentBikeState;
		class TrackBike;
		class Tracker;
	}
}