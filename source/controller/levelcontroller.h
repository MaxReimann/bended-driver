#pragma once
// OSG
#include <osg/ref_ptr>
// troen
#include "abstractcontroller.h"
#include "../forwarddeclarations.h"

namespace troen
{
	class LevelController : public AbstractController
	{
	public:
		LevelController();
		const COLLISIONTYPE getCollisionType() { return LEVELTYPE; };
	};
}