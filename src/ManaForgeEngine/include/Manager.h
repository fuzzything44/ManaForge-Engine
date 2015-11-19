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

	using AllManagers = typename detail::FindManagers<boost::mpl::vector<ThisType>>::type;
	static_assert(boost::mpl::is_sequence<AllManagers>::value, "AllManagers needs to be a sequence.");

	using AllManagersButThis = typename boost::mpl::remove<AllManagers, ThisType>::type;
	static_assert(boost::mpl::is_sequence<AllManagersButThis>::value, "AllManagers needs to be a sequence.");

private:
	template<typename... Args>
	using CatComponents_t = CatSequences_t<typename Args::AllComponents...>;
	template<typename... Args>
	using CatTags_t = CatSequences_t<typename Args::AllTags...>;
public:
	using AllComponents = CatSequences_t<MyComponents, Unduplicate_t<ExpandSequenceToVaraidic_t<AllManagersButThis, CatComponents_t>>>;
	using AllTags = CatSequences_t<MyTags, Unduplicate_t<ExpandSequenceToVaraidic_t<AllManagersButThis, CatTags_t>>>;

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
		return boost::mpl::contains<AllTags, Test>::type::value;
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
	template <typename Tag> static constexpr size_t getMyTagID()
	{
		static_assert(isTag<Tag>(), "Must be tag");

		return boost::mpl::distance<
			typename boost::mpl::begin<MyTags>::type
			, typename boost::mpl::find<MyTags, Tag>::type
		>::type::value;
	}

	template<typename ComponentOrTag>
	static constexpr size_t getComponentOrTagID()
	{
		return detail::getComponentOrTagID_IMPL<ThisType, ComponentOrTag>::value;
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
		typename boost::mpl::at_c<AllTags, at - ThisType::getNumComponents()>::type;

private:
	template<size_t at, bool isComponent = (at < ThisType::getNumComponents())>
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
			>;
	};
