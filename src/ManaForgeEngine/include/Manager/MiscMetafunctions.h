#pragma once
#include <type_traits>

#include <boost/mpl/contains.hpp>
#include <boost/mpl/transform.hpp>
#include <boost/mpl/deref.hpp>

#include "Entity.h"

namespace detail
{

    template<typename Managers, typename ManagerToTest, bool = boost::mpl::contains<Managers, ManagerToTest>::type::value>
    struct AssignBasePointer_t
    {
        static_assert(boost::mpl::is_sequence<Managers>::value, "Must be a sequence");
        static_assert(std::is_base_of<ManagerBase, ManagerToTest>::value, "Must be a manager");

        template<typename... Args>
        using TupleOfEntityPointers = std::tuple<Entity<Args>*...>;


        static void apply(Entity<ManagerToTest>* toApplyTo,  ExpandSequenceToVaraidic_t<Managers, TupleOfEntityPointers> bases)
        {
            constexpr size_t ID = boost::mpl::distance<typename boost::mpl::begin<Managers>::type,
				typename boost::mpl::find<Managers, ManagerToTest>::type>::type::value;

            toApplyTo = std::get<ID>(bases);
        }
    };

    template<typename Managers, typename ManagerToTest>
    struct AssignBasePointer_t<Managers, ManagerToTest, false>
    {
        template<typename... Args>
        using TupleOfEntityPointers = std::tuple<Entity<Args>*...>;


        static void apply(Entity<ManagerToTest>* toApplyTo,  ExpandSequenceToVaraidic_t<Managers, TupleOfEntityPointers> bases)
		{}
    };

    template<typename ManagerType, typename CurrentIter, typename EndIter, typename CurrentVector>
    struct IsolateComponentsFromThisManager
    {
        using Component = typename boost::mpl::deref<CurrentIter>::type;

        using type =
            typename IsolateComponentsFromThisManager
            <
				ManagerType
				, typename boost::mpl::next<CurrentIter>::type
				, EndIter
                , std::conditional_t
                <
                    ManagerType::template isComponent<Component>()
					, typename boost::mpl::push_back<CurrentVector, Component>::type
					, CurrentVector
				>
			>::type;
	};

    template<typename ManagerType, typename EndIter, typename CurrentVector>
    struct IsolateComponentsFromThisManager<ManagerType, EndIter, EndIter, CurrentVector>
    {
        using type = CurrentVector;
    };


	template<typename ManagerType, typename ComponentToCheckFor, typename CurrentIter, typename EndIter,
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
		static_assert(ManagerType::template isComponent<ComponentToCheckFor>(), "needs to be a component");

