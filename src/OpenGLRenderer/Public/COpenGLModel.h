#pragma once

#include <Engine.h>

#include "OpenGLModelData.h"
//#include "OpenGLMaterialInstance.h"

class COpenGLModel 
{
public:
	COpenGLModel(const OpenGLModelData& data)
		: modelData{ data }
	{}

	friend class OpenGLRenderer;
	
private:
	uint8 renderOrder;
	
	OpenGLModelData modelData;
	//std::shared_ptr<OpenGLMaterialInstance> material;
};