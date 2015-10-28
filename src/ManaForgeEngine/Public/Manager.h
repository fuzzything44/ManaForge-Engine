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

#include <boost/serialization/strong_typedef.hpp>

#include <boost/fusion/adapted/std_tuple.hpp>
#include <boost/fusion/algorithm/iteration/for_each.hpp>


#include <vector>
#include <utility>
#include <bitset>
#include <tuple>
#include <memory>
#include <type_traits>
#include <cassert>

#include "MPLHelp.h"
#include "Entity.h"

struct ManagerBase : std::enable_shared_from_this<ManagerBase>
{

};

template<typename Manager>
void updateManager(Manager& man){}

template<typename Manager>
void initManager(Manager& man){}

template<typename Manager>
void exitManager(Manager& man){}

template<typename Manager>
struct ManagerData {};

namespace detail
{
	template<typename ThisType, typename ManagerToGet>
	struct GetRefToManager
	{
		static ManagerToGet& apply(ThisType& data)
		{
			static_assert(ThisType::template isManager<ManagerToGet>(), "ManagerToGet must be a manager");
			static_assert(ThisType::template isManager<ThisType>(), "ManagerToGet must be a manager");

			constexpr auto managerID = ThisType::template getManagerID<ManagerToGet>();

			return *std::get<managerID>(data.basePtrStorage);
		}
	};

	template<typename ThisType>
	struct GetRefToManager<ThisType, ThisType>
	{
		static ThisType& apply(ThisType& data)
		{
			static_assert(ThisType::template isManager<ThisType>(), "ManagerToGet must be a manager");

			return data;
		}
	};

	template<typename BaseManagerType, typename ThisManager>
	void getAllMatching(ManagerBase& thisType, std::vector<size_t>& append, const typename BaseManagerType::RuntimeSignature_t& toTest)
	{
		auto& thisTypeDerived = static_cast<ThisManager&>(thisType);

		auto&& sigToTest = thisTypeDerived.template baseRuntimeSignatureToThisRuntimeSignature<BaseManagerType>(toTest);
		for (auto&& elem : thisTypeDerived.getEntityStorage())
		{
			if (elem.matchesSignature(sigToTest)) append.push_back(elem.GUID);
		}
		for (auto&& elem : thisTypeDerived.children)
		{
			elem.first.getAllMatching(*elem.second, append, sigToTest);
		}
	}

	template<typename ThisManager, typename BaseManager>
	struct Update_t
	{
		static void update(ManagerBase& thisManager, BaseManager& baseManager)
		{
			auto&& castedManager = static_cast<ThisManager&>(thisManager);

			updateManager(castedManager);

			for (auto&& child : castedManager.children)
			{
				child.first.update(*child.second, castedManager);
			}
		}
	};

}


template<typename BaseManager>
struct EntityHandle
{
	friend BaseManager;
private:
	explicit EntityHandle(size_t handle, size_t entityID_) : GUID{ handle }, entityID{ entityID_ } {}
public:
	const size_t GUID;
	const size_t entityID;
};



template <typename Components_, typename Tags_, typename Bases_ = boost::mpl::vector0<> >
struct Manager : ManagerBase
{
	static_assert(boost::mpl::is_sequence<Components_>::value, "Components must be a sequence");

	// GLOBAL TYPEDEFS
	using MyComponents = Components_;
	using MyTags = Tags_;
	using MyBases = Bases_;
	using ThisType = Manager<MyComponents, MyTags, MyBases>;
	using EntityType = Entity<ThisType>;
	using HandleType = EntityHandle<ThisType>;


private:
	template<typename SequenceOfManagers, bool needsExit = false>
	struct FindManagers
	{
		static_assert(boost::mpl::is_sequence<SequenceOfManagers>::value, "SequenceOfManagers must be a sequence");

		template<typename... Args>
		using FindUpperManagers =
			CatSequences_t<typename Args::MyBases...>;