		using type =
			typename GetManagerFromComponent
			<
				ManagerType
				, ComponentToCheckFor
				, typename boost::mpl::next<CurrentIter>::type
				, EndIter
			>::type
			;
	};

	template<typename ManagerType, typename ComponentToCheckFor, typename CurrentIter, typename EndIter>
	struct GetManagerFromComponent<ManagerType, ComponentToCheckFor, CurrentIter, EndIter, std::true_type>
	{
		using type = typename boost::mpl::deref<CurrentIter>::type;
		static_assert(ManagerType::template isManager<type>(), "Internal Error");
	};

	template<typename ManagerType, typename ComponentToCheckFor, typename EndIter>
	struct GetManagerFromComponent<ManagerType, ComponentToCheckFor, EndIter, EndIter, std::false_type>
	{
		//		static_assert(false, "ERROR, COULD NOT FIND COMPONENT IN ANY MANAGERS");
	};

	template<typename Manager, typename Signature>
	struct IsSignature
	{
		using type =
			std::conditional_t
			<
				Manager::template isSignature<Signature>()
				, std::true_type
				, std::false_type
			>
			;
	};
	template<typename ManagerType, typename SignatureToFind, bool running>
	struct FindMostBaseManagerForSignature
	{
		static_assert(std::is_base_of<ManagerBase, ManagerType>::value, "Must be a subclass of ManagerBase");

		static_assert(boost::mpl::is_sequence<SignatureToFind>::value, "Signatures are sequences");
		static_assert(ManagerType::template isSignature<SignatureToFind>(), "Must be a signature");

		using Transformed_Bools =
			typename boost::mpl::transform // when searching for std::true type
			<
				typename ManagerType::MyBases
				, IsSignature<boost::mpl::placeholders::_1, SignatureToFind>
			>::type
			;

		// Get the manager above this that has the correct components

		static const constexpr size_t ManagerID =
			boost::mpl::distance
			<
				typename boost::mpl::begin<Transformed_Bools>::type
				, typename boost::mpl::find // that was found
				<
					Transformed_Bools // in the vector of std::true_type (matches component) or std::false_type (doesn't match component)
					, std::true_type // when searching for std::true_type (matches component)
				>::type
			>::type::value
			;

		using ManagerWithComponent = typename boost::mpl::at_c<typename ManagerType::MyBases, ManagerID>::type;

		static const constexpr bool runningNext = !std::is_same<ManagerWithComponent, boost::mpl::void_>::value;

		// make sure we either found a Manager or not -- not some other case.
		static_assert(ManagerType::template isManager<ManagerWithComponent>() ^ !runningNext, "INTERNAL ERROR: must be a manager!");

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
				, ManagerType
			>
			;
	};
	template<typename ManagerType, typename SignatureToFind>
	struct FindMostBaseManagerForSignature<ManagerType, SignatureToFind, false>
	{
		using type = ManagerType;
	};



	template<
		typename ManagerType
		, typename Sequence
		, size_t ID = 0
		, bool = ManagerType::template isTagComponent<typename boost::mpl::at_c<Sequence, ID>::type>()
		, bool = boost::mpl::size<Sequence>::type::value == ID
	>
	struct IsolateStorageComponents
	{
		using Tag_t = typename boost::mpl::at_c<Sequence, ID>::type;

		using NewSequence = typename boost::mpl::remove<Sequence, Tag_t>::type;

		using type =
			typename IsolateStorageComponents
			<
				ManagerType
				, NewSequence
				, ID
			>::type;

	};

	template<typename ManagerType, typename Sequence, size_t ID>
	struct IsolateStorageComponents<ManagerType, Sequence, ID, false, false>
	{
		using type =
			std::conditional_t
			<
				ID < boost::mpl::size<Sequence>::size::value
				,typename IsolateStorageComponents
				<
					ManagerType
					, Sequence
					, ID + 1
				>::type
				, Sequence
			>
			;
	};

	template<typename ManagerType, typename Sequence, size_t ID>
	struct IsolateStorageComponents<ManagerType, Sequence, ID, false, true>
	{
		using type =
			Sequence;
	};


	template<
		typename ManagerType
		, typename Sequence
		, size_t ID = 0
		, bool = ManagerType::template isStorageComponent<typename boost::mpl::at_c<Sequence, ID>::type>()
		, bool = boost::mpl::size<Sequence>::type::value == ID
	>
	struct IsolateTagComponents
	{
		using Component = typename boost::mpl::at_c<Sequence, ID>::type;

		using NewSequence = typename boost::mpl::remove<Sequence, Component>::type;

		using type =
			typename IsolateTagComponents
			<
				ManagerType
				, NewSequence
				, ID
			>::type;

	};

	template<typename ManagerType, typename Sequence, size_t ID>
	struct IsolateTagComponents<ManagerType, Sequence, ID, false, false>
	{
		using type =
			std::conditional_t
			<
				ID < boost::mpl::size<Sequence>::size::value
				,typename IsolateTagComponents
				<
					ManagerType
					, Sequence
					, ID + 1
				>::type
				, Sequence
			>
			;
	};

	template<typename ManagerType, typename Sequence, size_t ID>
	struct IsolateTagComponents<ManagerType, Sequence, ID, false, true>
	{
		using type =
			Sequence;
	};

}

