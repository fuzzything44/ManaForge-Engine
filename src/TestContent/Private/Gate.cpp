#include "Gate.h"
#include <Runtime.h>
#include <TextureLibrary.h>

#define ACTOR_SAVE_DATA Gate, Actor, isOpen

#include ACTOR_REGISTER_FOR_SAVING_SOURCE()

Gate::Gate(const Transform& trans)
	: Actor(trans)
{
	vec2 locations[] = {
		vec2(-1.f, -1.f),
		vec2(-1.f, +1.f),
		vec2(+1.f, -1.f),
		vec2(+1.f, +1.f) 
	};

	uvec3 elems[] =
	{	
		uvec3(0, 1, 2),
		uvec3(1, 2, 3) 
	};

	TextureLibrary* tex = Runtime::get().moduleManager.getRenderer().newTextureLibrary(4, 256);
	tex->addImage("1");
	tex->addImage("2");
	tex->addImage("3");
	tex->addImage("4");

	

	Material* mat = Runtime::get().moduleManager.getRenderer().newMaterial("boilerplate");
	mat->setTexture(0, reinterpret_cast<Texture*>(tex));


	gateMesh = new MeshComponent(
		this, Transform{}, ModelData(
			Transform{}, locations, tex->getUVCoords("1").toPtr(), elems, 4, 6, mat));

}

Gate::~Gate()
{

}
