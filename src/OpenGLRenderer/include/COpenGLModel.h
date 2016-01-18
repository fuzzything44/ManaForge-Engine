#pragma once

#include <Engine.h>

#include "OpenGLModelData.h"
#include "OpenGLMaterialInstance.h"

class COpenGLModel 
{
public:
	
	// NOTE: uses the OpenGLModelData QOpenGLFunctions
	COpenGLModel(const OpenGLModelData& data, const std::shared_ptr<OpenGLMaterialInstance>& mat = {})
		: modelData{ data }
		, material(mat)
	{}
	COpenGLModel(const COpenGLModel&) = default;
	COpenGLModel(COpenGLModel&&) = default;
	
	COpenGLModel& operator=(const COpenGLModel&) = default;
	COpenGLModel& operator=(COpenGLModel&&) = default;
	
	uint8 renderOrder;
	
	OpenGLModelData modelData;
	std::shared_ptr<OpenGLMaterialInstance> material;
	
};