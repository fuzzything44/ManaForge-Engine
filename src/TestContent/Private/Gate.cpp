#include "Gate.h"
#include <Runtime.h>
#include <TextureLibrary.h>
#include <AudioSystem.h>
#include <Texture.h>
#include <Renderer.h>
#include <MaterialInstance.h>
#include <ModelData.h>

#include <cmath>

MFCLASS_SOURCE(Gate)

bool Gate::isInitalized = false;

Gate::Gate() : Actor(), timeToDestruction(3.f)
{

	auto tex = Runtime::get().renderer->getTexture("0");

	tex->setFilterMode(Texture::FilterMode::MIPMAP_LINEAR);


	mat = Runtime::get().renderer->newMaterial(Runtime::get().renderer->getMaterialSource("boilerplate"));

	mat->setTexture(0, tex);


	auto modelData = Runtime::get().renderer->newModelData("GateMesh");
	if (!modelData->isInitialized()) {

		vec2 vertLocs[] = {{-1.f, -1.f}, {+1.f, -1.f}, {-1.f, +1.f}, {+1.f, +1.f}};

		vec2 UVs[] = {{0.f, 1.f}, {1.f, 1.f}, {0.f, 0.f}, {1.f, 0.f}};

		uvec3 tris[] = {{0, 1, 2}, {1, 2, 3}};

		modelData->init(vertLocs, UVs, 6, tris, 2);
	}
	meshComp = std::make_unique<MeshComponent>(*this, Transform{}, std::move(mat), std::move(modelData));

	auto shape = Runtime::get().physSystem->newPhysicsShape();

	setPhysicsType(PhysicsType::DYNAMIC);


	shape->asRectangle(.75f, .75f);

	physComp = std::make_unique<PhysicsComponent>(*this, *shape, Transform{});
	physComp->setDensity(1.f);
}
void Gate::tick(float deltaTime)
{
	timeToDestruction -= deltaTime;
	if (timeToDestruction <= 0) {
		delete this;
	}
}
Gate::~Gate() {}
