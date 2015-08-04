#pragma once

#include "OpenGLRendererConfig.h"

#include <MaterialSource.h>

class OpenGLRenderer;

class OpenGLMaterialSource : public MaterialSource
{
  public:
	explicit OpenGLMaterialSource(OpenGLRenderer& renderer, const path_t& name = "");
	virtual ~OpenGLMaterialSource();

	virtual void init(const path_t& name) override;

	virtual path_t getName() const override;


	GLint operator*() const { return program; }

	int32 startTexUniform;
	int32 MVPUniformLocation;

  private:
	path_t name;
	OpenGLRenderer& renderer;
	GLint program;
};
