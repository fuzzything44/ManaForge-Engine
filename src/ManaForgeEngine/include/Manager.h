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


#include "Manager/Callbacks.h"
#include "Manager/MiscMetafunctions.h"
#include "Manager/Entity.h"


#undef max

template<typename... T>
constexpr auto make_type_tuple = boost::hana::make_tuple(boost::hana::type_c<T>...);

namespace detail 
{
namespace lambdas
{
	
auto myStorageComponents_LAM = [](auto tuple, auto newElement)
	{
		return boost::hana::if_(boost::hana::traits::is_empty(newElement), tuple, boost::hana::append(tuple, newElement));
	};
	
auto myTagComponents_LAM = [](auto tuple, auto newElement)
	{
		return boost::hana::if_(boost::hana::traits::is_empty(newElement), boost::hana::append(tuple, newElement), tuple);
	};
	
auto allStorageComponents_LAM = [](auto tuple, auto newElement)
	{
		return boost::hana::if_(boost::hana::traits::is_empty(newElement), tuple, boost::hana::append(tuple, newElement));
	};
	
auto allTagComponents_LAM = [](auto tuple, auto newElement)
	{
		return boost::hana::if_(boost::hana::traits::is_empty(newElement), boost::hana::append(tuple, newElement), tuple);
	};
auto removeTypeAddVec = [](auto arg)
	{
		return std::vector<typename decltype(arg)::type>{};
	};
auto removeTypeAddPtr = [](auto arg)
	{
		return (typename decltype(arg)::type*){};
	};
auto getAllManagers = [](auto arg)
	{
		return decltype(arg)::type::allManagers;
	};
auto getAllComponents = [](auto arg)
	{
		return decltype(arg)::type::allComponents;
	};
}
}


struct ManagerBase{};

template <typename Components_, typename Bases_ = boost::hana::tuple<> >
struct Manager : ManagerBase
{
	static_assert(decltype(is_tuple<Components_>())::value, "Components_ must be a boost::hana::tuple");
	static_assert(decltype(is_tuple<Bases_>())::value, "Bases_ must be a boost::hana::tuple");
	
	// GLOBAL TYPEDEFS
	static constexpr auto myComponents = boost::hana::make<Components_>();
	static constexpr auto myBases = boost::hana::make<Bases_>();
	using This_t = Manager<Components_, Bases_>;
	
	static constexpr auto allManagers = decltype(boost::hana::append(remove_dups(boost::hana::concat(boost::hana::fold(
		boost::hana::transform(myBases, detail::lambdas::getAllManagers), boost::hana::make_tuple(), boost::hana::concat), myBases)) , boost::hana::type_c<This_t>)){};

	static constexpr auto allComponents = decltype(remove_dups(boost::hana::concat(boost::hana::fold(boost::hana::transform(myBases, detail::lambdas::getAllComponents), 
		boost::hana::make_tuple(), boost::hana::concat), myComponents))){}; 

