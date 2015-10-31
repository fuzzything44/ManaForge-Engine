#pragma once

#include <Engine.h>

#include "OpenGLModelData.h"
#include "OpenGLMaterialInstance.h"

class COpenGLModel 
{
public:
	friend class OpenGLRenderer;
	
private:
	uint8 renderOrder;
	
	std::shared_ptr<OpenGLModelData> modelData;
	std::shared_ptr<OpenGLMaterialInstance> material;
};