#pragma once
// STD
#include <vector>
#include <set>
#include <iterator>
#include <algorithm>
// troen
#include "forwarddeclarations.h"
#include "qqueue.h"

// typedefs for collision events

namespace troen
{

	/*! The GameLogic class manages the events within the game such as collision/separation events, deaths of players etc.*/
	class GameLogic
	{

	public:
		typedef enum enum_GAMESTATE {
			GAME_START,
			GAME_RUNNING,
			GAME_OVER
		} GAMESTATE;

		GameLogic(TroenGame* game, const int timeLimit = 5);
		void attachPhysicsWorld(std::shared_ptr<PhysicsWorld>& physicsWorld);

		//
		// stepping
		//
		void step(const long double gameloopTime, const long double gameTime);
		GAMESTATE getGameState() { return m_gameState; };

		//
		// collision event handling
		//
		virtual void collisionEvent(btRigidBody* pBody0, btRigidBody * pBody1, btPersistentManifold* contactManifold);

		//
		// logic methods
		//
		void removeAllFences();
		void toggleFencePartsLimit();
		void hideFencesInRadarForPlayer(int id);
		void showFencesInRadarForPlayer(int id);

		void resetBike(BikeController *bikeController);
		void resetBikePositions();

		void restartLevel();
		

	private:
		//
		// collision event handling
		//
		void handleCollisionOfBikeAndNonmovingObject(
			BikeController* bike,
			AbstractController* object,
			const int objectType,
			btPersistentManifold* contactManifold);
		void handleCollisionOfTwoBikes(
			BikeController* bike1,
			BikeController* bike2,
			btPersistentManifold* contactManifold);
		// death handling
		void handlePlayerDeath(BikeController* bike);
		void handlePlayerDeathNonFence(BikeController* deadBike);
		void handlePlayerFall(BikeController* deadBike);
		void handleEndZoneCollision(BikeController* bike);
		// helper
		float impulseFromContactManifold(btPersistentManifold* contactManifold, BikeController* bike);
		void playCollisionSound(float impulse);
		Player* getPlayerWithID(int bikeID);

		void checkForFallenPlayers();

		//
		// communication links
		//
		TroenGame *m_troenGame;

		//
		// stepping variables & methods
		//
		GAMESTATE m_gameState;
		long double m_timeLimit;
		long double m_gameStartTime;
		void stepGameStart(
			const long double gameloopTime,
			const long double gameTime);
		void stepGameRunning(
			const long double gameloopTime,
			const long double gameTime);
		void stepGameOver(
			const long double gameloopTime,
			const long double gameTime);
		bool m_limitedFenceMode;
	};
}