#include "fenceview.h"
// OSG
#include <osg/ShapeDrawable>
#include <osg/Geode>
#include <osg/Vec4>
// troen
#include "shaders.h"
#include "../model/fencemodel.h"

using namespace troen;

FenceView::FenceView(std::shared_ptr<FenceModel> &model)
{
	m_model = model;
	m_node = new osg::Group();

	initializeFence();
	initializeShader();
}

void FenceView::initializeFence()
{
	m_fenceHeight = m_model->getFenceHeight();

	m_coordinates = new osg::Vec3Array;
	m_coordinates->setDataVariance(osg::Object::DYNAMIC);

	m_geometry = new osg::Geometry();
	m_geometry->setVertexArray(m_coordinates);

	// seems to be important so that we won't crash after 683 fence parts
	m_geometry->setUseDisplayList(false);
	
	// use the shared normal array.
	// polyGeom->setNormalArray(shared_normals.get(), osg::Array::BIND_OVERALL);

	m_drawArrays = new osg::DrawArrays(osg::PrimitiveSet::QUAD_STRIP, 0, 0);
	m_geometry->addPrimitiveSet(m_drawArrays);

	osg::Geode* geode = new osg::Geode();
	geode->addDrawable(m_geometry);
	m_node->addChild(geode);
}

void FenceView::initializeShader()
{
	osg::ref_ptr<osg::StateSet> NodeState = m_node->getOrCreateStateSet();

	// TODO (dw) set to actual player color
	osg::Uniform* fenceColorU = new osg::Uniform("fenceColor", osg::Vec3(0.0, 1.0, 1.0));
	NodeState->addUniform(fenceColorU);

	osg::Uniform* fenceHeightU = new osg::Uniform("fenceHeight", m_fenceHeight);
	NodeState->addUniform(fenceHeightU);

	NodeState->setMode(GL_BLEND, osg::StateAttribute::ON);
	NodeState->setAttributeAndModes(shaders::m_allShaderPrograms[shaders::FENCE], osg::StateAttribute::ON);
}

void FenceView::addFencePart(osg::Vec3 a, osg::Vec3 b)
{
	if (m_coordinates->size()==0)
	{
		m_coordinates->push_back(a);
		m_coordinates->push_back(osg::Vec3(a.x(), a.y(), a.z() + m_fenceHeight));
	}

	m_coordinates->push_back(b);
	m_coordinates->push_back(osg::Vec3(b.x(), b.y(), b.z() + m_fenceHeight));
	
	// TODO remove if no disadvantages seem necessary?
	// m_geometry->dirtyBound();
	m_drawArrays->setCount(m_coordinates->size());
	
}