#pragma once
// OSG
#include <osgViewer/View>
#include <osgText/Text>

#include "../forwarddeclarations.h"
#include "../player.h"

// troen

#include "../sampleosgviewer.h"

namespace troen
{

	class NavigationWindow
	{
	public:
		NavigationWindow(std::shared_ptr<BikeController> bikeController, osg::ref_ptr<GameEventHandler> eventHandler, SampleOSGViewer *viewer, osg::Viewport *viewport, bool fullscreen);
		void addElements(osg::ref_ptr<osg::Group> group);

		osg::ref_ptr<osgViewer::View> mapView()		{ return m_view; };
		osg::ref_ptr<osg::Group> mapNode()			{ return m_rootNode; };
		osg::ref_ptr<SampleOSGViewer> navViewer()		{ return m_viewer; };
		osg::ref_ptr<osg::Camera>  createPreRenderCamera(int sizeX, int sizeY);
		osg::ref_ptr<osgViewer::View> debugView() { return m_debugView; }

		osg::ref_ptr<osg::Uniform> m_bendingActiveUniform;
	private:
		osg::ref_ptr<osgViewer::View>	m_view;
		osg::ref_ptr<SampleOSGViewer>	m_viewer;
		osg::ref_ptr<osg::Group>		m_sceneNode;
		osg::ref_ptr<osg::Group>		m_rootNode;
		osg::ref_ptr<osg::Camera>		m_preRenderCam;
		osg::ref_ptr<osg::Texture2D> m_fboTexture;
		osg::ref_ptr<osgViewer::View> m_debugView;
	};
}