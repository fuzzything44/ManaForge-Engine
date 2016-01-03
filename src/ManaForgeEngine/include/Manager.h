#pragma once

#include "Engine.h"

#include <boost/hana.hpp>


#include <call_from_tuple.h>

#include <boost/serialization/strong_typedef.hpp>

#include <boost/fusion/adapted/std_tuple.hpp>
#include <boost/fusion/algorithm/iteration/for_each.hpp>

#include <boost/dynamic_bitset.hpp>

#include <vector>
#include <utility>
#include <bitset>
#include <tuple>
#include <type_traits>
#include <cassert>
#include <unordered_map>

#include "MPLHelp.h"

#include "Manager/Callbacks.h"
#include "Manager/GetRefToManager.h"
#include "Manager/FindAllManagers.h"
#include "Manager/MiscMetafunctions.h"
#include "Manager/Entity.h"


#undef max

struct ManagerBase{};

template <typename Components_, typename Bases_ = boost::hana::set<> >
struct Manager : ManagerBase
{
	// GLOBAL TYPEDEFS
	static constexpr auto myComponents = boost::hana::make<Components_>();
	static constexpr auto myBases = boost::hana::make<Bases_>();
	using This_t = Manager<Components_, Bases_>;
	
	static constexpr auto allManagers = myBases;//TODO: fix typename detail::FindManagers<boost::mpl::vector<This_t>>::type;
	
	static constexpr auto allManagersExceptThis = myBases;//TODO: fix typename boost::mpl::remove<AllManagers, This_t>::type;
	
	static constexpr auto allComponents = myComponents;//TODO: fix CatSequences_t<MyComponents, Unduplicate_t<ExpandSequenceToVaraidic_t<AllManagersButThis, CatComponents_t>>>;

	static constexpr auto myStorageComponents = boost::hana::fold(myComponents, boost::hana::make_set(), [](auto set, auto newElement)
		{
			return boost::hana::if_(std::is_empty<typename decltype(newElement)::type>::value, set, boost::hana::append(set, newElement));
		}
	);
	static constexpr auto myTagComponents = boost::hana::fold(myComponents, boost::hana::make_set(), [](auto set, auto newElement)
		{
			return boost::hana::if_(std::is_empty<typename decltype(newElement)::type>::value, boost::hana::append(set, newElement), set);
		}
	);

	static constexpr auto allStorageComponents = boost::hana::fold(allComponents, boost::hana::make_set(), [](auto set, auto newElement)
		{
			return boost::hana::if_(std::is_empty<typename decltype(newElement)::type>::value, set, boost::hana::append(set, newElement));
		}
	);
	static constexpr auto allTagComponents = boost::hana::fold(allComponents, boost::hana::make_set(), [](auto set, auto newElement)
		{
			return boost::hana::if_(std::is_empty<typename decltype(newElement)::type>::value, boost::hana::append(set, newElement), set);
		}
	);

	template<typename... Args>
	using TupleOfPtrs = std::tuple<Args*...>;

	// CONSTEXPR FUNCTIONS/TESTS


	static constexpr auto numComponents = boost::hana::size(allComponents);
	
	static constexpr auto numMyComponents = boost::hana::size(myComponents);
	
	template<typename T> 
	static constexpr auto isComponent(T componentToTest)
	{
		return boost::hana::contains(allComponents, componentToTest);
	}
	template<typename T> 
	static constexpr auto isMyComponent(T componentToTest)
	{
		return boost::hana::contains(myComponents, componentToTest);
	}
	template <typename T> 
	static constexpr auto getComponentID(T component)
	{
		BOOST_HANA_CONSTANT_CHECK(isComponent(component));
		
		return get_index_of_first_matching(allComponents, component);
	}
	template <typename T> 
	static constexpr auto getMyComponentID(T component)
	{
		BOOST_HANA_CONSTANT_CHECK(isComponent(component));
		
		return get_index_of_first_matching(myComponents, component);
	}

	static constexpr auto numStorageComponents = boost::hana::size(allStorageComponents);
	static constexpr auto numMyStorageComponents = boost::hana::size(allStorageComponents);
	
	template<typename T> 
	static constexpr auto isStorageComponent(T componentToTest)
	{
		return boost::hana::contains(allStorageComponents, componentToTest);
	}
	template<typename T> 
	static constexpr auto isMyStorageComponent(T componentToTest)
	{
		return boost::hana::contains(myStorageComponents, componentToTest);
	}
	template <typename T> 
	static constexpr auto getStorageComponentID(T component)
	{
		BOOST_HANA_CONSTANT_CHECK(isStorageComponent(component));
		
		return get_index_of_first_matching(allStorageComponents, component);
	}
	
