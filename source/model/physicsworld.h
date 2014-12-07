#pragma once
// STD
#include <vector>
#include <set>
#include <iterator>
#include <algorithm>
#include <array>
#include <mutex>
// troen
#include "../forwarddeclarations.h"

// typedefs for collision events
typedef std::vector<std::shared_ptr<troen::AbstractController>> ControllerVector;
typedef std::pair<const btRigidBody*, const btRigidBody*> CollisionPair;
typedef std::set<CollisionPair> CollisionPairSet;

namespace troen
{

	typedef void (*pt2Function)(void*, btPersistentManifold *m);

/*! The PhysicsWorld provides a level of abstraction over the btDynamicsWorld of Bullet. Primarily, it is responsible for stepping the world, adding/removing collision bodies and detecting collisions between them.*/
	class PhysicsWorld
	{
	public:
		PhysicsWorld(std::shared_ptr<GameLogic>& gameLogic, bool useDebugView = false);
		virtual ~PhysicsWorld();

		void initializeWorld();
		void stepSimulation(const long double currentTime);

		void addRigidBodies(const std::vector<std::shared_ptr<btRigidBody>>& bodies, const short group = 0, const short mask = 0);
		void addRigidBody(btRigidBody *body, const short group = 0, const short mask = 0);
		void removeRigidBodies(const std::vector<std::shared_ptr<btRigidBody>>& bodies);
		void removeRigidBody(btRigidBody* body);

		void removeRigidBodyFromCollisionPairs(btRigidBody* body);
		
		void addCollisionObject(btCollisionObject* obj);
		void removeCollisionObject(btCollisionObject* obj);

		void checkForCollisionEvents();
		std::array<std::array<int, 100>, 100>* discretizeWorld();

		// debugview
		util::GLDebugDrawer* m_debug;

		
		btDiscreteDynamicsWorld* getDiscreteWorld()
		{
			return m_world;
		};

		std::mutex* getMutex() { return &m_physicsMutex; };
		void registerCustomCallback(void *p2Object, pt2Function func);
	private:
		btDiscreteDynamicsWorld*			m_world;
		mybtSequentialImpulseConstraintSolver*m_solver;
		btDefaultCollisionConfiguration*	m_collisionConfiguration;
		btCollisionDispatcher*				m_dispatcher;
		btBroadphaseInterface*				m_broadphase;

		// collision events
		CollisionPairSet m_pairsLastUpdate;
		std::vector<btRigidBody*> m_removedRigidBodies;

		// steping variables
		long double m_lastSimulationTime;

		bool m_useDebugView;
		std::weak_ptr<GameLogic> m_gameLogic;

		std::array<std::array<int, 100>, 100> m_discretizedWorld;

		std::mutex m_physicsMutex;
		std::vector<std::pair<void*,pt2Function>> m_customCallbacks;
	};
}