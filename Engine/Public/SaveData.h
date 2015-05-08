#pragma once

// This class has a bunch of includes for serialization and a define

#include <boost/preprocessor/variadic.hpp>
#include <boost/preprocessor/iteration.hpp>
#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/stringize.hpp>

#include <boost/archive/xml_iarchive.hpp>
#include <boost/archive/xml_oarchive.hpp>

#include <boost/archive/polymorphic_iarchive.hpp>
#include <boost/archive/polymorphic_oarchive.hpp>

#include <boost/serialization/base_object.hpp>
#include <boost/serialization/access.hpp>
#include <boost/serialization/nvp.hpp>

// before using this please define ACTOR_SAVE_DATA in the format:
// #define ACTOR_SAVE_DATA <this_class>, <parent_class>, <saved val 1>, <saved val 2>, etc **** NOTE WHITESPACE BETWEEN ACTOR_SAVE_DATA and <this_class>, IT IS VITAL
// USAGE: #include ACTOR_REGISTER_FOR_SAVING()
#define ACTOR_REGISTER_FOR_SAVING() "registerForSerialization.h"

#define INIT_SAVED_CLASS() \
	friend boost::serialization::access; \
	template <typename Archive>\
	void serialize(Archive& ar, const unsigned int version); \
  /**/
