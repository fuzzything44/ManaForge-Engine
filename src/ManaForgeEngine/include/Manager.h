#pragma once

#include "Engine.h"

#include <boost/mpl/size.hpp>
#include <boost/mpl/find.hpp>
#include <boost/mpl/for_each.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/begin.hpp>
#include <boost/mpl/at.hpp>
#include <boost/mpl/back_inserter.hpp>
#include <boost/mpl/contains.hpp>
#include <boost/mpl/fold.hpp>
#include <boost/mpl/joint_view.hpp>
#include <boost/mpl/equal_to.hpp>
#include <boost/mpl/erase.hpp>
#include <boost/mpl/copy.hpp>
#include <boost/mpl/set.hpp>
#include <boost/mpl/count.hpp>
#include <boost/mpl/transform.hpp>
#include <boost/mpl/pop_front.hpp>
#include <boost/mpl/find_if.hpp>
#include <boost/mpl/copy_if.hpp>
#include <boost/mpl/remove.hpp>

#include <call_from_tuple.h>

#include <boost/serialization/strong_typedef.hpp>

#include <boost/fusion/adapted/std_tuple.hpp>
#include <boost/fusion/algorithm/iteration/for_each.hpp>

#include <boost/dynamic_bitset.hpp>

#include <vector>
#include <utility>
#include <bitset>
#include <tuple>
#include <memory>
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

struct ManagerBase : std::enable_shared_from_this<ManagerBase>
{

};

template <typename Components_, typename Bases_ = boost::mpl::vector0<> >
struct Manager : ManagerBase
{
	static_assert(boost::mpl::is_sequence<Components_>::value, "Components must be a sequence");
	static_assert(boost::mpl::is_sequence<Bases_>::value, "Bases must be a sequence");
	
	// GLOBAL TYPEDEFS
	using MyComponents = Components_;
	using MyBases = Bases_;
	using ThisType = Manager<MyComponents, MyBases>;

	using AllManagers = typename detail::FindManagers<boost::mpl::vector<ThisType>>::type;
	static_assert(boost::mpl::is_sequence<AllManagers>::value, "AllManagers needs to be a sequence.");

	using AllManagersButThis = typename boost::mpl::remove<AllManagers, ThisType>::type;
	static_assert(boost::mpl::is_sequence<AllManagersButThis>::value, "AllManagers needs to be a sequence.");

private:
	template<typename... Args>
	using CatComponents_t = CatSequences_t<typename Args::AllComponents...>;
public:
	using AllComponents = CatSequences_t<MyComponents, Unduplicate_t<ExpandSequenceToVaraidic_t<AllManagersButThis, CatComponents_t>>>;

private:
	template<typename Component>
	struct IsSizeOne
	{
		static constexpr const bool value = sizeof(Component) == 1;
	};
	
	template<typename Component>
	struct IsSizeGreaterThanOne
	{
		static constexpr const bool value = sizeof(Component) > 1;
	};
public:
	using MyStorageComponents = typename boost::mpl::copy_if<MyComponents, IsSizeGreaterThanOne<boost::mpl::placeholders::_1>>::type;
	using MyTagComponents = typename boost::mpl::copy_if<MyComponents, IsSizeOne<boost::mpl::placeholders::_1>>::type;
	

	using AllStorageComponents = typename boost::mpl::copy_if<AllComponents, IsSizeGreaterThanOne<boost::mpl::placeholders::_1>>::type;
	using AllTagComponents = typename boost::mpl::copy_if<AllComponents, IsSizeOne<boost::mpl::placeholders::_1>>::type;
	
	// CONSTEXPR FUNCTIONS/TESTS


	static constexpr size_t getNumComponents()
	{
		return boost::mpl::size<AllComponents>::value;
	}
	static constexpr size_t getNumMyComponents()
	{
		return boost::mpl::size<MyComponents>::value;
	}
	template<typename Test> static constexpr bool isComponent()
	{
		return boost::mpl::contains<AllComponents, Test>::value;
	}
	template<typename Test> static constexpr bool isMyComponent()
	{
		return boost::mpl::contains<MyComponents, Test>::value;
	}
	template <typename Comp> static constexpr size_t getComponentID()
	{
		static_assert(isComponent<Comp>(), "Must be component");

		return boost::mpl::distance<
			typename boost::mpl::begin<AllComponents>::type
			, typename boost::mpl::find<AllComponents, Comp>::type
		>::type::value;
	}
	template <typename Comp> static constexpr size_t getMyComponentID()
	{
		static_assert(isMyComponent<Comp>(), "Must be component");

		return boost::mpl::distance<
			typename boost::mpl::begin<MyComponents>::type
			, typename boost::mpl::find<MyComponents, Comp>::type
		>::type::value;
	}
	
