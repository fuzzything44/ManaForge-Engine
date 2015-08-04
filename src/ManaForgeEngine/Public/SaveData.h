#pragma once

// This class has a bunch of includes for serialization and a define
#include "Engine.h"

#define BOOST_SERIALIZATION_DYN_LINK 1

#include "SerializeGLM.h"

#include <boost/mpl/vector.hpp>
#include <boost/mpl/at.hpp>
#include <boost/mpl/int.hpp>

#include <boost/serialization/export.hpp>


#include <boost/archive/polymorphic_iarchive.hpp>
#include <boost/archive/polymorphic_oarchive.hpp>

#include <type_traits>

class Actor;

template <typename T>
struct bases;


template <typename vector, int32 index>
struct serializeBases
{
	template <typename Archive, typename Derived>
	static inline void apply(Archive& ar, Derived& derived)
	{
		using Base = typename boost::mpl::at<vector, boost::mpl::int_<index>>::type;

		static_assert(std::is_base_of<Base, Derived>::type::value, "Derived needs to be derived from Base");

		ar& boost::serialization::make_nvp("base", boost::serialization::base_object<Base>(derived));

		serializeBases<vector, index - 1>::apply(ar, derived);
	}
};


template <typename vector>
struct serializeBases<vector, -1>
{
	template <typename Archive, typename Derived>
	static inline void apply(Archive& ar, Derived& derived)
	{
	}
};

template <typename... args>
struct serializeMembers_impl;

template <typename First, typename... Rest>
struct serializeMembers_impl<First, Rest...>
{
	template <typename Archive>
	static inline void apply(Archive& ar, First& first, Rest&... rest)
	{
		ar& boost::serialization::make_nvp("member", first);

		serializeMembers<Rest...>::apply(ar, rest...);
	}
};

template <>
struct serializeMembers_impl<>
{
	template <typename Archive>
	static inline void apply(Archive& ar)
	{
	}
};

template <typename Archive, typename... Types>
void serializeMembers(Archive& ar, Types&... values)
{
	serializeMembers_impl<Types...>::apply(ar, values...);
}

#define MFCLASS(className, ... /*bases*/)                                                                           \
	class className;                                                                                                \
	BOOST_CLASS_EXPORT_KEY2(className, BOOST_PP_CAT(BOOST_PP_CAT(MODULE_NAME, "."), BOOST_PP_STRINGIZE(className))) \
	template <>                                                                                                     \
	struct bases<className>                                                                                         \
	{                                                                                                               \
		using type = boost::mpl::vector<__VA_ARGS__>;                                                               \
	};

#define MFCLASS_SOURCE(className)                                                                            \
	BOOST_CLASS_EXPORT_IMPLEMENT(className)                                                                  \
                                                                                                             \
	template void className::serialize<>(boost::archive::polymorphic_oarchive&, const unsigned int version); \
	template void className::serialize<>(boost::archive::polymorphic_iarchive&, const unsigned int version);


#define MFCLASS_BODY(className, ... /*members to be serialized*/)                                                            \
	friend class boost::serialization::access;                                                                               \
	template <typename Archive>                                                                                              \
	void serialize(Archive& ar, const unsigned version)                                                                      \
	{                                                                                                                        \
		serializeBases<bases<className>::type, boost::mpl::size<bases<className>::type>::type::value - 1>::apply(ar, *this); \
                                                                                                                             \
		serializeMembers(ar, __VA_ARGS__);                                                                                   \
	}
