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

#include "MPLHelp.h"

#include "Manager/Callbacks.h"
#include "Manager/GetRefToManager.h"
#include "Manager/FindAllManagers.h"
#include "Manager/MiscMetafunctions.h"

#include "MappedVector.h"

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

	template<typename Sequence>
	using RemoveTags_t = typename detail::RemoveTags<ThisType, Sequence>::type;

	template<typename Sequence>
	using RemoveComponents_t = typename detail::RemoveComponents<ThisType, Sequence>::type;

	// FUNCTIONS FOR ENTITES - CREATING, DESTROYING, COMPONENT HANDLING
	size_t createEntity()
	{
		auto myIndex = (*nextIndex)++;

		return myIndex;
	}

private:
	template<typename TupleType, typename Sequence, size_t ID = 0
		, bool needsExit = boost::mpl::size<Sequence>::type::value == ID
	>
	struct CreateEntityBatch_IMPL
	{
		using Component = typename boost::mpl::at_c<Sequence, ID>::type;
		using ManagerType = GetManagerFromComponent_t<Component>;

		static_assert(ThisType::template isComponent<Component>(), "Must be a component");

		static void apply(ThisType& manager, TupleType& components, std::vector<size_t>& indicies)
		{
			auto&& storage = manager.template getRefToManager<ManagerType>().template getComponentStorage<Component>();

			MappedVector<int> a;

			auto&& storageToCopy = std::get<ID>(components);

			storage.data.insert(storage.data.end(), std::make_move_iterator(storageToCopy.begin()), std::make_move_iterator(storageToCopy.end()));
			storage.indicies.insert(storage.indicies.end(), indicies.begin(), indicies.end());

			CreateEntityBatch_IMPL<TupleType, Sequence, ID + 1>::apply(manager, components, indicies);

		}
	};
	template<typename TupleType, typename Sequence, size_t ID>
	struct CreateEntityBatch_IMPL<TupleType, Sequence, ID, true>
	{
		static void apply(ThisType& manager, TupleType& components, std::vector<size_t>& indicies)
		{}
	};