	template<typename T> 
	static constexpr auto isTagComponent(T componentToTest)
	{
		return boost::hana::contains(allTagComponents, componentToTest);
	}
	template<typename T> 
	static constexpr auto isMyTagComponent(T componentToTest)
	{
		return boost::hana::contains(myTagComponents, componentToTest);
	}
	template <typename T> 
	static constexpr auto getTagComponentID(T component)
	{
		BOOST_HANA_CONSTANT_CHECK(isTagComponent(component));
		
		return get_index_of_first_matching(allTagComponents, component);
	}
	
	static constexpr auto numManagers = boost::hana::size(allManagers);
	
	template <typename T> 
	static constexpr auto getManagerID(T manager)
	{
		BOOST_HANA_CONSTANT_CHECK(isManager(manager));
		
		return get_index_of_first_matching(allManagers, manager);
	}
	template<typename T> 
	static constexpr auto getManagerExceptThisID(T manager)
	{
		BOOST_HANA_CONSTANT_CHECK(isExceptThisManager(manager));

		return get_index_of_first_matching(allManagersExceptThis, manager);
	}

	template<typename T> 
	static constexpr auto isBase(T baseToTest)
	{
		return boost::hana::contains(myBases, baseToTest);
	}

	static constexpr auto numBases = boost::hana::size(myBases);
	
	template <typename T> 
	static constexpr auto getBaseID(T base)
	{
		BOOST_HANA_CONSTANT_CHECK(isBase(base));
		
		return get_index_of_first_matching(myBases, base);
	}
	
	template<typename T>
	static constexpr auto isSignature(T signature)
	{
		return boost::hana::all_of(signature, isComponent);
	}

	template<typename T>
	static constexpr auto getManagerFromComponent(T component)
	{
		return boost::hana::fold(allManagers, boost::mpl::void_, [component](auto toTest, auto last)
			{
				return boost::hana::if_(decltype(toTest)::type::isComponent(component), toTest, last);
			}
		);
	}


	template<typename T>
	static constexpr auto isolateStorageComponents(T toIsolate)
	{
		return boost::hana::fold(toIsolate, boost::hana::make_set(), [](auto toTest, auto currentSet)
			{
				return boost::hana::if_(isStorageComponent(toTest), boost::hana::append(currentSet, toTest), currentSet);
			}
		);
	}
	template<typename T>
	static constexpr auto isolateTagComponents(T toIsolate)
	{
		return boost::hana::fold(toIsolate, boost::hana::make_set(), [](auto toTest, auto currentSet)
			{
				return boost::hana::if_(isTagComponent(toTest), boost::hana::append(currentSet, toTest), currentSet);
			}
		);
	}
	template<typename T>
	static constexpr auto isolateMyComponents(T toIsolate)
	{
		return boost::hana::fold(toIsolate, boost::hana::make_set(), [](auto toTest, auto currentSet)
			{
				return boost::hana::if_(isMyComponent(toTest), boost::hana::append(currentSet, toTest), currentSet);
			}
		);
	}
	
	using RuntimeSignature_t = std::bitset<This_t::numComponents>;

	template<typename T>
	static RuntimeSignature_t generateRuntimeSignature(T signature)
	{

		BOOST_HANA_CONSTANT_CHECK(isSignature(signature));

		RuntimeSignature_t ret;

		boost::hana::for_each(signature, [&ret](auto type)
			{
				ret[decltype(getComponentID(type))::value] = true;
			}
		);

		return ret;
	}