		using ExpandedManagers =
			Unduplicate_t
			<
			CatSequences_t
			<
			SequenceOfManagers,
			ExpandSequenceToVaraidic_t
			<
			SequenceOfManagers
			, FindUpperManagers
			>
			>
			>
			;

		static constexpr const bool needsExitNext =
			boost::mpl::equal_to
			<
			typename boost::mpl::size<SequenceOfManagers>::type
			, typename boost::mpl::size<ExpandedManagers>::type
			>::type::value
			;

		using type =
			typename FindManagers
			<
			ExpandedManagers
			, needsExitNext
			>::type
			;
	};
	template<typename SequenceOfManagers>
	struct FindManagers<SequenceOfManagers, true>
	{
		using type = SequenceOfManagers;
		static_assert(boost::mpl::is_sequence<type>::value, "SequenceOfManagers needs to be a sequence.");
	};

public:
	using AllManagers = typename FindManagers<boost::mpl::vector<ThisType>>::type;
	static_assert(boost::mpl::is_sequence<AllManagers>::value, "AllManagers needs to be a sequence.");

private:
	template<typename... Args>
	using CatComponents_t = CatSequences_t<typename Args::MyComponents...>;
	template<typename... Args>
	using CatTags = CatSequences_t<typename Args::MyTags...>;
public:
	using AllComponents = ExpandSequenceToVaraidic_t<AllManagers, CatComponents_t>;
	using AllTags = ExpandSequenceToVaraidic_t<AllManagers, CatTags>;

	// CONSTEXPR TESTS
	template<size_t at> using ComponentByID =
		typename boost::mpl::at_c<MyComponents, at>::type;

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
	static const constexpr size_t numMyComponents = getNumMyComponents(); // this is a workaround. Turns out you can't use varaidcs here!
	template <typename Comp> static constexpr size_t getComponentID()
	{
		static_assert(isMyComponent<Comp>(), "Must be my component");

		return boost::mpl::distance<
			typename boost::mpl::begin<MyComponents>::type
			, typename boost::mpl::find<MyComponents, Comp>::type
		>::type::value;
	}


	static constexpr size_t getNumTags()
	{
		return boost::mpl::size<AllTags>::value;
	}
	static constexpr size_t getNumMyTags()
	{
		return boost::mpl::size<MyTags>::value;
	}
	template<typename Test> static constexpr bool isTag()
	{
		return boost::mpl::contains<AllTags, Test>::value;
	}
	template<typename Test> static constexpr bool isMyTag()
	{
		return boost::mpl::contains<MyTags, Test>::value;
	}
	static const constexpr size_t numMyTags = getNumMyTags(); // this is a workaround. Turns out you can't use varaidcs here!
	template <typename Tag> static constexpr size_t getTagID()
	{
		static_assert(isMyTag<Tag>(), "Must be my tag");

		return getNumComponents() + boost::mpl::distance<
			typename boost::mpl::begin<MyTags>::type
			, typename boost::mpl::find<MyTags, Tag>::type
		>::type::value;
	}

	static const constexpr size_t numComponentsAndTags = numMyComponents + numMyTags;

private:
	template<typename ComponentOrTag, bool = isTag<ComponentOrTag>()>
	struct GetTagOrComponentIMPL
	{
		static const constexpr size_t value = getTagID<Tag>();
	};
	template<typename Component>
	struct GetTagOrComponentIMPL<Component, false>
	{
		static const constexpr size_t value = getComponentID<Component>();
	};
public:

