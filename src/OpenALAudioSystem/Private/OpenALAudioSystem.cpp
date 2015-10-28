#include "OpenALAudioSystem.h"


extern std::shared_ptr<OpenALAudioManager_t> audioManager = OpenALAudioManager_t::factory(std::make_tuple(coreManager));