	template<typename T, typename Components>
	auto newEntity(T signature, const Components& components
		= decltype(components){} /*tuple of the components*/)
	{
		using namespace boost::hana::literals;
		
		BOOST_HANA_CONSTANT_CHECK(isSignature(signature));
		BOOST_HANA_CONSTANT_CHECK(boost::hana::size(signature) > 0_c);

		static constexpr auto StorageComponents = isolateStorageComponents(signature);
		using TagComponents = IsolateTagComponents_t<Signature>;

		// get managers for the entity
		using MostBaseManager = FindMostBaseManagerForSignature_t<Signature>;
		using Managers_DUP = typename boost::mpl::transform<Signature, GetBaseManager_STRUCT<boost::mpl::placeholders::_1>>::type;
		using ManagersForSignature = Unduplicate_t<typename boost::mpl::push_back<Managers_DUP, MostBaseManager>::type>;


		// construct the components
		ExpandSequenceToVaraidic_t<ManagersForSignature, TupleOfEntityPtrs> entities;

		// construct the entities
		boost::fusion::for_each(entities,
			[](auto& entPtr)
		{
			entPtr = new std::decay_t<decltype(*entPtr)>{};
		});

		std::array<size_t, boost::mpl::size<StorageComponents>::value> componentIDs;
		ExpandSequenceToVaraidic_t<StorageComponents, TupleOfPtrs> componentPtrs;

		// construct component
		tuple_for_each_with_index(componentPtrs,
			[this, &componentIDs, &componentPtrs, &components, &entities](auto& ptr, auto ID_t)
		{
			using ComponentType = std::remove_pointer_t<std::decay_t<decltype(ptr)>>;
			constexpr size_t ID = decltype(ID_t)::value;

			using ManagerType = GetManagerFromComponent_t<ComponentType>;

			auto&& compStorage = this->template getRefToManager<ManagerType>().template getComponentStorage<ComponentType>();

			compStorage.emplace_back(std::move(std::get<ID>(components)));

			size_t compID = compStorage.size() - 1;
			std::get<ID>(componentPtrs) = &compStorage[compID];
			componentIDs[ID] = compID;

			// assign the entity
			auto&& entityStorage = this->template getRefToManager<ManagerType>()
				.template getComponentEntityStorage<ComponentType>();

			// acquire the pointer
			auto ptrToEntity = std::get<boost::mpl::distance<typename boost::mpl::begin<ManagersForSignature>::type,
				typename boost::mpl::find<ManagersForSignature, ManagerType>::type
			>::type::value>(entities);

			// assign it
			entityStorage.emplace_back(ptrToEntity);

		});

		// assign bases
		boost::fusion::for_each(entities,
			[&entities](auto& entity)
		{
			using EntityType = std::decay_t<decltype(*entity)>;
			using ManagerType = typename EntityType::ManagerType;
			using Bases = typename Manager::MyBases;

			boost::fusion::for_each(entity->bases,
				[&entities](auto& basePtr)
			{
				using Entity_t = std::decay_t<decltype(*basePtr)>;
				using BaseManager_t = typename Entity_t::ManagerType;

				detail::AssignBasePointer_t<ManagersForSignature, BaseManager_t>::apply(basePtr, entities);

			});
		});


		boost::fusion::for_each(entities,
			[&componentIDs](auto& entityPtr)
		{
			using EntityType = std::decay_t<decltype(*entityPtr)>;
			using ManagerType = typename EntityType::ManagerType;

			using SignatureForManager = typename ManagerType::template IsolateComponentsFromThisManager_t<Signature>;
			static_assert(ManagerType::template isSignature<SignatureForManager>(), "Error, some components aren't valid.");
			using StorageComponents = IsolateStorageComponents_t<SignatureForManager>;

			entityPtr->signature = ManagerType::template generateRuntimeSignature<SignatureForManager>();

			// construct the components array
			for_each_no_construct_ptr<StorageComponents>(
				[&entityPtr, &componentIDs](auto storageComponentPtr)
			{
				using ComponentType = std::remove_pointer_t<decltype(storageComponentPtr)>;
				using ComponentManagerType = GetManagerFromComponent_t<ComponentType>;


				size_t ID = std::get<boost::mpl::distance<typename boost::mpl::begin<StorageComponents>::type,
					typename boost::mpl::find<StorageComponents, ComponentType>::type>::type::value>(componentIDs);

				ManagerType::template getEntityPtr<ComponentManagerType>(entityPtr)->components
					[ComponentManagerType::template getMyStorageComponentID<ComponentType>()] = ID;
			});
		});

		return std::get<boost::mpl::distance<typename boost::mpl::begin<ManagersForSignature>::type,
			typename boost::mpl::find<ManagersForSignature, MostBaseManager>::type>::type::value>(entities);

	}

	template<typename...Args>
	using TupleOfVectorRefrences = std::tuple<std::vector<Args>&...>;
	
	// returns the elements created [first, last)
	template<typename T, typename Components>
	std::pair<size_t, size_t> createEntityBatch(T signature, Components components, size_t numToConstruct)
	{
		// TODO: implement
	}
	void destroyEntity(Entity<This_t>* handle)
	{

	}

