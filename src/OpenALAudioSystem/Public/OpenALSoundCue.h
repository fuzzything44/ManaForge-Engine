#pragma once

#include "OpenALAudioSystemConfig.h"

#include <string>


#include <AL/alut.h>

struct OpenALSoundCue
{
	explicit OpenALSoundCue(const path_t& name)
		:name(name)
	{
		path_t path = L"sounds\\" + name.wstring() + L".wav";

		bufferHandle = alutCreateBufferFromFile(path.string().c_str());

		ALint amtChannels;
		alGetBufferi(bufferHandle, AL_CHANNELS, &amtChannels);

		if (amtChannels == 2) {
			MFLOG(Info) << "using stereo buffer: " << name << ". The sound will not be location based";
		}
	}

	ALuint bufferHandle;
	path_t name;
};