	static constexpr size_t getNumStorageComponents()
	{
		return boost::mpl::size<AllStorageComponents>::value;
	}
	static constexpr size_t getNumMyStorageComponents()
	{
		return boost::mpl::size<MyStorageComponents>::value;
	}
	template<typename Test> static constexpr bool isStorageComponent()
	{
		return boost::mpl::contains<AllStorageComponents, Test>::value;
	}
	template<typename Test> static constexpr bool isMyStorageComponent()
	{
		return boost::mpl::contains<MyStorageComponents, Test>::value;
	}
	template <typename Comp> static constexpr size_t getMyStorageComponentID()
	{
		static_assert(isMyStorageComponent<Comp>(), "Must be a storage component");

		return boost::mpl::distance<
			typename boost::mpl::begin<MyStorageComponents>::type
			, typename boost::mpl::find<MyStorageComponents, Comp>::type
		>::type::value;
	}

	static constexpr size_t getNumTagComponents()
	{
		return boost::mpl::size<AllTagComponents>::value;
	}
	static constexpr size_t getNumMyTagComponents()
	{
		return boost::mpl::size<MyTagComponents>::value;
	}
	template<typename Test> static constexpr bool isTagComponent()
	{
		return boost::mpl::contains<AllTagComponents, Test>::type::value;
	}
	template<typename Test> static constexpr bool isMyTagComponent()
	{
		return boost::mpl::contains<MyTagComponents, Test>::value;
	}
	template <typename Tag> static constexpr size_t getMyTagComponentID()
	{
		static_assert(isTagComponent<Tag>(), "Must be tag");

		return boost::mpl::distance<
			typename boost::mpl::begin<MyTagComponents>::type
			, typename boost::mpl::find<MyTagComponents, Tag>::type
		>::type::value;
	}
	static constexpr size_t getNumManagers()
	{
		return boost::mpl::size<AllManagers>::value;
	}
	template <typename Comp> static constexpr size_t getManagerID()
	{
		static_assert(isManager<Comp>(), "Must be a Manager");

		return boost::mpl::distance<
			typename boost::mpl::begin<AllManagers>::type,
			typename boost::mpl::find<AllManagers, Comp>::type
		>::type::value;
	}
	template<typename Mana> static constexpr size_t getManagerExceptThisID()
	{
		static_assert(isManager<Mana>(), "Must be a Manager");

		return boost::mpl::distance<
			typename boost::mpl::begin<AllManagersButThis>::type,
			typename boost::mpl::find<AllManagersButThis, Mana>::type
		>::type::value;
	}
	template<typename Test> static constexpr bool isManager()
	{
		return boost::mpl::contains<AllManagers, Test>::value;
	}
	template<size_t ID>
	using ManagerFromID = typename boost::mpl::at_c<AllManagers, ID>::type;

	template<size_t at> using ComponentByID =
		typename boost::mpl::at_c<AllComponents, at>::type;

private:
	// turns a signature into a vector of bool_ if it is a valid component
	template<typename... T>
	using isSignature_IMPL = boost::mpl::vector<boost::mpl::bool_<ThisType::template isComponent<T>()>...>;
public:
	template<typename SignatureToCheck>
	static constexpr bool isSignature()
	{
		static_assert(boost::mpl::is_sequence<SignatureToCheck>::value, "Must be a sequence");

		// make a new sequence -- each element is if (the element is a component)
		using Transformed = ExpandSequenceToVaraidic_t<SignatureToCheck, isSignature_IMPL>;
		
		// if there are no false types, then it is a signature
		return !boost::mpl::contains<Transformed, std::false_type>::type::value;
	}

	template<typename Component>
	using GetManagerFromComponent_t =
		typename detail::GetManagerFromComponent
		<
			ThisType
			, Component
			, typename boost::mpl::begin<AllManagers>::type
			, typename boost::mpl::end<AllManagers>::type
		>::type
		;

	template<typename Sequence>
	using IsolateStorageComponents_t = typename detail::IsolateStorageComponents<ThisType, Sequence>::type;

	template<typename Sequence>
	using IsolateTagComponents_t = typename detail::IsolateTagComponents<ThisType, Sequence>::type;

	template<typename... Types>
	using TupleOfConstRefs_t = std::tuple<Types...>;
	
	template<typename Signature>
	auto newEntity(const ExpandSequenceToVaraidic_t<Signature, TupleOfConstRefs_t>& components/*tuple of the components*/)
	{
		// TODO: impmenet
		
		return true;
	}

	template<typename...Args>
	using TupleOfVectorRefrences = std::tuple<std::vector<Args>&...>;


	// returns the elements created [first, last)
	template<typename Signature>
	std::pair<size_t, size_t> createEntityBatch(
		ExpandSequenceToVaraidic_t<IsolateStorageComponents_t<Signature>, TupleOfVectorRefrences> components,
		size_t numToConstruct)
	{
		// TODO: implement
	}
	void destroyEntity(EntityBase handle)
	{
		// TODO: reimplement
		
	}

	template<typename Component>
	Component& getComponent(EntityBase* handle)
	{
		// TODO: implement
	}

	template<typename Component>
	bool hasComponent(size_t handle)
	{
		//TODO: reimplement
	}

	template<typename Tag>
	bool hasTag(size_t handle)
	{
		// TODO: implement
	}

