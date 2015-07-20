#include "OpenGLRendererPCH.h"

#include "OpenGLRendererConfig.h"

#include "OpenGLModel.h"
#include "OpenGLModelData.h"
#include "OpenGLRenderer.h"
#include "OpenGLMaterialSource.h"

#include <Transform.h>
#include <ModuleManager.h>
#include <Helper.h>
#include <CameraComponent.h>
#include <MeshComponent.h>

#include <glm-ortho-2d.h>

OpenGLModel::OpenGLModel(OpenGLRenderer& renderer) :
	renderer(renderer)
{	


	// add model to renderer's list
	renderer.models.push_front(this);

}



OpenGLModel::~OpenGLModel()
{

	renderer.models.remove(this);
	
}

void OpenGLModel::init(std::shared_ptr<MaterialInstance> mat, std::shared_ptr<ModelData> data, MeshComponent& ownerComp)
{
	material = std::static_pointer_cast<OpenGLMaterialInstance>(mat);
	modelData = std::static_pointer_cast<OpenGLModelData>(data);
	parent = &ownerComp;

}


MeshComponent& OpenGLModel::getOwnerComponent()
{
	check(parent);  return *parent;
}

const MeshComponent& OpenGLModel::getOwnerComponent() const
{
	check(parent); return *parent;
}



void OpenGLModel::draw()
{
	mat3 view = renderer.getCurrentCamera().getViewMat();
	mat3 model = parent->getModelMatrix();

	mat3 MVPmat = view * model;

	auto&& matSource = std::static_pointer_cast<OpenGLMaterialSource>(material->getSource());
	
	check(material);
	material->use();

	glUniformMatrix3fv(matSource->MVPUniformLocation, 1, GL_FALSE, &MVPmat[0][0]);
	

	glUniform1f(glGetUniformLocation(**matSource, "renderOrder"), 1.f);

	modelData->draw();
}