	template<typename T>
	auto getStorageComponent(T component, Entity<This_t>* handle) -> typename decltype(component)::type&
	{
		BOOST_HANA_CONSTANT_CHECK(isStorageComponent(component));
		
		constexpr auto managerForComponent = getManagerFromComponent(Component);

		constexpr auto staticID = decltype(managerForComponent)::type::template getMyStorageComponentID(component);

		auto* ent = getEntityPtr(managerForComponent, handle);

		size_t componentID = ent->components[staticID];

		return getComponentStorage(component)[componentID];

		
	}

	template<typename Component>
	bool hasComponent(Entity<This_t>* entity)
	{
		static_assert(isComponent<Component>(), "Error: must be a component!");
		using ManagerForComponent = GetManagerFromComponent_t<Component>;

		auto ent = getEntityPtr<ManagerForComponent>(entity);

		return ent->signature[ManagerForComponent::template getComponentID<Component>()];
	}
	
	template<typename ManagerToGet, bool dummy = true /*just to avoid full sepcialization, which isn't standard complient*/>
	struct GetEntityPtr_IMPL
	{
		static auto apply(Entity<This_t>* ent)
		{
			BOOST_HANA_CONSTANT_CHECK(isManager(boost::hana::type_c<ManagerToGet>{}));
			assert(ent);

			return std::get<getManagerExceptThisID<ManagerToGet>()>(ent->bases);

		}
	};
	template<bool dummy>
	struct GetEntityPtr_IMPL<This_t, dummy>
	{
		static auto apply(Entity<This_t>* ent)
		{
			return ent;
		}
	};
	
	template<typename ManagerToGet>
	static Entity<ManagerToGet>* getEntityPtr(Entity<This_t>* ent)
	{
		return GetEntityPtr_IMPL<ManagerToGet>::apply(ent);
	}
	
	template<typename ManagerToGet>
	ManagerToGet& getRefToManager()
	{
		static_assert(isManager<ManagerToGet>(), "ManagerToGet must be a manager");

		return detail::GetRefToManager<This_t, ManagerToGet>::apply(*this);
	}

	template<typename CurrentIter, typename EndIter>
	struct CallFunctionWighSigParamsIMPL
	{
		template<typename F, typename...Args>
		static void apply(This_t& manager, Entity<This_t>* ID, F&& func, Args&&...args)
		{
			using NextIter = typename boost::mpl::next<CurrentIter>::type;
			using ComponentType = typename boost::mpl::deref<CurrentIter>::type;
			using ManagerType = typename This_t::template GetManagerFromComponent_t<ComponentType>;

			CallFunctionWighSigParamsIMPL
				<
				NextIter
				, EndIter
				>::apply
				(
					manager
					, ID
					, std::forward<F>(func)
					, std::forward<Args>(args)...
					, manager.template getRefToManager<ManagerType>().template getComponent<ComponentType>(ID)
					);
		}
	};
	template<typename EndIter>
	struct CallFunctionWighSigParamsIMPL<EndIter, EndIter>
	{
		template<typename F, typename...Args>
		static void apply(This_t& manager, Entity<This_t>* ID, F&& func, Args&&...args)
		{
			std::forward<F>(func)(std::forward<Args>(args)...);
		}
	};
	template<typename A, typename B> friend struct CallFunctionWighSigParamsIMPL;

	template<typename Component>
	std::vector<Component>& getComponentStorage()
	{
		static_assert(isStorageComponent<Component>(), "Must be a storage component");

		using Manager_t = GetManagerFromComponent_t<Component>;

		const constexpr size_t ID = Manager_t::template getMyStorageComponentID<Component>();

		return std::get<ID>(getRefToManager<Manager_t>().storageComponentStorage);

	}


	template<typename Component>
	std::vector<Entity<This_t>*>& getComponentEntityStorage()
	{
		static_assert(isComponent<Component>(), "Must be a component");

		using Manager_t = GetManagerFromComponent_t<Component>;

		const constexpr size_t ID = Manager_t::template getMyComponentID<Component>();

		return std::get<ID>(getRefToManager<Manager_t>().componentEntityStorage);
	}

public:

	// CALLING FUNCTIONS ON ENTITIES
	template<typename SignatureToRun, typename F>
	void callFunctionWithSigParams(Entity<This_t>* ent, F&& func)
	{
		CallFunctionWighSigParamsIMPL
			<
			typename boost::mpl::begin<SignatureToRun>::type
			, typename boost::mpl::end<SignatureToRun>::type
			>::apply(*this, ent, std::forward<F>(func));
	}

