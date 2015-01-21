#pragma once
// OSG
// troen
#include "troengame.h"

namespace troen
{

/*! The TroenGameBuilder is responsible for setting up the entire Troen-Game. This includes acquiring all necessary resources, instantiating the essential classes and wiring everything up.*/
	class TroenGameBuilder
	{

	public:
		TroenGameBuilder(TroenGame * game);

		bool build();
		bool destroy();

	private:
		bool buildInput();
		bool composeSceneGraph();
		bool buildGameLogic();
		bool buildPhysicsWorld();
		bool composeRadarScene();
		osg::ref_ptr<osg::Group> composePostprocessing();
		TroenGame * t;
	};
}