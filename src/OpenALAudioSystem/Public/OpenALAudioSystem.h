#pragma once

#include "OpenALAudioSystemConfig.h"

#include "COpenALSoundSource.h"

#include <CoreManager.h>



using AudioManager_t =
	Manager
	<
		boost::mpl::vector<COpenALSoundSource>
		, boost::mpl::vector0<>
		, boost::mpl::vector1<CoreManager_t>
	>;

template<> 
void updateManager<AudioManager_t>(AudioManager_t& manager)
{
	std::cout << "HEY";
}

std::shared_ptr<AudioManager_t> audioManager = AudioManager_t::factory(std::make_tuple(coreManager));