#include "Gate.h"
#include <Runtime.h>
#include <TextureLibrary.h>
#include <AudioSystem.h>
#include <Texture.h>
#include <Renderer.h>
#include <MaterialInstance.h>
#include <ModelData.h>
#include <TimerManager.h>

#include <cmath>

MFCLASS_SOURCE(Gate)

bool Gate::isInitalized = false;

Gate::Gate()
	: Actor()
{
	auto tex = Runtime::get().getRenderer().getTexture("water");

	tex->setFilterMode(Texture::FilterMode::MIPMAP_LINEAR);

	mat = Runtime::get().getRenderer().newMaterialInstance(
		Runtime::get().getRenderer().getMaterialSource("animation"));
	mat->setProperty("tiles", 2);
	mat->setUpdateCallback([time = 0.f](MaterialInstance & inst) mutable
		{
			time += Runtime::get().getDeltaTime();

			inst.setProperty("currentTile", int(time * 10) % 4);
		});

	mat->setTexture(0, tex);

	auto modelData = Runtime::get().getRenderer().newModelData("GateMesh");
	if (!modelData->isInitialized()) {

		vec2 vertLocs[] = {{-1.f, -1.f}, {+1.f, -1.f}, {-1.f, +1.f}, {+1.f, +1.f}};

		vec2 UVs[] = {{0.f, 1.f}, {1.f, 1.f}, {0.f, 0.f}, {1.f, 0.f}};

		uvec3 tris[] = {{0, 1, 2}, {1, 2, 3}};

		modelData->init(vertLocs, UVs, 6, tris, 2);
	}
	meshComp = std::make_unique<MeshComponent>(*this, Transform{}, std::move(mat), std::move(modelData));

	auto shape = Runtime::get().getPhysicsSystem().newPhysicsShape();

	setPhysicsType(PhysicsType::DYNAMIC);

	shape->asRectangle(.75f, .75f);

	physComp = std::make_unique<PhysicsComponent>(*this, *shape, Transform{});
	physComp->setDensity(1.f);

	using namespace std::chrono_literals;

	Runtime::get().getTimerManager().addTimer(1s,
		[this]()
		{
			delete this;
		},
		false);
}

Gate::~Gate() = default;
