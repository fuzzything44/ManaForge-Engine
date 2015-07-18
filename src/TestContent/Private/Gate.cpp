#include "Gate.h"
#include <Runtime.h>
#include <TextureLibrary.h>
#include <AudioSystem.h>
#include <Texture.h>
#include <Renderer.h>
#include <MaterialInstance.h>
#include <ModelData.h>

#include <cmath>

#define SAVE_DATA Gate, Actor, isOpen, timeToDestruction
#include REGISTER_FOR_SAVING_SOURCE()


bool Gate::isInitalized = false;

Gate::Gate()
	: Actor(), timeToDestruction(3.f)
{

	vec2 vertLocs[] =
	{
		{ -1.f, -1.f },
		{ +1.f, -1.f },
		{ -1.f, +1.f },
		{ +1.f, +1.f }
	};

	vec2 UVs[] =
	{
		{0.f, 1.f},
		{1.f, 1.f},
		{0.f, 0.f},
		{1.f, 0.f}
	};

	uvec3 tris[] = 
	{
		{0, 1, 2},
		{1, 2, 3}
	};

	auto tex = Runtime::get().moduleManager.getRenderer().getTexture("0");

	tex->setFilterMode(Texture::FilterMode::MIPMAP_LINEAR);

	
	mat = Runtime::get().moduleManager.getRenderer().newMaterial(
		Runtime::get().moduleManager.getRenderer().getMaterialSource("boilerplate"));

	mat->setTexture(0, tex);


	


	auto modelData = Runtime::get().moduleManager.getRenderer().newModelData();
	modelData->init(vertLocs, UVs, 6, tris, 2);
	meshComp = std::make_unique<MeshComponent>(*this, Transform{}, *mat, std::move(modelData));

	auto shape = Runtime::get().moduleManager.getPhysicsSystem().newPhysicsShape();

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
Gate::~Gate()
{
	
}
