#pragma once
#include "COpenALSoundSource.h"

#include <CoreManager.h>



using OpenALAudioManager_t =
	Manager
	<
		boost::mpl::vector<COpenALSoundSource>
		, boost::mpl::vector0<>
		, boost::mpl::vector1<CoreManager_t>
	>;

template<> 
void updateManager<OpenALAudioManager_t>(OpenALAudioManager_t& manager)
{
	std::cout << "HEY";
}

extern std::shared_ptr<OpenALAudioManager_t> audioManager;