	template<typename ComponentOrTag>
	static constexpr size_t getTagOrComponentID()
	{
		return GetTagOrComponentIMPL<ComponentOrTag>::value;
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
	template<typename Test> static constexpr bool isManager()
	{
		return boost::mpl::contains<AllManagers, Test>::value;
	}
	template<size_t ID>
	using ManagerFromID = typename boost::mpl::at_c<AllManagers, ID>::type;

private:
	template<typename TypeToCheck>
	struct IsSignatureIMPL
	{
		using type = std::conditional_t
			<
			isComponent<TypeToCheck>() || isTag<TypeToCheck>()
			, std::true_type
			, std::false_type
			>
			;
	};
public:

	template<typename SignatureToCheck>
	static constexpr bool isSignature()
	{
		static_assert(boost::mpl::is_sequence<SignatureToCheck>::value, "Must be a sequence");

		using Transformed = typename boost::mpl::transform<SignatureToCheck, IsSignatureIMPL<boost::mpl::placeholders::_1>>::type;

		return
			std::is_same
			<
				typename boost::mpl::find
				<
					Transformed
					, std::false_type
				>::type
				, typename boost::mpl::end<Transformed>::type
			>::value;

	}

private:


	template<typename ComponentToCheckFor, typename CurrentIter, typename EndIter,
		typename = std::conditional_t
		<
		boost::mpl::contains
		<
		typename boost::mpl::deref<CurrentIter>::type::MyComponents
		, ComponentToCheckFor
		>::value
		, std::true_type
		, std::false_type
		>
	>
	struct GetManagerFromComponent
	{
		static_assert(isComponent<ComponentToCheckFor>(), "needs to be a component");

		using type =
			typename GetManagerFromComponent
			<
			ComponentToCheckFor
			, typename boost::mpl::next<CurrentIter>::type
			, EndIter
			>::type
			;
	};

	template<typename ComponentToCheckFor, typename CurrentIter, typename EndIter>
	struct GetManagerFromComponent<ComponentToCheckFor, CurrentIter, EndIter, std::true_type>
	{
		using type = typename boost::mpl::deref<CurrentIter>::type;
		static_assert(isManager<type>(), "Internal Error");
	};

	template<typename ComponentToCheckFor, typename EndIter>
	struct GetManagerFromComponent<ComponentToCheckFor, EndIter, EndIter, std::false_type>
	{
		//		static_assert(false, "ERROR, COULD NOT FIND COMPONENT IN ANY MANAGERS");
	};

	template<typename TagToCheckFor, typename CurrentIter, typename EndIter,
		typename = std::conditional_t
		<
		boost::mpl::contains
		<
		typename boost::mpl::deref<CurrentIter>::type::MyTags
		, TagToCheckFor
		>::value
		, std::true_type
		, std::false_type
		>
	>
	struct GetManagerFromTag
	{
		static_assert(isTag<TagToCheckFor>(), "needs to be a tag");

		using type =
			typename GetManagerFromTag
			<
			TagToCheckFor
			, typename boost::mpl::next<CurrentIter>::type
			, EndIter
			>::type
			;
	};

	template<typename TagToCheckFor, typename CurrentIter, typename EndIter>
	struct GetManagerFromTag<TagToCheckFor, CurrentIter, EndIter, std::true_type>
	{
		using type = typename boost::mpl::deref<CurrentIter>::type;
		static_assert(isManager<type>(), "Internal Error");
	};

	template<typename TagToCheckFor, typename EndIter>
	struct GetManagerFromTag<TagToCheckFor, EndIter, EndIter, std::false_type>
	{
		//		static_assert(false, "ERROR, COULD NOT FIND COMPONENT IN ANY MANAGERS");
	};


public:

	template<typename Component>
	using GetManagerFromComponent_t =
		typename GetManagerFromComponent
		<
		Component
		, typename boost::mpl::begin<AllManagers>::type
		, typename boost::mpl::end<AllManagers>::type
		>::type
		;
	template<typename Tag>
	using GetManagerFromTag_t =
		typename GetManagerFromTag
		<
		Tag
		, typename boost::mpl::begin<AllManagers>::type
		, typename boost::mpl::end<AllManagers>::type
		>::type
		;

private:

	template<typename ComponentOrTag, bool = isTag<ComponentOrTag>()>
	struct GetManagerFromComponentOrTag
	{
		using type = GetManagerFromTag_t<ComponentOrTag>;
	};
	template<typename Component>
	struct GetManagerFromComponentOrTag<Component, false>
	{
		using type = GetManagerFromComponent_t<Component>;
	};
public:

	template<typename ComponentOrTag>
	using GetManagerFromComponentOrTag_t = typename GetManagerFromComponentOrTag<ComponentOrTag>::type;



public:
	template<typename... Managers>
	using TupleOfBitmaps = std::tuple<std::bitset<Managers::numComponentsAndTags>...>;
	// Bitset that each Sequence and Entity has. This is the type.
	using RuntimeSignature_t = ExpandSequenceToVaraidic_t<AllManagers, TupleOfBitmaps>;

	// FUNCTIONS FOR ENTITES - CREATING, DESTROYING, COMPONENT HANDLING
	HandleType createEntity()
	{
		reallocateIfNecessary();

		auto myIndex = (*nextIndex)++;

		getEntityStorage().emplace_back(myIndex);

		return HandleType{ myIndex, getEntityStorage().size() - 1 };
	}

	template<typename Component>
	std::vector<Component>& getComponentStorage()
	{
		static_assert(isComponent<Component>(), "Component must be a component");

		using Manager_Type = GetManagerFromComponent_t<Component>;

		constexpr auto ID = Manager_Type::template getComponentID<Component>();

		return std::get<ID>(getRefToManager<Manager_Type>().componentStorage);
	}
	template<typename Component, typename... Args>
	Component& addComponent(HandleType handle, Args... args)
	{
		static_assert(isComponent<Component>(), "Component must be a component");

		// get constants for convience
		using ManagerForComponent = GetManagerFromComponent_t<Component>;
		constexpr size_t managerID = getManagerID<ManagerForComponent>();

		constexpr size_t componentID = ManagerForComponent::template getComponentID<Component>();

		// get the entity
		EntityType& entity = getEntityStorage()[handle.entityID];

		// check if this entity already has this component
		bool alreadyHasComponent = std::get<managerID>(entity.components)[componentID] = true;

		// if the Entity already has the component, make sure to destruct the current one.
		if (alreadyHasComponent)
		{
			getComponentStorage<Component>()[handle.GUID].~Component();
		}

		// set the component with the arguments passed
		getComponentStorage<Component>()[handle.GUID] = Component(std::forward<Args>(args)...);


		// set the bitset value to true so signature checking works
		std::get<managerID>(entity.components)[componentID] = true;

		return getComponentStorage<Component>()[handle.GUID];
	}
	template<typename Component>
	void removeComponent(HandleType handle)
	{
		static_assert(isComponent<Component>(), "Component must be a component");

		Entity<ThisType>& ent = getEntityStorage()[handle.entityID];

		using Manager_t = GetManagerFromComponent_t<Component>;
		const constexpr size_t managerID = getManagerID<Manager_t>();
		const constexpr size_t componentID = Manager_t::template getComponentID<Component>();

		std::get<managerID>(ent.components)[componentID] = false;
	}
	template<typename Component>
	Component& getComponent(HandleType handle)
	{
		static_assert(isComponent<Component>(), "Component must be a component");

		// get constants for convience
		using ManagerForComponent = GetManagerFromComponent_t<Component>;
		constexpr size_t managerID = getManagerID<ManagerForComponent>();
		constexpr size_t componentID = ManagerForComponent::template getComponentID<Component>();

		return getComponentStorage<Component>()[handle.GUID];
	}

	template<typename Tag>
	void addTag(HandleType handle)
	{
		static_assert(isTag<Tag>(), "Tag must be a Tag");
		// get constants for convience
		using ManagerForTag = GetManagerFromTag_t<Tag>;
		constexpr size_t managerID = getManagerID<ManagerForTag>();
		constexpr size_t tagID = ManagerForTag::template getTagID<Tag>();

		// get the entity
		EntityType& entity = getEntityStorage()[handle.entityID];

		std::get<managerID>(entity.components)[tagID] = true;
	}
	template<typename Tag>
	void removeTag(HandleType handle)
	{
		static_assert(isTag<Tag>(), "Tag must be a Tag");
		// get constants for convience
		using ManagerForTag = GetManagerFromTag_t<Tag>;
		constexpr size_t managerID = getManagerID<ManagerForTag>();
		constexpr size_t tagID = ManagerForTag::template getTagID<Tag>();

		// get the entity
		EntityType& entity = getEntityStorage()[handle.entityID];

		std::get<managerID>(entity.components)[tagID] = false;
	}
	template<typename Tag>
	bool hasTag(HandleType handle)
	{
		static_assert(isTag<Tag>(), "Tag must be a Tag");
		// get constants for convience
		using ManagerForTag = GetManagerFromTag_t<Tag>;
		constexpr size_t managerID = getManagerID<ManagerForTag>();
		constexpr size_t tagID = ManagerForTag::template getTagID<Tag>();

		// get the entity
		EntityType& entity = getEntityStorage()[handle.entityID];

		return std::get<managerID>(entity.components)[tagID];
	}

	std::vector<EntityType>& getEntityStorage()
	{
		return entityStorage;
	}

private:


public:

	// ALLOCATION/DATA STORAGE FUNCTION
	void reallocateIfNecessary()
	{
		if (*nextIndex >= getComponentStorage<typename boost::mpl::at_c<AllComponents, 0>::type>().size())
		{
			auto newSize = 2 * (*nextIndex + 3);

			// loop through this class AND the bases
			for_each_no_construct_ptr<typename boost::mpl::push_back<MyBases, ThisType>::type>(
				[ptr = this, newSize](auto man)
			{
				using Manager_t = std::remove_pointer_t<decltype(man)>;
				static_assert(ThisType::template isManager<Manager_t>(), "Internal error");

				auto&& refToManager = ptr->getRefToManager<Manager_t>();

				for_each_no_construct_ptr<typename Manager_t::MyComponents>(
					[ptr, newSize, man](auto comp)
				{
					using Manager_t = std::remove_pointer_t<decltype(man)>;
					static_assert(isManager<Manager_t>(), "Internal error");
					using Component_t = std::remove_pointer_t<decltype(comp)>;
					static_assert(isComponent<Component_t>(), "Internal error");

					const constexpr auto componentID = Manager_t::template getComponentID<Component_t>();

					auto&& refToManager = ptr->getRefToManager<Manager_t>();
					auto&& componentStorageTuple = refToManager.componentStorage;
					auto&& componentStorageVector = std::get<componentID>(componentStorageTuple);

					componentStorageVector.resize(newSize);

				});
			});


		}
	}

	template<typename ManagerToGet>
	ManagerToGet& getRefToManager()
	{
		static_assert(isManager<ManagerToGet>(), "ManagerToGet must be a manager");

		return detail::GetRefToManager<ThisType, ManagerToGet>::apply(*this);
	}

	template<typename ManagerToGet>
	std::vector<Entity<ManagerToGet>>& getEntityStorage()
	{
		static_assert(isManager<ManagerToGet>(), "");

		return getRefToManager<ManagerToGet>().getEntityStorage();
	}

private:


	template<typename BeginIter, typename EndIter, typename Sequence, bool shouldExit = false>
	struct CheckManagersForSequence
	{
		using Manager_t = typename boost::mpl::deref<BeginIter>::type;
		static_assert(isManager<Manager_t>(), "");

		static_assert(isManager<Manager_t>(), "");

		static constexpr const bool couldHaveSeq = Manager_t::template isSignature<Sequence>();

		using type =
			std::conditional_t
			<
			couldHaveSeq
			, Manager_t
			, typename CheckManagersForSequence
			<
			typename boost::mpl::next<BeginIter>::type
			, EndIter
			, Sequence
			, couldHaveSeq
			>::type
			>;
	};
	template<typename BeginIter, typename EndIter, typename Sequence>
	struct CheckManagersForSequence<BeginIter, EndIter, Sequence, true>
	{
		using type = typename boost::mpl::deref<BeginIter>::type;
	};
	template<typename EndIter, typename Sequence>
	struct CheckManagersForSequence<EndIter, EndIter, Sequence, false>
	{
		using type = boost::mpl::na;
	};

	template<typename Manager, typename SignatureToFind, bool running>
	struct FindMostBaseManagerForSignature
	{
		static_assert(boost::mpl::is_sequence<SignatureToFind>::value, "Signatures are sequences");
		static_assert(isSignature<SignatureToFind>(), "Must be a signature");

		using ManagerWithComponent =
			typename CheckManagersForSequence
			<
			typename boost::mpl::begin<typename Manager::MyBases>::type
			, typename boost::mpl::end<typename Manager::MyBases>::type
			, SignatureToFind
			>::type
			;

		static const constexpr bool runningNext = !std::is_same<ManagerWithComponent, boost::mpl::na>::value;

		using type =
			std::conditional_t
			<
			runningNext
			, typename FindMostBaseManagerForSignature
			<
			ManagerWithComponent
			, SignatureToFind
			, runningNext
			>::type
			, Manager
			>
			;
	};
	template<typename Manager, typename SequenceToFind>
	struct FindMostBaseManagerForSignature<Manager, SequenceToFind, false>
	{
		using type = Manager;
	};
public:
	template<typename SequenceToFind>
	using FindMostBaseManagerForSignature_t = typename FindMostBaseManagerForSignature<ThisType, SequenceToFind, true>::type;

	template<typename Signature>
	static RuntimeSignature_t generateRuntimeSignature()
	{
		static_assert(boost::mpl::is_sequence<Signature>::value, "");

		RuntimeSignature_t ret;

		for_each_no_construct_ptr<Signature>([&ret](auto a)
		{
			using ComponentOrTag_t = std::remove_pointer_t<decltype(a)>;
			static_assert(isComponent<ComponentOrTag_t>() || isTag<ComponentOrTag_t>(), "Must be a component");
			using Manager_t = GetManagerFromComponentOrTag_t<ComponentOrTag_t>;
			static_assert(isManager<Manager_t>(), "Must be a manager");

			constexpr const auto componentOrTagID = Manager_t::getTagOrComponentID<ComponentOrTag_t>();
			constexpr const auto managerID = getManagerID<Manager_t>();

			std::get<managerID>(ret)[componentOrTagID] = true;
		});

		return ret;
	}

private:
	template<typename...Args>
	using VoidStdFunctionArguments = std::function<void(Args...)>;
public:

	template<typename Signature>
	using SignatureToFunction = ExpandSequenceToVaraidic_t<Signature, VoidStdFunctionArguments>;

	template<typename BaseManager>
	RuntimeSignature_t baseRuntimeSignatureToThisRuntimeSignature(typename BaseManager::RuntimeSignature_t toConvert)
	{
		RuntimeSignature_t ret;

		tuple_for_each_with_index(toConvert, [&ret, &toConvert](auto elem, auto indexType)
		{
			constexpr const size_t index = decltype(indexType)::value;
			using ManagerForBitset = typename BaseManager::template ManagerFromID<index>;

			constexpr const size_t retIndex = ThisType::template getManagerID<ManagerForBitset>();

			std::get<retIndex>(ret) = std::get<index>(toConvert);

		});

		return ret;
	}

private:
	template<typename CurrentIter, typename EndIter, bool isTypeTag = isTag<typename boost::mpl::deref<CurrentIter>::type>()>
	struct CallFunctionWighSigParamsIMPL
	{

		template<typename F, typename...Args>
		static void apply(ThisType& manager, HandleType ID, F&& func, Args&&...args)
		{
			typename boost::mpl::deref<CurrentIter>::type nextTag;

			using NextIter = typename boost::mpl::next<CurrentIter>::type;

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
					, nextTag
					);
		}
	};
	template<typename CurrentIter, typename EndIter>
	struct CallFunctionWighSigParamsIMPL<CurrentIter, EndIter, false>
	{
		template<typename F, typename...Args>
		static void apply(ThisType& manager, HandleType ID, F&& func, Args&&...args)
		{
			using NextIter = typename boost::mpl::next<CurrentIter>::type;

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
					, manager.getComponent<typename boost::mpl::deref<CurrentIter>::type>(ID)
					);
		}
	};
	template<typename EndIter>
	struct CallFunctionWighSigParamsIMPL<EndIter, EndIter, false>
	{
		template<typename F, typename...Args>
		static void apply(ThisType& manager, HandleType ID, F&& func, Args&&...args)
		{
			std::forward<F>(func)(std::forward<Args>(args)...);
		}
	};
