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
#include <boost/mpl/remove.hpp>

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

#include "Manager/Entity.h"
#include "Manager/Callbacks.h"
#include "Manager/GetRefToManager.h"
#include "Manager/EntityHandle.h"
#include "Manager/FindAllManagers.h"
#include "Manager/MiscMetafunctions.h"

#include "MappedVector.h"

struct ManagerBase : std::enable_shared_from_this<ManagerBase>
{

};

template <typename Components_, typename Tags_, typename Bases_ = boost::mpl::vector0<> >
struct Manager : ManagerBase
{
	static_assert(boost::mpl::is_sequence<Components_>::value, "Components must be a sequence");
	static_assert(boost::mpl::is_sequence<Tags_>::value, "Tags must be a sequence");
	static_assert(boost::mpl::is_sequence<Bases_>::value, "Bases must be a sequence");

	// GLOBAL TYPEDEFS
	using MyComponents = Components_;
	using MyTags = Tags_;
	using MyBases = Bases_;
	using ThisType = Manager<MyComponents, MyTags, MyBases>;
	using EntityType = Entity<ThisType>;
	using HandleType = EntityHandle<ThisType>;

	using AllManagers = typename detail::FindManagers<boost::mpl::vector<ThisType>>::type;
	static_assert(boost::mpl::is_sequence<AllManagers>::value, "AllManagers needs to be a sequence.");

	using AllManagersButThis = typename boost::mpl::remove<AllManagers, ThisType>::type;
	static_assert(boost::mpl::is_sequence<AllManagersButThis>::value, "AllManagers needs to be a sequence.");

private:
	template<typename... Args>
	using CatComponents_t = CatSequences_t<typename Args::MyComponents...>;
	template<typename... Args>
	using CatTags = CatSequences_t<typename Args::MyTags...>;
public:
	using AllComponents = ExpandSequenceToVaraidic_t<AllManagers, CatComponents_t>;
	using AllTags = ExpandSequenceToVaraidic_t<AllManagers, CatTags>;

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
	template <typename Tag> static constexpr size_t getTagID()
	{
		static_assert(isTag<Tag>(), "Must be tag");

		return getNumComponents() + boost::mpl::distance<
			typename boost::mpl::begin<AllTags>::type
			, typename boost::mpl::find<AllTags, Tag>::type
		>::type::value;
	}