public:
	template<typename...Args>
	using TupleOfVectorRefrences = std::tuple<std::vector<Args>&...>;

	template<typename Signature>
	std::vector<size_t> createEntityBatch(
		ExpandSequenceToVaraidic_t<RemoveTags_t<Signature>, TupleOfVectorRefrences> components,
		size_t numToConstruct)
	{
		static_assert(isSignature<Signature>(), "Must be a signagure");

		// TODO: reimplement

		return std::vector<size_t>();
	}
	void destroyEntity(size_t handle)
	{
		auto&& iterToEntity = entityStorage.begin(); std::advance(iterToEntity, handle);

		entityStorage.erase(iterToEntity);
	}

	template<typename Component, typename... Args>
	Component& addComponent(size_t handle, Args&&... args)
	{
		static_assert(isComponent<Component>(), "Component must be a component");

		// get constants for convience
		using ManagerForComponent = GetManagerFromComponent_t<Component>;
		constexpr size_t managerID = getManagerID<ManagerForComponent>();

		constexpr size_t componentID = getComponentID<Component>();

		// set the component with the arguments passed
		auto&& iter = getComponentStorage<Component>().emplace(handle, std::forward<Args>(args)...);

		return getComponentStorage<Component>()[handle];
	}
	template<typename Component>
	void removeComponent(size_t handle)
	{
		getComponent(handle);
	}
	template<typename Component>
	Component& getComponent(size_t handle)
	{
		static_assert(isMyComponent<Component>(), "Component must be a component");


		// get constants for convience
		using ManagerForComponent = GetManagerFromComponent_t<Component>;
		constexpr size_t componentID = ManagerForComponent::template getMyComponentID<Component>();

		assert(std::get<componentID>(componentStorage).elemExists(handle));

		return getComponentStorage<Component>()[handle];
	}

	template<typename Component>
	bool hasComponent(size_t handle)
	{
		static_assert(isComponent<Component>(), "Component must be a Component");
		// get constants for convience
		using ManagerForComponent = GetManagerFromComponent_t<Component>;
		constexpr size_t managerID = getManagerID<ManagerForComponent>();
		constexpr size_t componentID = ManagerForComponent::template getComponentID<Component>();

		// get the entity
		EntityType& entity = entityStorage[handle];

		return entity.components[componentID];
	}

	template<typename Tag>
	void addTag(size_t handle)
	{
		static_assert(ThisType::template isTag<Tag>(), "Tag must be a Tag");
		// get constants for convience
		using ManagerForTag = GetManagerFromTag_t<Tag>;
		constexpr size_t managerID = getManagerID<ManagerForTag>();
		constexpr size_t tagID = ManagerForTag::template getMyTagID<Tag>();

		auto&& tagStorage = std::get<tagID>(getRefToManager<ManagerForTag>().tagStorage);

		if (tagStorage.size() <= handle)
		{
			tagStorage.resize(handle + 1);
			tagStorage[handle] = true;
		}
	}
	template<typename Tag>
	void removeTag(size_t handle)
	{
		static_assert(isTag<Tag>(), "Tag must be a Tag");
		// get constants for convience
		using ManagerForTag = GetManagerFromTag_t<Tag>;
		constexpr size_t managerID = getManagerID<ManagerForTag>();
		constexpr size_t tagID = ManagerForTag::template getMyTagID<Tag>();


		auto&& tagStorage = std::get<tagID>(getRefToManager<ManagerForTag>().tagStorage);

		tagStorage[handle] = false;
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

	template<typename Component>
	MappedVector<Component>& getComponentStorage()
	{
		static_assert(isComponent<Component>(), "Component must be a component");

		using Manager_Type = GetManagerFromComponent_t<Component>;

		constexpr auto ID = Manager_Type::template getMyComponentID<Component>();

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

private:
	template<typename...Args>
	using VoidStdFunctionArguments = std::function<void(Args...)>;
public:

	template<typename Signature>
	using SignatureToFunction = ExpandSequenceToVaraidic_t<Signature, VoidStdFunctionArguments>;

private:

	template<typename CurrentIter, typename EndIter>
	struct CallFunctionWighSigParamsIMPL
	{
		template<typename F, typename...Args>
		static void apply(ThisType& manager, size_t ID, F&& func, Args&&...args)
		{
			using NextIter = typename boost::mpl::next<CurrentIter>::type;
			using ComponentType = typename boost::mpl::deref<CurrentIter>::type;
			using ManagerType = ThisType::template GetManagerFromComponent_t<ComponentType>;

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
					, manager.getRefToManager<ManagerType>().template getComponent<ComponentType>(ID)
					);
		}
	};
	template<typename EndIter>
	struct CallFunctionWighSigParamsIMPL<EndIter, EndIter>
	{
		template<typename F, typename...Args>
		static void apply(ThisType& manager, size_t ID, F&& func, Args&&...args)
		{
			std::forward<F>(func)(std::forward<Args>(args)...);
		}
	};
	template<typename A, typename B> friend struct CallFunctionWighSigParamsIMPL;
public:

	// CALLING FUNCTIONS ON ENTITIES
	template<typename SignatureToRun, typename F>
	void callFunctionWithSigParams(size_t ID, F&& func)
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

	template<typename... Args>
	using TupleOfVectorIterators = std::tuple<typename MappedVector<Args>::iterator...>;

	template<typename Signature>
	using ComponentIterTypes = ExpandSequenceToVaraidic_t<Signature, TupleOfVectorIterators>;

	template<typename Signature>
	ComponentIterTypes<Signature> generateBeginIters()
	{
		ComponentIterTypes<Signature> sig;

		boost::fusion::for_each(sig, [this](auto&& a)
		{
			using ComponentType = std::decay_t<typename std::iterator_traits<std::decay_t<decltype(a)>>::value_type::second_type>;
			using ManagerType = GetManagerFromComponent_t<ComponentType>;

			constexpr const size_t componentID = ManagerType::template getMyComponentID<ComponentType>();

			a = std::get<componentID>(this->getRefToManager<ManagerType>().componentStorage).begin();
		});

		return sig;
	}

	template<typename Signature>
	ComponentIterTypes<Signature> generateEndIters()
	{
		ComponentIterTypes<Signature> sig;

		boost::fusion::for_each(sig, [this](auto&& a)
		{
			using ComponentType = std::decay_t<typename std::iterator_traits<std::decay_t<decltype(a)>>::value_type::second_type>;
			using ManagerType = GetManagerFromComponent_t<ComponentType>;

			constexpr const size_t componentID = ManagerType::template getMyComponentID<ComponentType>();

			a = std::get<componentID>(this->getRefToManager<ManagerType>().componentStorage).end();
		});

		return sig;
	}

	template<typename TupleOfIters>
	static bool advanceUntilEqualIndex(TupleOfIters&& iters, TupleOfIters&& endIters)
	{
		size_t largest = 0;

		// find largest index
		tuple_for_each_with_index(iters, [&largest, &endIters](auto&& iter, auto ID)
		{

			if(iter != std::get<decltype(ID)::value>(endIters)) largest = std::max(largest, iter->first);
		});

		bool succeded = true;

		// advance iterators
		tuple_for_each_with_index(iters, [largest, &succeded, &endIters](auto&& iter, auto ID)
		{
			constexpr size_t index = decltype(ID)::value;

			if (succeded)
			{

				if (iter == std::get<index>(endIters))
				{
					succeded = false;
					return;
				}

				while (iter->first < largest)
				{
					++iter;
				}
			}

		});

		return succeded;
	}

	template<typename Signature, size_t numComponents>
	struct runAllMatchingIMPL_IMPL
	{
		// implementation with components
		template<typename F>
		static void apply(F&& functor, ThisType& manager)
		{
			using Components = RemoveTags_t<Signature>;
			using Tags = RemoveComponents_t<Signature>;


			ComponentIterTypes<Components> componentIters = manager.generateBeginIters<Components>();
			ComponentIterTypes<Components> componentEndIters = manager.generateEndIters<Components>();

			bool shouldCont = advanceUntilEqualIndex(componentIters, componentEndIters);

			while (shouldCont)
			{
				bool matches = true;

				size_t index = std::get<0>(componentIters)->first;

				// check for matching tags
				boost::mpl::for_each<Tags>([&manager, &matches, index](auto&& tag)
				{
					using TagType = std::decay_t<decltype(tag)>;
					using Manager_t = GetManagerFromTag_t<TagType>;

					constexpr const size_t compID = Manager_t::template getMyTagID<TagType>();

					if (matches) matches = std::get<compID>(manager.getRefToManager<Manager_t>().tagStorage)[index];
				});

				if (matches)
				{
					manager.callFunctionWithSigParams<Components>(index, std::forward<F>(functor));
				}

				// iterate it so it gets used as the next index
				++std::get<0>(componentIters);

				shouldCont = manager.advanceUntilEqualIndex(componentIters, componentEndIters);


			}

		}
	};

	template<typename Signature>
	struct runAllMatchingIMPL_IMPL<Signature, 0>
	{
		// implementation with no components
		template<typename F>
		static void apply(F&& functor, ThisType& manager)
		{
			using Components = RemoveTags_t<Signature>;
			using Tags = RemoveComponents_t<Signature>;

			size_t index = 0;
			bool shouldContinue = true;
			while (shouldContinue)
			{

				bool matches = true;


				// check for matching tags
				boost::mpl::for_each<Tags>([&manager, &matches, index, &shouldContinue](auto&& tag)
				{
					using TagType = std::decay_t<decltype(tag)>;
					using Manager_t = GetManagerFromTag_t<TagType>;

					constexpr const size_t compID = Manager_t::template getMyTagID<TagType>();

					auto&& storage = std::get<compID>(manager.getRefToManager<Manager_t>().tagStorage);
					if (storage.size() <= index)
					{
						shouldContinue = false;
						matches = false;
					}
					else
					{
						if (matches) matches = (storage.size() > index) && storage[index];
					}
				});

				if (matches)
				{
					manager.callFunctionWithSigParams<Components>(index, std::forward<F>(functor));
				}

				++index;
			}

		}
	};

	template<typename SignatureToRun, typename F>
	void runAllMatchingIMPL(F&& functor)
	{
		static_assert (std::is_same<FindMostBaseManagerForSignature_t<SignatureToRun>, ThisType>::value, "Please use runAllMatching instead");

		using Components = RemoveTags_t<SignatureToRun>;
		using Tags = RemoveComponents_t<SignatureToRun>;

		constexpr size_t numComponents = boost::mpl::size<Components>::type::value;

		runAllMatchingIMPL_IMPL<SignatureToRun, numComponents>::apply(std::forward<F>(functor), *this);
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

	std::array<boost::dynamic_bitset<>, (ThisType::getNumMyTags())> tagStorage;

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
