#pragma once

// This class has a bunch of includes for serialization and a define
#include "Engine.h"

#define BOOST_SERIALIZATION_DYN_LINK 1

#include <boost/preprocessor/variadic.hpp>
#include <boost/preprocessor/iteration.hpp>
#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/stringize.hpp>



#include <boost/archive/polymorphic_iarchive.hpp>
#include <boost/archive/polymorphic_oarchive.hpp>

#include <boost/serialization/base_object.hpp>
#include <boost/serialization/access.hpp>
#include <boost/serialization/nvp.hpp>
#include <boost/serialization/export.hpp>

#include <boost/serialization/map.hpp>
#include <boost/serialization/list.hpp>
#include <boost/serialization/vector.hpp>

#include "SerializeGLM.h"

// before using this please define ACTOR_SAVE_DATA in the format:
// #define SAVE_DATA <this_class>, <parent_class>, <saved val 1>, <saved val 2>, etc **** NOTE WHITESPACE BETWEEN ACTOR_SAVE_DATA and <this_class>, IT IS VITAL
// USAGE: #include REGISTER_FOR_SAVING_SOURCE()
// put in the source file
#define REGISTER_FOR_SAVING_SOURCE() "registerForSerialization.h"

// put this in the header after the definition of the class -- this isn't an include
#define REGISTER_FOR_SAVING_HEADER(actorName, ModuleName)\
	BOOST_CLASS_EXPORT_KEY2(\
		actorName, \
		BOOST_PP_CAT(\
			BOOST_PP_CAT(ModuleName, "."),\
			BOOST_PP_STRINGIZE(actorName)))

#define INIT_SAVED_CLASS() \
	friend boost::serialization::access; \
	template <typename Archive>\
	void serialize(Archive& ar, const unsigned int version); \
	virtual bool needsSave()\
	{\
		return true;\
	}\
	/**/
