#pragma once

#include <Engine.h>

//#include "OpenGLModelData.h"
//#include "OpenGLMaterialInstance.h"

class COpenGLModel 
{
public:
	friend class OpenGLRenderer;
	
private:
	uint8 renderOrder;
	
	//OpenGLModelData modelData;
	//std::shared_ptr<OpenGLMaterialInstance> material;
};