#pragma once

#include <bitset>
#include <array>

#include <MPLHelp.h>

#include <boost/mpl/size.hpp>

#include <boost/fusion/algorithm.hpp>

template<typename ManagerType_>
struct Entity
{
	using ManagerType = ManagerType_;


    template<typename ... Args>
    using TupleOfEntityPointers = std::tuple<Entity<Args>*...>;

	ExpandSequenceToVaraidic_t<typename ManagerType::MyBases, TupleOfEntityPointers> bases;

    typename ManagerType::MyRuntimeSignature_t signature;

    std::array<size_t, ManagerType::getNumMyStorageComponents()> components;

};
