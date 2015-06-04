#include "OpenALSoundCue.h"

#include <Logging.h>

OpenALSoundCue::OpenALSoundCue(const std::string& name)
{
	std::string path = "sounds\\" + name + ".wav";

	bufferHandle = alutCreateBufferFromFile(path.c_str());

	ALint amtChannels;
	alGetBufferi(bufferHandle, AL_CHANNELS, &amtChannels);

	if (amtChannels == 2)
	{
		ENG_LOGLN("Warining, using stereo buffer: " << name << ". The sound will not be location based");
	}
}