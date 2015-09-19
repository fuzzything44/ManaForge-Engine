#pragma once

#include "OpenGLRendererConfig.h"

#include <MaterialSource.h>
#include <Cacher.h>

class OpenGLRenderer;

class OpenGLMaterialSource : public MaterialSource
{
	friend StrongCacher<path_t, OpenGLMaterialSource>;
public:
	explicit OpenGLMaterialSource(OpenGLRenderer& renderer, const path_t& name = "");

	OpenGLMaterialSource(const OpenGLMaterialSource& other) = delete;
	OpenGLMaterialSource(OpenGLMaterialSource&& other) = default;

	OpenGLMaterialSource& operator=(const OpenGLMaterialSource& other) = delete;
	OpenGLMaterialSource& operator=(OpenGLMaterialSource&& other);
	
private:
	virtual ~OpenGLMaterialSource();
	
public:
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
