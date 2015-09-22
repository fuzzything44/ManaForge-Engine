#pragma once

#include <Engine.h>
#include <ModelData.h>
#include <Model.h>

#include <atomic>

#include "OpenGLMaterialInstance.h"

class OpenGLModelData;

class OpenGLModel final : public Model
{
public:
	friend class OpenGLRenderer;

	explicit OpenGLModel(OpenGLRenderer& renderer, uint8 renderOrder);
	virtual ~OpenGLModel();

	virtual void init(std::shared_ptr<MaterialInstance> mat,
		std::shared_ptr<ModelData> data,
		MeshComponent& ownerComp) override;

	virtual MeshComponent& getOwnerComponent() override;
	virtual const MeshComponent& getOwnerComponent() const override;

	virtual uint8 getRenderOrder() const override;

	void draw();

private:
	uint8 renderOrder;

	std::atomic<bool> isValid;

	std::shared_ptr<OpenGLModelData> modelData;
	std::shared_ptr<OpenGLMaterialInstance> material;

	MeshComponent* parent;

	std::list<OpenGLModel*>::iterator location;

	OpenGLRenderer& renderer;
};