#pragma once
#include <type_traits>

#include <boost/mpl/contains.hpp>
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

}

