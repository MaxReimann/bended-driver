#include "abstractcontroller.h"
// troen
#include "../view/levelview.h"
#include "../model/levelmodel.h"

using namespace troen;

AbstractController::AbstractController()
{
	
}

osg::ref_ptr<osg::Group> AbstractController::getViewNode()
{
	return m_view->getNode();
}



std::vector<std::shared_ptr<btRigidBody>> AbstractController::getRigidBodies()
{
	return m_model->getRigidBodies();
}