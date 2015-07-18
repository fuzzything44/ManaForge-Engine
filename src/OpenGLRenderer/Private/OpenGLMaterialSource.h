#pragma once

#include "OpenGLRendererConfig.h"

#include <MaterialSource.h>

class OpenGLMaterialSource : public MaterialSource
{
public:

	explicit OpenGLMaterialSource(const path_t& name = "");
	virtual ~OpenGLMaterialSource();

	virtual void init(const path_t& name) override;
	
	virtual std::string getName() override;


	GLint operator*() const
	{
		return program;
	}

private:
	std::string name;

	GLint program;
};

