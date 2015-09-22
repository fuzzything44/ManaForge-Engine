#pragma once
#include "Engine.h"

#include <memory>

// forward declarations
class MeshComponent;
class MaterialInstance;

class Model
{
protected:
	virtual ~Model() = default;
public:

	inline static void deleter(Model* ptr);

	virtual void init(
		std::shared_ptr<MaterialInstance> mat, std::shared_ptr<ModelData> data, MeshComponent& ownerComp) = 0;

	virtual uint8 getRenderOrder() const = 0;

	virtual MeshComponent& getOwnerComponent() = 0;
	virtual const MeshComponent& getOwnerComponent() const = 0;
};

#include "Runtime.h"
#include "Renderer.h"

inline void Model::deleter(Model* ptr)
{
	Runtime::get().getRenderer().deleteModel(ptr);
}
