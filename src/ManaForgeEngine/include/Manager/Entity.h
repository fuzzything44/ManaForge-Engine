#pragma once

#include <bitset>
#include <array>

#include <MPLHelp.h>

#include <boost/mpl/size.hpp>

#include <boost/fusion/algorithm.hpp>

template<typename ManagerType>
struct Entity
{
    template<typename ... Args>
    using TupleOfEntityPointers = std::tuple<Entity<Args>*...>;

	ExpandSequenceToVaraidic_t<typename ManagerType::MyBases, TupleOfEntityPointers> bases;

    std::bitset<ManagerType::getNumComponents()> signature;

    std::vector<size_t> components;

    Entity(const decltype(signature)& sig_, decltype(components)&& comp_, const decltype(bases)& bases_)
		: signature(sig_)
		, components(std::move(comp_))
		, bases(bases_)
    {}
};
