#pragma once
#include "OpenGLRendererConfig.h"

#include <boost/serialization/nvp.hpp> // for BOOST_SERIALIZATION_NVP

#include <SerializeGLM.h>

struct OpenGLCharacterData
{
	vec2 uvBegin;
	vec2 uvEnd;
	float advance;
	vec2 offset;
	vec2 size;

	template <typename Archive> void serialize(Archive& ar, unsigned int version)
	{
		ar& BOOST_SERIALIZATION_NVP(uvBegin);
		ar& BOOST_SERIALIZATION_NVP(uvEnd);
		ar& BOOST_SERIALIZATION_NVP(advance);
		ar& BOOST_SERIALIZATION_NVP(offset);
		ar& BOOST_SERIALIZATION_NVP(size);
	}
};
