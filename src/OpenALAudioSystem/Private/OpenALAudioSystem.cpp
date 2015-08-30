#include "OpenALAudioSystem.h"

#include "OpenALSoundCue.h"
#include "OpenALSoundSource.h"

#include <ENGException.h>
#include <Runtime.h>
#include <Helper.h>
#include <Renderer.h>
#include <CameraComponent.h>

#include "AL/alut.h"

#include <boost/lexical_cast.hpp>

OpenALAudioSystem::OpenALAudioSystem()
{
	if (alutInit(nullptr, nullptr) == AL_FALSE) // catch errors
	{
		auto&& err = alutGetError();
		MFLOG(Warning) << "Failed to initalize alut with error: " << alutGetErrorString(err)
					   << " And error number: " << err
					   << "\n If you recieve this error, you won't have sound.";
	}


	float orientation[] = {
		0.f,
		1.f,
		0.f, // forward vector	Facing positive Y
		0.f,
		0.f,
		1.f // up vector		Z-up
	};

	alListenerfv(AL_ORIENTATION, orientation);
}

OpenALAudioSystem::~OpenALAudioSystem() {}

void OpenALAudioSystem::addSource(OpenALSoundSource* source) { sources.push_back(source); }

std::shared_ptr<SoundCue> OpenALAudioSystem::newSoundCue(const path_t& name)
{
	auto&& iter = cues.find(name);

	if (iter != cues.end()) {
		auto&& ptr = iter->second;
		if (ptr.expired())
		{
			iter->second = std::make_shared<OpenALSoundCue>(name);
		}
		return std::shared_ptr<OpenALSoundCue>(iter->second);
	}

	auto&& ret = std::make_shared<OpenALSoundCue>(name);
	cues.insert({ name, ret });

	return ret;

}

std::unique_ptr<SoundSource> OpenALAudioSystem::newSoundSource(SoundCue& cue, AudioComponent& owner)
{
	return std::make_unique<OpenALSoundSource>(static_cast<OpenALSoundCue&>(cue), owner, *this);
}

bool OpenALAudioSystem::update()
{
	auto&& camera = Runtime::get().getRenderer().getCurrentCamera();
	
	// setup listener properities
	auto&& cameraLoc = camera.getWorldLocation();
	alListener3f(AL_POSITION, cameraLoc.x, cameraLoc.y, 0.f);

	auto&& cameraVel = camera.getOwner().getVelocity();
	alListener3f(AL_VELOCITY, cameraVel.x, cameraVel.y, 0.f);

	for (auto& elem : sources)
	{
		elem->update();
	}

	return true;
}
