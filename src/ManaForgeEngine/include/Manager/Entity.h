#pragma once

#include <bitset>
#include <array>

#include <MPLHelp.h>

#include <boost/mpl/size.hpp>

#include <boost/fusion/algorithm.hpp>

struct EntityBase
{
	bool(*hasComponentOrTag)(size_t componentOrTagID);
	size_t(*getComponentID)(EntityBase*, size_t componentID);
	void(*deleteComponents)(EntityBase*);

	~EntityBase()
	{
		deleteComponents(this);
	}
};



template<typename ManagerType, typename Signature>
struct Entity : EntityBase
{
	static_assert(std::is_base_of<ManagerBase, ManagerType>::value, "Must be a subclass of manager!");
	static_assert(boost::mpl::is_sequence<Signature>::type::value, "Must be a sequence!");
	static_assert(ManagerType::template isSignature<Signature>(), "Must be a valid signature!");

	template<typename... Components>
	struct getComponentID_IMPL
	{
		constexpr static std::array<size_t, sizeof...(Components)> apply()
		{
			return std::array<size_t, sizeof...(Components)>({ ManagerType::template getComponentID<Components>()... });
		}
	};

	Entity(ManagerType& manager)
		:manager(manager)
	{
		hasComponentOrTag = 
			[](size_t ID)
		{
			constexpr std::array<size_t, boost::mpl::size<Signature>::type::value> IDs = 
				ExpandSequenceToVaraidic_t<Signature, getComponentID_IMPL>::apply();

			return std::find(IDs.begin(), IDs.end(), ID) != IDs.end();
		};

		deleteComponents = 
			[](EntityBase* base)
		{
			auto casted = static_cast<Entity<ManagerType, Signature>&>(*base);

			using StorageComponents = typename ManagerType::template IsolateStorageComponents_t<Signature>;

			for_each_no_construct_ptr<StorageComponents>(
				[&casted](auto ptr)
			{
				using ComponentType = std::decay_t<std::remove_pointer_t<decltype(ptr)>>;

				static_assert(ManagerType::template isComponent<ComponentType>(), "Must be a component!");

				constexpr size_t componentID = ManagerType::template getComponentID<ComponentType>();

				auto&& componentStorage = casted.manager.template getComponentStorage<ComponentType>();

				// call destructor 
				componentStorage[casted.componentIDs[componentID]].~ComponentType();

				// get last element to swap with


			});
		};

		getComponentID = [](EntityBase* base, size_t componentID)
		{
			assert(base->hasComponentOrTag(componentID));

			auto&& casted = static_cast<Entity<ManagerType, Signature>&>(*base);

			constexpr std::array<size_t, boost::mpl::size<Signature>::type::value> IDs =
				ExpandSequenceToVaraidic_t<Signature, getComponentID_IMPL>::apply();

			return *std::find(IDs.begin(), IDs.end(), componentID);

		};
	}

	std::array<size_t, boost::mpl::size<Signature>::type::value> componentIDs;

	ManagerType& manager;

};
