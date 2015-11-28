#pragma once
#include "COpenALSoundSource.h"
#include "OpenALSoundSystemConfig.h"

#include <CoreManager.h>

#include <AL/alut.h>

using OpenALAudioManager_t =
	Manager
	<
		boost::mpl::vector1<COpenALSoundSource>
		, boost::mpl::vector1<CoreManager_t>
	>;


template<>
void updateManager<OpenALAudioManager_t>(OpenALAudioManager_t& manager)
{

	// setup listener properities
	//auto&& cameraLoc = manager.getComponent<CPosition>(camera).value; TODO: implement
	//alListener3f(AL_POSITION, cameraLoc.x, cameraLoc.y, 0.f);

	//auto&& cameraVel = camera.getOwner().getVelocity();
	//alListener3f(AL_VELOCITY, cameraVel.x, cameraVel.y, 0.f);

	manager.runAllMatching<boost::mpl::vector<COpenALSoundSource, CPosition>>([](COpenALSoundSource& src, CPosition& pos)
	{
		// update runtime variables
		alSource3f(src.sourceHandle, AL_POSITION, pos.value.x, pos.value.y, 0.f);

	});

	manager.runAllMatching<boost::mpl::vector<COpenALSoundSource, CVelocity>>([](COpenALSoundSource& src, CVelocity& vel)
	{
		// update runtime variables
		alSource3f(src.sourceHandle, AL_VELOCITY, vel.value.x, vel.value.y, 0.f);

	});

}

template<>
void exitManager<OpenALAudioManager_t>(OpenALAudioManager_t& manager)
{
	alutExit();
}

template<>
void initManager<OpenALAudioManager_t>(OpenALAudioManager_t& manager)
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

extern OpenALAudioSystem_API OpenALAudioManager_t* openALAudioManager;
