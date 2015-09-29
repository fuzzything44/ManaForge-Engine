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

OpenGLModel::OpenGLModel(OpenGLRenderer& renderer, uint8 renderOrder)
	: renderer(renderer)
	, renderOrder(renderOrder)
	, isValid(true)
{
}

OpenGLModel::~OpenGLModel() = default;

void OpenGLModel::init(
	std::shared_ptr<MaterialInstance> mat, std::shared_ptr<ModelData> data, MeshComponent& ownerComp)
{
	material = std::static_pointer_cast<OpenGLMaterialInstance>(mat);
	modelData = std::static_pointer_cast<OpenGLModelData>(data);
	parent = &ownerComp;
}

MeshComponent& OpenGLModel::getOwnerComponent()
{
	assert(parent);
	return *parent;
}

const MeshComponent& OpenGLModel::getOwnerComponent() const
{
	assert(parent);
	return *parent;
}

uint8 OpenGLModel::getRenderOrder() const { return renderOrder; }

void OpenGLModel::draw()
{
	assert(renderer.isOnRenderThread());

	if (!isValid) return;
	isValid = false; // set this so it doesn't get destructed while this is happening.

	mat3 view = renderer.getCurrentCamera().getViewMat();
	mat3 model = parent->getModelMatrix();

	mat3 MVPmat = view * model;

	auto&& matSource = static_cast<OpenGLMaterialSource*>(material->getSource());

	assert(material);
	material->use();

	glUniformMatrix3fv(matSource->MVPUniformLocation, 1, GL_FALSE, &MVPmat[0][0]);

	glUniform1f(glGetUniformLocation(**matSource, "renderOrder"), 1.f);

	modelData->draw();

	isValid = true; // ok -- were done -- you can destruct me now.
}
