#include "OpenGLMaterialInstance.h"

#include <QOpenGLFunctions_3_3_Core>

void OpenGLMaterialInstance::use(const mat3& MVP)
{
	QOpenGLFunctions_3_3_Core funs;
	
	funs.glUseProgram(source.program);

	// apply textures
	for (auto&& ID = 0; ID != textures.size(); ++ID)
	{
		if (textures[ID].handle)
		{
			funs.glUniform1i(source.startTexUniform + ID, ID);
			funs.glActiveTexture(GL_TEXTURE0 + ID);
			funs.glBindTexture(GL_TEXTURE_2D, textures[ID].handle);
			
		}
	}

	// apply all other properties
	std::for_each(properties.begin(), properties.end(), [](auto&& elem) { elem(); });

	// set the MVP matrix
	funs.glUniformMatrix3fv(source.MVPUniformLocation, 1, GL_FALSE, &MVP[0][0]);

}