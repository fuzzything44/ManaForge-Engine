#pragma once

#include <Material.h>

class OpenGLMaterial : public Material
{
public:
	~OpenGLMaterial() override;
	void addShaderProgramFromFile(std::string filename) override;
	void addShaderProgramFromSource(std::string shader) override;
	void setTexture(uint32 ID, std::string texture) override;

private:



};