	template<typename ManagerToGet>
	ManagerToGet& getRefToManager()
	{
		static_assert(isManager<ManagerToGet>(), "ManagerToGet must be a manager");

		return detail::GetRefToManager<ThisType, ManagerToGet>::apply(*this);
	}

	template<typename SequenceToFind>
	using FindMostBaseManagerForSignature_t = 
		typename detail::FindMostBaseManagerForSignature<ThisType, SequenceToFind, true>::type;

	template<typename CurrentIter, typename EndIter>
	struct CallFunctionWighSigParamsIMPL
	{
		template<typename F, typename...Args>
		static void apply(ThisType& manager, EntityBase* ID, F&& func, Args&&...args)
		{
			using NextIter = typename boost::mpl::next<CurrentIter>::type;
			using ComponentType = typename boost::mpl::deref<CurrentIter>::type;
			using ManagerType = typename ThisType::template GetManagerFromComponent_t<ComponentType>;

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
		static void apply(ThisType& manager, EntityBase* ID, F&& func, Args&&...args)
		{
			std::forward<F>(func)(std::forward<Args>(args)...);
		}
	};
	template<typename A, typename B> friend struct CallFunctionWighSigParamsIMPL;

	template<typename Component>
	std::vector<Component>& getComponentStorage()
	{
		using ManagerType = GetManagerFromComponent_t<Component>;
		const constexpr size_t ID = ManagerType::template getMyComponentID<Component>();

		return std::get<ID>(getRefToManager<ManagerType>().componentStorage).second;
	}

	template<typename Component>
	auto& getComponentEntityStorage()
	{
		using ManagerType = GetManagerFromComponent_t<Component>;

		const constexpr size_t componentID = ManagerType::template getMyComponentID<Component>();

		return std::get<componentID>(getRefToManager<ManagerType>().componentStorage).first;
	}

public:

	// CALLING FUNCTIONS ON ENTITIES
	template<typename SignatureToRun, typename F>
	void callFunctionWithSigParams(EntityBase* ent, F&& func)
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
		// TODO: reimplement

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

	ManagerData<ThisType> myManagerData;

	std::shared_ptr<size_t> nextIndex;

	

	template<typename... Args>
	using TupleOfPtrs = std::tuple<Args*...>;
	using BasePtrStorage_t = 
		ExpandSequenceToVaraidic_t
		<
			AllManagersButThis // we dont' want to store the pointers for ThisType: it is just this!
			, TupleOfPtrs
		>; 
	using MyBasePtrStorage_t = ExpandSequenceToVaraidic_t<MyBases, TupleOfPtrs>;
	BasePtrStorage_t basePtrStorage;


	struct FunctionPointerStorage
	{

		using Update_t =
			void(*)(ManagerBase&, ThisType&);

		using BeginPlay_t =
			void(*)(ManagerBase&, ThisType&);

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
				, std::shared_ptr<ManagerBase>
			>
		> children;

public:

	std::shared_ptr<ThisType> getPtr() { return shared_from_this(); }
	
	static std::shared_ptr<ThisType> factory(const MyBasePtrStorage_t& bases = MyBasePtrStorage_t{})
	{
		
		auto ret = std::shared_ptr<ThisType>(new ThisType{ bases });
		initManager<ThisType>(*ret);

		boost::fusion::for_each(bases,
			[ret](auto elem)
		{
			assert(elem);
			using BaseType = std::decay_t<decltype(*elem)>;
			static_assert(ThisType::template isManager<BaseType>(), "Error, not a manager");

			typename BaseType::FunctionPointerStorage::Update_t updatePtr =
				&detail::Update_t<ThisType, BaseType>::update;

			typename BaseType::FunctionPointerStorage::BeginPlay_t beginPlayPtr =
				&detail::BeginPlay_t<ThisType, BaseType>::beginPlay;

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

	
	ManagerData<ThisType>& getManagerData()
	{
		return myManagerData;
	}

private:

	Manager(const MyBasePtrStorage_t& bases)
		:myManagerData{}
	{
		auto thisptr = this; // funky workaround
		tuple_for_each_with_index(bases, [thisptr](auto& ptr, auto)
		{
			using BaseType = std::decay_t<decltype(*ptr)>;
			static_assert(ThisType::template isManager<BaseType>(), "Must be a manager");


			constexpr size_t managerID = ThisType::template getManagerID<BaseType>();

			std::get<managerID>(thisptr->basePtrStorage) = ptr;

			tuple_for_each_with_index(ptr->basePtrStorage, [thisptr](auto ptrIndBase, auto)
			{
				using IndBaseType = std::decay_t<decltype(*ptrIndBase)>;
				static_assert(ThisType::template isManager<IndBaseType>(), "Must be a manager");


				constexpr size_t managerID = ThisType::template getManagerID<IndBaseType>();

				std::get<managerID>(thisptr->basePtrStorage) = ptrIndBase;
			});

		});
	}

public:

	~Manager()
	{
		exitManager<ThisType>(*this);
	}
};
