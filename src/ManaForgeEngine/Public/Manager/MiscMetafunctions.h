#pragma once
#include <type_traits>

#include <boost/mpl/contains.hpp>
#include <boost/mpl/transform.hpp>
#include <boost/mpl/deref.hpp>

namespace detail
{
	template<typename ManagerType, typename ComponentOrTag, bool = ManagerType::template isTag<ComponentOrTag>()>
	struct GetTagOrComponentID_IMPL
	{
		static const constexpr size_t value = ManagerType::template getTagID<Tag>();
	};
	template<typename ManagerType, typename Component>
	struct GetTagOrComponentID_IMPL<ManagerType, Component, false>
	{
		static const constexpr size_t value = ManagerType::template getComponentID<Component>();
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

	template<typename ManagerType, typename TagToCheckFor, typename CurrentIter, typename EndIter,
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
		static_assert(ManagerType::template isTag<TagToCheckFor>(), "needs to be a tag");

		using type =
			typename GetManagerFromTag
			<
				ManagerType
				, TagToCheckFor
				, typename boost::mpl::next<CurrentIter>::type
				, EndIter
			>::type
			;
	};

	template<typename ManagerType, typename TagToCheckFor, typename CurrentIter, typename EndIter>
	struct GetManagerFromTag<ManagerType, TagToCheckFor, CurrentIter, EndIter, std::true_type>
	{
		using type = typename boost::mpl::deref<CurrentIter>::type;
		static_assert(ManagerType::template isManager<type>(), "Internal Error");
	};

	template<typename ManagerType, typename TagToCheckFor, typename EndIter>
	struct GetManagerFromTag<ManagerType, TagToCheckFor, EndIter, EndIter, std::false_type>
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

	template<typename Manager, typename SignatureToFind, bool running>
	struct FindMostBaseManagerForSignature
	{
		static_assert(boost::mpl::is_sequence<SignatureToFind>::value, "Signatures are sequences");
		static_assert(Manager::template isSignature<SignatureToFind>(), "Must be a signature");

		using ManagerWithComponent =
			typename boost::mpl::deref
			<
				typename boost::mpl::find
				<
					typename boost::mpl::transform
					<
						typename Manager::MyBases
						, IsSignature<boost::mpl::placeholders::_1, SignatureToFind>
					>
					, std::true_type
				>::type
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

}