public:

	// CALLING FUNCTIONS ON ENTITIES
	template<typename SignatureToRun, typename F>
	void callFunctionWithSigParams(HandleType ID, F&& func)
	{
		CallFunctionWighSigParamsIMPL
			<
			typename boost::mpl::begin<SignatureToRun>::type
			, typename boost::mpl::end<SignatureToRun>::type
			>::apply(*this, ID, std::forward<F>(func));
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
		static_assert (std::is_same<FindMostBaseManagerForSignature_t<SignatureToRun>, ThisType>::value, "Please use runAllMatching instead");

		RuntimeSignature_t checkBitset = generateRuntimeSignature<SignatureToRun>();

		// get all of the entities that match the signature
		std::vector<size_t> vec;

		detail::getAllMatching<ThisType, ThisType>(*this, vec, checkBitset);

		for (auto&& elem : vec)
		{
			callFunctionWithSigParams<SignatureToRun>(HandleType{ elem, elem }, std::forward<F>(functor));
		}


	}


	void update()
	{
		updateManager(*this);

		for (auto&& child : children)
		{
			child.first.update(*child.second, *this);
		}
	}

public:// TODO:

	std::vector<EntityType> entityStorage;

	std::shared_ptr<size_t> nextIndex;

	template<typename... Args>
	using TupleOfVectors = std::tuple<std::vector<Args>...>;
	ExpandSequenceToVaraidic_t<MyComponents, TupleOfVectors> componentStorage;

	template<typename... Args>
	using TupleOfSharedPtrs = std::tuple<std::shared_ptr<Args>...>;
	using BasePtrStorage_t = ExpandSequenceToVaraidic_t<MyBases, TupleOfSharedPtrs>;
	BasePtrStorage_t basePtrStorage;


	struct FunctionPointerStorage
	{

		using GetAllMatching_t =
			void(*)
			(
				ManagerBase&
				, std::vector<size_t>&
				, const RuntimeSignature_t&
				);

		using Update_t =
			void(*)(ManagerBase&, ThisType&);

		GetAllMatching_t getAllMatching;
		Update_t update;
	};
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
	
	static std::shared_ptr<ThisType> factory(const BasePtrStorage_t& bases = BasePtrStorage_t{})
	{
		
		auto ret = std::shared_ptr<ThisType>(new ThisType{ bases });
		initManager<ThisType>(*ret);

		boost::fusion::for_each(bases,
			[ret](auto elem)
		{
			assert(elem);
			using BaseType = std::decay_t<decltype(*elem)>;
			static_assert(ThisType::template isManager<BaseType>(), "Error, not a manager");

			typename BaseType::FunctionPointerStorage::GetAllMatching_t getAllMatchingPtr =
				&detail::getAllMatching<BaseType, ThisType>;

			typename BaseType::FunctionPointerStorage::Update_t updatePtr = 
				&detail::Update_t<ThisType, BaseType>::update;

			elem->children.push_back(
				std::make_pair(typename BaseType::FunctionPointerStorage{ getAllMatchingPtr, updatePtr}, ret)
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

	ManagerData<ThisType> myManagerData;


	Manager(const BasePtrStorage_t& bases)
		: basePtrStorage(bases)
	{

	}

public:

	~Manager()
	{
		exitManager<ThisType>(*this);
	}
};
