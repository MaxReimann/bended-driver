#pragma once




// OSG
#include <osg/ref_ptr>
#include <osg/Geode>
// troen
#include "../forwarddeclarations.h"
#include <memory>

#include "abstractview.h"


namespace troen
{
	namespace shaders
	{		
		#ifndef _SHADERS_H
		#define _SHADERS_H 
				extern std::vector<osg::ref_ptr<osg::Program> > m_allShaderPrograms;
		#endif

		bool loadShaderSource(osg::Shader* obj, const std::string& fileName);
		void reloadShader(osg::ref_ptr<osg::Program> & program,
			std::string fragmentFileName,
			std::string vertexFileName = "");

		void reloadShaders();


		enum Shadernames  { DEFAULT,
							GRID,
							SHADER_NAME_COUNT //always last, isnt a shader
						};


	}

}