public:

	template<typename SignatureToCheck>
	static constexpr bool isSignature()
	{
		static_assert(boost::mpl::is_sequence<SignatureToCheck>::value, "Must be a sequence");

		// make a new sequence -- each element is if the element is a component or a tag
		using Transformed = typename boost::mpl::transform<SignatureToCheck, IsSignatureIMPL<boost::mpl::placeholders::_1>>::type;

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

	template<typename ComponentOrTag, bool = ThisType::template isTag<ComponentOrTag>()>
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

	template<typename Sequence>
	using RemoveTags_t = typename detail::RemoveTags<ThisType, Sequence>::type;

	template<typename Sequence>
	using RemoveComponents_t = typename detail::RemoveComponents<ThisType, Sequence>::type;

	template<typename... Types>
	using TupleOfConstRefs_t = std::tuple<const Types&...>;
	template<typename Signature /*a boost vector*/>
	auto newEntity(ExpandSequenceToVaraidic_t<Signature, TupleOfConstRefs_t> components/*tuple of the components*/)
	{
		using ManagerType = FindMostBaseManagerForSignature_t<Signature>;
		
		auto entRet = std::make_unique<Entity<ManagerType, Signature>>(*this);

		auto&& storage = getRefToManager<ManagerType>().entityStorage;
		
		
		size_t entID = storage.size();

		boost::fusion::for_each(components, 
			[this, &entRet, entID](auto&& a)
		{
			using ComponentOrTagType = std::decay_t<decltype(a)>;

			auto&& compStorage = this->template getComponentStorage<ComponentOrTagType>();
			compStorage.emplace_back(std::move(a));
			size_t compID = compStorage.size() - 1;

			auto&& compEntStorage = this->template getComponentEntityStorage<ComponentOrTagType>();
			compEntStorage.push_back(entRet.get());

			entRet->componentIDs[ManagerType::template getComponentOrTagID<ComponentOrTagType>()] = compID;

		});

		auto&& ret = entRet.get();
		storage.emplace_back(std::move(entRet));

		return ret;
	}

	template<typename...Args>
	using TupleOfVectorRefrences = std::tuple<std::vector<Args>&...>;


	// returns the elements created [first, last)
	template<typename Signature>
	std::pair<size_t, size_t> createEntityBatch(
		ExpandSequenceToVaraidic_t<RemoveTags_t<Signature>, TupleOfVectorRefrences> components,
		size_t numToConstruct)
	{
		static_assert(isSignature<Signature>(), "Must be a signagure");
		

		size_t firstIndex = (*nextIndex)++;
		size_t onePastLastIndex = firstIndex + numToConstruct;

		using Components = RemoveTags_t<Signature>;
		using Tags = RemoveComponents_t<Signature>;

		std::vector<size_t> indiciesToConstruct(numToConstruct);

		// make a vector that stores the elements firstIndex...onePastLastIndex
		for (auto ind = firstIndex; ind != onePastLastIndex; ++ind)
		{
			indiciesToConstruct[ind - firstIndex] = ind;
		}
		
		// add components
		boost::fusion::for_each(components, [this, &indiciesToConstruct](auto&& componentVector)
		{
			using ComponentType = typename std::decay_t<decltype(componentVector)>::value_type;
			using ManagerType = GetManagerFromComponent_t<ComponentType>;

			constexpr const size_t componentID = ManagerType::template getMyComponentID<ComponentType>();

			auto&& componentStorage = std::get<componentID>(this->getRefToManager<ManagerType>().componentStorage);

			std::vector<std::pair<size_t, ComponentType>> transformed(componentVector.size());

			std::transform(indiciesToConstruct.begin(), indiciesToConstruct.end(), componentVector.begin(), transformed.begin(), 
				[](size_t& index, ComponentType& comp)
			{
				return std::pair<size_t, ComponentType>{ index, std::move(comp) };
			});

			componentStorage.insert(transformed.begin(), transformed.end());
		});

		// add tags
		boost::mpl::for_each<Tags>([this, onePastLastIndex](auto tag)
		{
			using TagType = decltype(tag);
			using ManagerForTag = GetManagerFromTag_t<TagType>;

			const constexpr size_t tagID = ManagerForTag::template getMyTagID<TagType>();

			auto&& tagStorage = std::get<tagID>(this->getRefToManager<ManagerForTag>().tagStorage);

			tagStorage.resize(onePastLastIndex, true);

		});

		return std::make_pair(firstIndex, onePastLastIndex);
	}
	void destroyEntity(EntityBase handle)
	{
		// TODO: reimplement
		
	}

	template<typename Component>
	Component& getComponent(EntityBase* handle)
	{
		using ManagerForComponent = GetManagerFromComponent_t<Component>;

		auto&& manager = this->template getRefToManager<ManagerForComponent>();

		return manager.template getComponentStorage<Component>()[handle->getComponentID(handle, getComponentID<Component>())];
	}

	template<typename Component>
	bool hasComponent(size_t handle)
	{
		//TODO: reimplement
	}

	template<typename Tag>
	bool hasTag(size_t handle)
	{
		static_assert(isTag<Tag>(), "Tag must be a Tag");
		// get constants for convience
		using ManagerForTag = GetManagerFromTag_t<Tag>;
		constexpr size_t managerID = getManagerID<ManagerForTag>();
		constexpr size_t tagID = ManagerForTag::template getMyTagID<Tag>();

		auto&& tagStorage = std::get<tagID>(getRefToManager<ManagerForTag>().tagStorage);

		return tagStorage[handle];
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
		using Tags = RemoveComponents_t<SignatureToRun>;
		using Components = RemoveTags_t<SignatureToRun>;

		using FirstComponent = typename boost::mpl::at_c<Components, 0>::type;

		auto&& componentEntStorage = getComponentEntityStorage<FirstComponent>();

		for (auto&& ent : componentEntStorage)
		{
			bool matches = true;
			for_each_no_construct_ptr<SignatureToRun>(
				[&functor, &ent, &matches](auto ptr)
			{
				using Comp = std::decay_t<decltype(*ptr)>;
				if (matches) matches = ent->hasComponentOrTag(ThisType::template getComponentID<Comp>());
			});

			callFunctionWithSigParams<SignatureToRun>(ent, std::forward<F>(functor));
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

	std::shared_ptr<size_t> nextIndex;


	template<typename... Args>
	using TupleOfComponentStorage = std::tuple
		<
			std::pair
			<
				std::vector<EntityBase*>, std::vector<Args>
			>...
		>;
	ExpandSequenceToVaraidic_t<MyComponents, TupleOfComponentStorage> componentStorage;

	
	std::vector<std::unique_ptr<EntityBase>> entityStorage;

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
