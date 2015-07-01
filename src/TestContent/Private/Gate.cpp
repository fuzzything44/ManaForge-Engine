#include "Gate.h"
#include <Runtime.h>
#include <TextureLibrary.h>
#include <AudioSystem.h>
#include <Texture.h>
#include <Renderer.h>
#include <Material.h>

#include <cmath>

#define SAVE_DATA Gate, Actor, isOpen
#include REGISTER_FOR_SAVING_SOURCE()

Gate::Gate()
	: Actor()
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

	Texture* tex = Runtime::get().moduleManager.getRenderer().newTexture("0");
	tex->setFilterMode(Texture::FilterMode::MIPMAP_LINEAR);

	mat = Runtime::get().moduleManager.getRenderer().newMaterial("boilerplate");

	mat->setTexture(0, tex);

	meshComp = new MeshComponent(this, Transform{ }, ModelData
		(vertLocs, UVs, tris, 6, 2, mat));



	PhysicsShape* shape = Runtime::get().moduleManager.getPhysicsSystem().newPhysicsShape();

	setPhysicsType(PhysicsType::DYNAMIC);

	shape->asRectangle(.75f, .75f);

	physComp = new PhysicsComponent(this, Transform{}, shape);
	physComp->setDensity(1.f);
	

}

Gate::~Gate()
{
	delete mat;
}