	template<typename ComponentOrTag>
	static constexpr size_t getTagOrComponentID()
	{
		return detail::GetTagOrComponentID_IMPL<ThisType, ComponentOrTag>::value;
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

	template<size_t at> using TagByID =
		typename boost::mpl::at_c<AllTags, at - getNumComponents()>::type;

private:
	template<size_t at, bool isComponent = (at < getNumComponents())>
	struct ComponentOrTagByID_IMPL
	{
		using type = TagByID<at>;
	};
	template<size_t at>
	struct ComponentOrTagByID_IMPL<at, true>
	{
		using type = ComponentByID<at>;
	};
public:

	template<size_t at> using ComponentOrTagByID = typename ComponentOrTagByID_IMPL<at>::type;

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
	template<typename Tag>
	using GetManagerFromTag_t =
		typename detail::GetManagerFromTag
		<
			ThisType
			, Tag
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

private:
	template<typename Sequence>
	using RemoveTags_t = typename detail::RemoveTags<ThisType, Sequence>::type;
public:

public:
	// Bitset that each Signature and Entity has. This is the type. Just a bitset of the possible components and tags
	static constexpr const size_t numComponentsAndTags = getNumComponents() + getNumTags();
	using RuntimeSignature_t = std::bitset<numComponentsAndTags>;

	// FUNCTIONS FOR ENTITES - CREATING, DESTROYING, COMPONENT HANDLING
	HandleType createEntity()
	{
		auto myIndex = (*nextIndex)++;

		entityStorage.emplace(myIndex);

		return HandleType{ myIndex };
	}
	void destroyEntity(HandleType handle)
	{
		auto&& iterToEntity = entityStorage.begin(); std::advance(iterToEntity, handle.GUID);

		entityStorage.erase(iterToEntity);
	}

	template<typename Component, typename... Args>
	Component& addComponent(HandleType handle, Args&&... args)
	{
		static_assert(isComponent<Component>(), "Component must be a component");

		// get constants for convience
		using ManagerForComponent = GetManagerFromComponent_t<Component>;
		constexpr size_t managerID = getManagerID<ManagerForComponent>();

		constexpr size_t componentID = ManagerForComponent::template getComponentID<Component>();

		// get the entity
		EntityType& entity = entityStorage[handle.GUID];

		// set the component with the arguments passed
		getComponentStorage<Component>().emplace(handle.GUID, std::forward<Args>(args)...);


		// set the bitset value to true so signature checking works
		entity.components[componentID] = true;

		return getComponentStorage<Component>()[handle.GUID];
	}
	template<typename Component>
	void removeComponent(HandleType handle)
	{
		static_assert(isComponent<Component>(), "Component must be a component");

		Entity<ThisType>& ent = entityStorage[handle.GUID];

		using Manager_t = GetManagerFromComponent_t<Component>;
		const constexpr size_t managerID = getManagerID<Manager_t>();
		const constexpr size_t componentID = Manager_t::template getComponentID<Component>();

		// delete the component
		auto&& storage = getComponentStorage<Component>();
		storage.erase(storage.get_elem_at(handle.GUID));

		ent.components[componentID] = false;
	}
	template<typename Component>
	Component& getComponent(HandleType handle)
	{
		static_assert(isComponent<Component>(), "Component must be a component");

		// get constants for convience
		using ManagerForComponent = GetManagerFromComponent_t<Component>;
		constexpr size_t managerID = getManagerID<ManagerForComponent>();
		constexpr size_t componentID = ManagerForComponent::template getComponentID<Component>();

		// make sure that the entity actually has the component
		assert(entityStorage[handle.GUID].components[componentID]);

		return getComponentStorage<Component>()[handle.GUID];
	}
	template<typename Component>
	bool hasComponent(HandleType handle)
	{
		static_assert(isComponent<Component>(), "Component must be a Component");
		// get constants for convience
		using ManagerForComponent = GetManagerFromComponent_t<Component>;
		constexpr size_t managerID = getManagerID<ManagerForComponent>();
		constexpr size_t componentID = ManagerForComponent::template getComponentID<Component>();

		// get the entity
		EntityType& entity = entityStorage[handle.GUID];

		return entity.components[componentID];
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
		EntityType& entity = entityStorage[handle.GUID];

		entity.components[tagID] = true;
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
		EntityType& entity = entityStorage[handle.GUID];

		entity.components[tagID] = false;
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
		EntityType& entity = entityStorage[handle.GUID];

		return entity.components[tagID];
	}

	template<typename Component>
	MappedVector<Component>& getComponentStorage()
	{
		static_assert(isComponent<Component>(), "Component must be a component");

		using Manager_Type = GetManagerFromComponent_t<Component>;

		constexpr auto ID = Manager_Type::template getComponentID<Component>();

		return std::get<ID>(getRefToManager<Manager_Type>().componentStorage);
	}

	template<typename ManagerToGet>
	ManagerToGet& getRefToManager()
	{
		static_assert(isManager<ManagerToGet>(), "ManagerToGet must be a manager");

		return detail::GetRefToManager<ThisType, ManagerToGet>::apply(*this);
	}

	template<typename SequenceToFind>
	using FindMostBaseManagerForSignature_t = typename detail::FindMostBaseManagerForSignature<ThisType, SequenceToFind, true>::type;

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

			ret[componentOrTagID] = true;
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
	RuntimeSignature_t baseRuntimeSignatureToThisRuntimeSignature(const typename BaseManager::RuntimeSignature_t& toConvert)
	{
		RuntimeSignature_t ret;

		const constexpr size_t sizeOfToConvertBitset = getBitsetSize<decltype(toConvert)>();

		detail::BaseRuntimeSignatureToThisRuntimeSignature_IMPL<ThisType, BaseManager, 0, sizeOfToConvertBitset>::apply(ret, toConvert);

		return ret;
	}

private:
	template<typename CurrentIter, typename EndIter, bool isTypeTag = isTag<typename boost::mpl::deref<CurrentIter>::type>()>
	struct CallFunctionWighSigParamsIMPL
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
			callFunctionWithSigParams<SignatureToRun>(HandleType{ elem }, std::forward<F>(functor));
		}
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

	MappedVector<EntityType> entityStorage;

	std::shared_ptr<size_t> nextIndex;

	template<typename... Args>
	using TupleOfVectors = std::tuple<MappedVector<Args>...>;
	ExpandSequenceToVaraidic_t<MyComponents, TupleOfVectors> componentStorage;

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

		using GetAllMatching_t =
			void(*)
			(
				ManagerBase&
				, std::vector<size_t>&
				, const RuntimeSignature_t&
				);

		using Update_t =
			void(*)(ManagerBase&, ThisType&);

		using BeginPlay_t =
			void(*)(ManagerBase&, ThisType&);

		GetAllMatching_t getAllMatching;
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

			typename BaseType::FunctionPointerStorage::GetAllMatching_t getAllMatchingPtr =
				&detail::getAllMatching<BaseType, ThisType>;

			typename BaseType::FunctionPointerStorage::Update_t updatePtr =
				&detail::Update_t<ThisType, BaseType>::update;

			typename BaseType::FunctionPointerStorage::BeginPlay_t beginPlayPtr =
				&detail::BeginPlay_t<ThisType, BaseType>::beginPlay;

			elem->children.push_back(
				std::make_pair(typename BaseType::FunctionPointerStorage{ getAllMatchingPtr, updatePtr, beginPlayPtr }, ret)
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
		tuple_for_each_with_index(bases, [thisptr = this](auto& ptr, auto)
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
