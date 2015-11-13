#pragma once

#include <bitset>
#include <array>

#include <MPLHelp.h>

#include <boost/mpl/size.hpp>

template<typename ManagerType>
struct EntityBase
{
	bool(*hasComponentOrTag)(size_t componentOrTagID);
};

template<typename ManagerType, typename... Components> 
struct ComponentsOrTagsToIDs
{
	constexpr static std::array<size_t, sizeof...(Components)> apply()
	{
		return{ ManagerType::template getComponentOrTagID<Components>... };
	}
};

template<typename ManagerType, typename Signature>
struct Entity : EntityBase<ManagerType>
{
	Entity()
	{
		hasComponentOrTag = 
			[](size_t ID)
		{
			constexpr std::array<size_t, boost::mpl::size<Signature>::type::value> IDs = 
				ExpandSequenceToVaraidic_t<Signature, ComponentsOrTagsToIDs>::apply();

			return std::find(IDs.begin(), IDs.end(), ID) != IDs.end();
		}
	}

	std::array<size_t, boost::mpl::size<Signature>::type::value> componentIDs;

};