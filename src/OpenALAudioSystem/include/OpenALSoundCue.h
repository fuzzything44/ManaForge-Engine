#pragma once
#include "Engine.h"

#include <boost/filesystem.hpp>

#include <AL/alut.h>

struct OpenALSoundCue
{
	explicit OpenALSoundCue(const path_t& name)
		:name(name)
	{
		path_t path = "sounds/" + name.string() + ".wav";

		if(!boost::filesystem::exists(path)) MFLOG(Error) << "Cannot find " << path.string();

		bufferHandle = alutCreateBufferFromFile(path.string().c_str());

        if(bufferHandle == AL_NONE)
        {
            MFLOG(Error) << "Cannot create buffer from wav file " << path.string() << " with error string: " << alutGetErrorString(alutGetError());
        }


		ALint amtChannels;
		alGetBufferi(bufferHandle, AL_CHANNELS, &amtChannels);

		if (amtChannels == 2) {
			MFLOG(Info) << "using stereo buffer: " << name << ". The sound will not be location based";
		}
	}

	ALuint bufferHandle;
	path_t name;
};
