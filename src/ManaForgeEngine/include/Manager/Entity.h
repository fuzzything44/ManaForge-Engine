#pragma once

#include <bitset>
#include <array>

#include <MPLHelp.h>

#include <boost/mpl/size.hpp>

struct EntityBase
{
	bool(*hasComponentOrTag)(size_t componentOrTagID);
	void(*deleteComponents)(EntityBase*);

	~EntityBase()
	{
		deleteComponents(this);
	}
};



template<typename ManagerType, typename Signature>
struct Entity : EntityBase
{
	template<typename... Components>
	struct ComponentsOrTagsToIDs
	{
		constexpr static std::array<size_t, sizeof...(Components)> apply()
		{
			return std::array<size_t, sizeof...(Components)>({ ManagerType::template getComponentOrTagID<Components>()... });
		}
	};

	Entity()
	{
		hasComponentOrTag = 
			[](size_t ID)
		{
			constexpr std::array<size_t, boost::mpl::size<Signature>::type::value> IDs = 
				ExpandSequenceToVaraidic_t<Signature, ComponentsOrTagsToIDs>::apply();

			return std::find(IDs.begin(), IDs.end(), ID) != IDs.end();
		};

		deleteComponents = 
			[](EntityBase* base)
		{
			auto casted = static_cast<Entity<ManagerType, Signature>&>(*base);

			using Components = ManagerType::template RemoveTags_t<Signature>;

			for_each_no_construct_ptr<Components>(
				[](auto ptr)
			{
				using ComponentType = std::decay_t<std::remove_pointer_t<decltype(ptr)>>;

				static_assert(ManagerType::template isComponent<ComponentType>(), "Must be a component!");

				constexpr size_t componentID = ManagerType::template getComponentID<ComponentType>();

				auto&& componentStorage = manager.template getComponentStorage<ComponentType>();

				// call destructor 
				compoentStorage[componentIDs[componentID]].~ComponentType();

				// get last element to swap with


			});
		};
	}

	std::array<size_t, boost::mpl::size<Signature>::type::value> componentIDs;

	ManagerType& manager;

};