#include "Gate.h"
#include <Runtime.h>
#include <TextureLibrary.h>
#include <AudioSystem.h>

#include <math.h>

#define SAVE_DATA Gate, Actor, isOpen
#include REGISTER_FOR_SAVING_SOURCE()

Gate::Gate()
	: Actor()
{

	trans = Transform(vec2(4.f, 4.f), .25 * M_PI, vec2(4.f, 4.f));

	SoundCue* cue = Runtime::get().moduleManager.getAudioSystem().newSoundCue("test");
	audioComp = new AudioComponent(this, Transform{}, cue, true);
	audioComp->play();

	vec2 vertLocs[] =
	{
		vec2(-1.f, -1.f),
		vec2(1.f, -1.f),
		vec2(-1.f, 1.f),
		vec2(1.f, 1.f)
	};

	vec2 UVs[] =
	{
		vec2(0.f, 1.f),
		vec2(1.f, 1.f),
		vec2(0.f, 0.f),
		vec2(1.f, 0.f)
	};

	uvec3 tris[] = 
	{
		uvec3(0, 1, 2),
		uvec3(1, 2, 3)
	};

	Texture* tex = Runtime::get().moduleManager.getRenderer().newTexture("0");
	tex->setFilterMode(Texture::FilterMode::MIPMAP_LINEAR);

	Material* mat = Runtime::get().moduleManager.getRenderer().newMaterial("boilerplate");

	mat->setTexture(0, tex);

	meshComp = new MeshComponent(this, Transform{ }, ModelData
		(vertLocs, UVs, tris, 6, 2, mat));
}

Gate::~Gate()
{

}