	template<typename SignatureToRun, typename F>
	void runAllMatching(F&& functor)
	{
		using Manager_t = FindMostBaseManagerForSignature_t<SignatureToRun>;

		getRefToManager<Manager_t>().template runAllMatchingIMPL<SignatureToRun>(std::forward<F>(functor));
	}

	template<typename SignatureToRun, typename F>
	void runAllMatchingIMPL(F&& functor)
	{
       // TODO: implement
	}



	void update()
	{
		// increment the tick number so the updates are called.
		++tickNumber;

		updateManager(*this);

		for (auto&& child : children)
		{
			child.first.update(*child.second, *this);
		}
	}

	void beginPlay()
	{
		beginPlayManager(*this);

		for (auto&& child : children)
		{
			child.first.beginPlay(*child.second, *this);
		}
	}

public:// TODO:

	ManagerData<This_t> myManagerData;

	std::shared_ptr<size_t> nextIndex;

	// storage for the actual components
	template<typename... Args>
	using StorageComponentStorage_t = std::tuple<std::vector<Args>...>;
	ExpandSequenceToVaraidic_t<MyStorageComponents, StorageComponentStorage_t> storageComponentStorage;

	std::array<std::vector<Entity<This_t>*>, (This_t::getNumMyComponents())> componentEntityStorage;


	using BasePtrStorage_t =
		ExpandSequenceToVaraidic_t
		<
			AllManagersButThis // we dont' want to store the pointers for This_t: it is just this!
			, TupleOfPtrs
		>;
	using MyBasePtrStorage_t = ExpandSequenceToVaraidic_t<MyBases, TupleOfPtrs>;
	BasePtrStorage_t basePtrStorage;


	struct FunctionPointerStorage
	{

		using Update_t =
			void(*)(ManagerBase&, This_t&);

		using BeginPlay_t =
			void(*)(ManagerBase&, This_t&);

		Update_t update;
		BeginPlay_t beginPlay;
	};
	bool hasBegunPlay = false;
	bool hasBeenCleandUp = false;

	size_t tickNumber = 0;

	std::vector
		<
		std::pair
			<
				FunctionPointerStorage
				, ManagerBase*
			>
		> children;

public:

	static This_t* factory(const MyBasePtrStorage_t& bases = MyBasePtrStorage_t{})
	{

		auto ret = new This_t{ bases };
		initManager<This_t>(*ret);

		boost::fusion::for_each(bases,
			[ret](auto elem)
		{
			assert(elem);
			using BaseType = std::decay_t<decltype(*elem)>;
			static_assert(This_t::template isManager<BaseType>(), "Error, not a manager");

			typename BaseType::FunctionPointerStorage::Update_t updatePtr =
				&detail::Update_t<This_t, BaseType>::update;

			typename BaseType::FunctionPointerStorage::BeginPlay_t beginPlayPtr =
				&detail::BeginPlay_t<This_t, BaseType>::beginPlay;

			elem->children.push_back(
				std::make_pair(typename BaseType::FunctionPointerStorage{ updatePtr, beginPlayPtr }, ret)
				);

			ret->nextIndex = elem->nextIndex;
		});

		if (!ret->nextIndex)
		{
			ret->nextIndex = std::make_shared<size_t>();
		}

		return ret;
	}


	ManagerData<This_t>& getManagerData()
	{
		return myManagerData;
	}

private:

	Manager(const MyBasePtrStorage_t& bases)
	{
		auto thisptr = this; // funky workaround
		tuple_for_each_with_index(bases, [thisptr](auto& ptr, auto)
		{
			using BaseType = std::decay_t<decltype(*ptr)>;
			static_assert(This_t::template isManager<BaseType>(), "Must be a manager");


			constexpr size_t managerID = This_t::template getManagerID<BaseType>();

			std::get<managerID>(thisptr->basePtrStorage) = ptr;

			tuple_for_each_with_index(ptr->basePtrStorage, [thisptr](auto ptrIndBase, auto)
			{
				using IndBaseType = std::decay_t<decltype(*ptrIndBase)>;
				static_assert(This_t::template isManager<IndBaseType>(), "Must be a manager");


				constexpr size_t managerID = This_t::template getManagerID<IndBaseType>();

				std::get<managerID>(thisptr->basePtrStorage) = ptrIndBase;
			});

		});
	}

public:

	~Manager()
	{
		exitManager<This_t>(*this);
	}
};
