#pragma once
#include "Engine.h"

#include <RefCounted.h>

#include <boost/filesystem.hpp>

#include <AL/alut.h>

struct OpenALSoundCue : RefCounted<OpenALSoundCue>
{
	explicit OpenALSoundCue(const path_t& name = "")
		:name(name)
	{
        if(!name.empty())
        {
            init(name);
        }

	}

    void init(const path_t& name)
    {
        path_t path = "sounds/" + name.string() + ".wav";

		if(!boost::filesystem::exists(path)) MFLOG("Cannot find " << path.string());

		bufferHandle = alutCreateBufferFromFile(path.string().c_str());

        if(bufferHandle == AL_NONE)
        {
            MFLOG("Cannot create buffer from wav file " << path.string() << " with error string: " << alutGetErrorString(alutGetError()));
        }


		ALint amtChannels;
		alGetBufferi(bufferHandle, AL_CHANNELS, &amtChannels);

		if (amtChannels == 2) {
			MFLOG("using stereo buffer: " << name << ". The sound will not be location based");
		}
    }

    OpenALSoundCue(const OpenALSoundCue& other) = default;
    OpenALSoundCue(OpenALSoundCue&& other) = default;

    OpenALSoundCue& operator=(const OpenALSoundCue& other) = default;
    OpenALSoundCue& operator=(OpenALSoundCue&& other) = default;

    void destroy()
    {
        alDeleteBuffers(1, &bufferHandle);
    }

    void invalidate()
    {
        bufferHandle = 0;
        name = "";
    }



	ALuint bufferHandle;
	path_t name;
};
