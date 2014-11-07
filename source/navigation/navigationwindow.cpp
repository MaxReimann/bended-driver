#include "navigationwindow.h"
// osg

#include <osgViewer/View>
#include <osgViewer/config/SingleWindow>

// troen
#include "../constants.h"
#include "../view/nodefollowcameramanipulator.h"
#include "../controller/bikecontroller.h"
#include "../model/bikemodel.h"

using namespace troen;




class CameraCopyCallback : public osg::NodeCallback
{
public:
	osg::ref_ptr<osgViewer::View> m_navView;
	std::shared_ptr<BikeController> bikeController;

	CameraCopyCallback(std::shared_ptr<BikeController> controller, osg::ref_ptr<osgViewer::View> navigationView) : NodeCallback()
	{
		m_navView = navigationView;
		bikeController = controller;
	}

	void operator()(osg::Node *node, osg::NodeVisitor *nv)
	{
		if (nv->getVisitorType() == osg::NodeVisitor::CULL_VISITOR)
		{
			osg::Vec3f gameEye, c, gameUp;
			osg::Vec3f eye, center, up;

			eye = bikeController->getModel()->getPositionOSG() + osg::Vec3d(0.0, 0.0, 30.0);
			center = eye + btToOSGVec3(bikeController->getModel()->getDirection());
			up = osg::Vec3d(0.0, 0.0, 1.0);
			m_navView->getCamera()->setViewMatrixAsLookAt(eye, center, up);

		}
		//dont traverse ?
		this->traverse(node, nv);
	}
};



NavigationWindow::NavigationWindow(std::shared_ptr<BikeController> bikeController)
{
	m_rootNode = new osg::Group();
	m_view = new osgViewer::View();
	m_view->getCamera()->setCullMask(CAMERA_MASK_MAIN);
	m_view->getCamera()->getOrCreateStateSet()->addUniform(new osg::Uniform("isReflecting", false));
	m_view->setSceneData(m_rootNode);

#ifdef WIN32
	m_view->apply(new osgViewer::SingleWindow(800, 200, DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT));
#else
	m_view->setUpViewInWindow(100, 100, DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT);
#endif
	
	m_viewer = new SampleOSGViewer();
	m_viewer->addView(m_view);

	osg::ref_ptr<RealizeOperation> navOperation = new RealizeOperation;
	m_viewer->setRealizeOperation(navOperation);
	m_viewer->realize();

	m_rootNode->setCullCallback(new CameraCopyCallback(bikeController, m_view));
}

void NavigationWindow::addElements(osg::ref_ptr<osg::Group> group)
{
	m_rootNode->addChild(group);
}

