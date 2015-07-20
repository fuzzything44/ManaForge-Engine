#include "OpenGLRendererPCH.h"

#include "OpenGLMaterialInstance.h"
#include "OpenGLTexture.h"
#include "OpenGLMaterialSource.h"

#include <Logging.h>
#include <Helper.h>
#include <Texture.h>

#include <OpenGLTexture.h>


OpenGLMaterialInstance::OpenGLMaterialInstance(std::shared_ptr<MaterialSource> source)
{
	if (source)
		init(source);
}

OpenGLMaterialInstance::~OpenGLMaterialInstance()
{

}

void OpenGLMaterialInstance::setTexture(uint32 ID, std::shared_ptr<Texture> texture)
{
	check(texture);
	textures[ID] = std::static_pointer_cast<OpenGLTexture>(texture);

}

void OpenGLMaterialInstance::init(std::shared_ptr<MaterialSource> source)
{
	program = std::static_pointer_cast<OpenGLMaterialSource>(source);


}

std::shared_ptr<MaterialSource> OpenGLMaterialInstance::getSource()
{
	return program;
}
std::shared_ptr<const MaterialSource> OpenGLMaterialInstance::getSource() const
{
	return program;
}


void OpenGLMaterialInstance::use()
{
	glUseProgram(**program);

	for (uint32 i = 0; i < maxTextures && textures[i]; i++)
	{
		check(*textures[i])

		glUniform1i(program->startTexUniform + i, i);

		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, (*textures[i])->getID());
	}
	
}