	static constexpr auto myStorageComponents = decltype(boost::hana::fold(myComponents, boost::hana::make_tuple(), detail::lambdas::myStorageComponents_LAM)){};
	static constexpr auto myTagComponents = decltype(boost::hana::fold(myComponents, boost::hana::make_tuple(), detail::lambdas::myTagComponents_LAM)){};
	static constexpr auto allStorageComponents = decltype(boost::hana::fold(allComponents, boost::hana::make_tuple(), detail::lambdas::allStorageComponents_LAM)){};
	static constexpr auto allTagComponents = decltype(boost::hana::fold(allComponents, boost::hana::make_tuple(), detail::lambdas::allTagComponents_LAM)){};

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
	static constexpr auto getMyStorageComponentID(T component)
	{
		BOOST_HANA_CONSTANT_CHECK(isStorageComponent(component));
		
		return get_index_of_first_matching(myStorageComponents, component);
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
	
	template<typename T>
	static constexpr auto isManager(T toTest)
	{
	//	static_assert(std::is_base_of<ManagerBase, typename decltype(toTest)::type>::value, "Error, needs to be a manager");
		
		return boost::hana::contains(allManagers, toTest);
	}
	template <typename T> 
	static constexpr auto getManagerID(T manager)
	{
		BOOST_HANA_CONSTANT_CHECK(isManager(manager));  
		
		return get_index_of_first_matching(allManagers, manager);
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
		return boost::hana::all_of(signature, [](auto type){ return isComponent(type); });
	}

	template<typename T>
	static constexpr auto getManagerFromComponent(T component)
	{
		BOOST_HANA_CONSTANT_CHECK(isComponent(component));
		
		return boost::hana::fold(allManagers, boost::hana::type_c<boost::hana::none_t>, [component](auto last, auto toTest)
			{
				return boost::hana::if_(decltype(toTest)::type::isMyComponent(component), toTest, last);
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
	
	template<typename T>
	static constexpr auto findDirectBaseManagerForSignature(T signature)
	{		
		
#ifdef IN_KDEVELOP_PARSER
	{ // TODO: update KDev... This is to work around a bug in Kdevelop's parser. For some reason it really wants a { here.
#endif
		
		return boost::hana::fold(myBases, boost::hana::type_c<This_t>, [&signature](auto toTest, auto currentRet)
			{
				return boost::hana::if_(decltype(toTest)::type::isSignature(signature), toTest, currentRet);
			}
		);
	}
	
	template<typename T>
	static constexpr auto findMostBaseManagerForSignature(T signature)
	{
		using namespace boost::hana::literals;
		auto ret = boost::hana::while_([](auto pair){ return pair[0_c] != pair[1_c]; }, 
			boost::hana::make_tuple(boost::hana::type_c<This_t>, findDirectBaseManagerForSignature(signature)), [&signature](auto tup)
			{
				return boost::hana::make_tuple(tup[1_c], decltype(tup[0_c])::type::findDirectBaseManagerForSignature(signature)); 
			}
		);
		
		BOOST_HANA_CONSTANT_CHECK(isManager(ret[0_c]));
		return ret[0_c];
		
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
		//TODO: implement
		
		return new Entity<This_t>;
		
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
		// TODO: implement
	}

	template<typename T>
	auto getStorageComponent(T component, Entity<This_t>* handle) -> typename decltype(component)::type&
	{
		BOOST_HANA_CONSTANT_CHECK(isStorageComponent(component));
		
		constexpr auto managerForComponent = decltype(getManagerFromComponent(component)){};

		constexpr auto staticID = decltype(managerForComponent)::type::template getMyStorageComponentID(component);
		
		auto* ent = getEntityPtr(managerForComponent, handle);

		size_t componentID = ent->components[staticID];

		return getComponentStorage(component)[componentID];

		
	}

	template<typename T>
	bool hasComponent(T component, Entity<This_t>* entity)
	{
		BOOST_HANA_CONSTANT_CHECK(isComponent(component));
		
		constexpr auto managerForComponent = decltype(getManagerFromComponent(component)){};

		auto ent = getEntityPtr(managerForComponent, entity);

		return ent->signature[decltype(decltype(managerForComponent)::type::template getComponentID(component))::value];
	}
	

	
	template<typename T>
	static auto getEntityPtr(T managerToGet, Entity<This_t>* ent) -> Entity<typename decltype(managerToGet)::type>*
	{
		BOOST_HANA_CONSTANT_CHECK(isManager(managerToGet));
		
		return ent->bases[getManagerID(managerToGet)];
	}
	
	template<typename T>
	auto getRefToManager(T manager) -> typename decltype(manager)::type&
	{
		BOOST_HANA_CONSTANT_CHECK(isManager(manager));
		
		return *basePtrStorage[getManagerID(manager)];
	}
	
	
	template<typename T>
	auto getComponentStorage(T component) -> std::vector<typename decltype(component)::type>&
	{
		BOOST_HANA_CONSTANT_CHECK(isStorageComponent(component));

		constexpr auto manager = decltype(getManagerFromComponent(component)){};

		const constexpr auto ID = decltype(manager)::type::template getMyStorageComponentID(component);

		return getRefToManager(manager).storageComponentStorage[ID];

	}


	template<typename T>
	std::vector<Entity<This_t>*>& getComponentEntityStorage(T component)
	{
		BOOST_HANA_CONSTANT_CHECK(isComponent(component));
		
		static constexpr auto manager = getManagerFromComponent(component);
		
		const constexpr auto ID = decltype(manager)::type::template getMyComponentID(component);
		
		return getRefToManager(manager).componentEntityStorage[ID];
	}

public:

	// CALLING FUNCTIONS ON ENTITIES
	template<typename T, typename F>
	void callFunctionWithSigParams(Entity<This_t>* ent, T signature, F&& func)
	{
		//TODO: implement
		
		
	}

	template<typename T, typename F>
	void runAllMatching(T signature, F&& functor)
	{
		BOOST_HANA_CONSTANT_CHECK(isSignature(signature));
		
		static constexpr auto manager = decltype(findMostBaseManagerForSignature(signature)){};

		getRefToManager(manager).runAllMatchingIMPL(signature, std::forward<F>(functor));
	}

	template<typename T, typename F>
	void runAllMatchingIMPL(T signature, F&& functor)
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

	ManagerData<This_t> myManagerData;

	// storage for the actual components
	decltype(boost::hana::transform(myStorageComponents, detail::lambdas::removeTypeAddVec)) storageComponentStorage;

	std::array<std::vector<Entity<This_t>*>, This_t::numMyComponents> componentEntityStorage;


	

	decltype(boost::hana::transform(allManagers, detail::lambdas::removeTypeAddPtr)) basePtrStorage;


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


	ManagerData<This_t>& getManagerData()
	{
		return myManagerData;
	}


	Manager(const decltype(boost::hana::transform(myBases, detail::lambdas::removeTypeAddPtr))& bases = decltype(bases){})
	{
		using namespace boost::hana::literals;
		
		assert(boost::hana::all_of(bases, [](auto ptr){ return ptr != 0; }));
		
		auto tempBases = boost::hana::remove_at(basePtrStorage, boost::hana::size(basePtrStorage) - boost::hana::size_c<1>);
		
		boost::hana::for_each(tempBases, [&bases](auto& baseToSet)
			{
				
				auto constexpr managerType = boost::hana::type_c<std::remove_pointer_t<std::decay_t<decltype(baseToSet)>>>;
				BOOST_HANA_CONSTANT_CHECK(isManager(managerType));
				
				auto hasBase = [&managerType](auto typeToCheck)
				{
					return decltype(typeToCheck)::type::isManager(managerType);
				};
				
				constexpr auto directBaseThatHasPtr = boost::hana::find_if(myBases, hasBase);
				BOOST_HANA_CONSTANT_CHECK(boost::hana::is_just(directBaseThatHasPtr));
				
				baseToSet = &bases[getBaseID(*directBaseThatHasPtr)]->getRefToManager(managerType);
				return directBaseThatHasPtr;
			});
		
		
		basePtrStorage = boost::hana::append(tempBases, this);
		
		initManager<This_t>(*this);
		
		
	}

	~Manager()
	{
		exitManager<This_t>(*this);
	}
};
