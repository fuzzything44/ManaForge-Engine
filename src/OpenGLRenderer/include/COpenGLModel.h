#pragma once

#include <Engine.h>

#include "OpenGLModelData.h"
#include "OpenGLMaterialInstance.h"

class COpenGLModel 
{
public:
	COpenGLModel(const OpenGLModelData& data = {}, const std::shared_ptr<OpenGLMaterialInstance>& mat = {})
		: modelData{ data }
		, material(mat)
	{}



	friend class OpenGLRenderer;
	
	uint8 renderOrder;
	
	OpenGLModelData modelData;
	std::shared_ptr<OpenGLMaterialInstance> material;
};