#pragma once
#include "Engine.h"

#include "Transform.h"

// forward declarations
class MeshComponent;
class MaterialInstance;

class Model
{
  public:
	virtual ~Model() = default;

	virtual void init(
		std::shared_ptr<MaterialInstance> mat, std::shared_ptr<ModelData> data, MeshComponent& ownerComp) = 0;

	virtual MeshComponent& getOwnerComponent() = 0;
	virtual const MeshComponent& getOwnerComponent() const = 0;
};
