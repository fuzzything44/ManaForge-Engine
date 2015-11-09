#pragma once
#include <type_traits>

#include <boost/mpl/contains.hpp>
#include <boost/mpl/transform.hpp>
#include <boost/mpl/deref.hpp>

namespace detail
{
	template<typename ManagerType, typename Tag, bool = ManagerType::template isTag<Tag>()>
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

	template<typename ManagerType, typename SignatureToFind, bool running>
	struct FindMostBaseManagerForSignature
	{
		static_assert(boost::mpl::is_sequence<SignatureToFind>::value, "Signatures are sequences");
		static_assert(ManagerType::template isSignature<SignatureToFind>(), "Must be a signature");

		using ManagerWithComponent =
			typename boost::mpl::deref
			<
				typename boost::mpl::find
				<
					typename boost::mpl::transform
					<
						typename ManagerType::MyBases
						, IsSignature<boost::mpl::placeholders::_1, SignatureToFind>
					>
					, std::true_type
				>::type
			>::type
			;

		static const constexpr bool runningNext = !std::is_same<ManagerWithComponent, boost::mpl::void_>::value;

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
	template<typename Manager, typename SequenceToFind>
	struct FindMostBaseManagerForSignature<Manager, SequenceToFind, false>
	{
		using type = Manager;
	};




	template<typename ThisManager, typename BaseManager, size_t current, size_t end>
	struct BaseRuntimeSignatureToThisRuntimeSignature_IMPL
	{
		static void apply(typename ThisManager::RuntimeSignature_t& ret, const typename BaseManager::RuntimeSignature_t& toConvert)
		{
			using ThisComponentType = BaseManager::template ComponentOrTagByID<current>;
			constexpr size_t thisComponentID = ThisManager::template getTagOrComponentID<ThisComponentType>();

			ret[thisComponentID] = toConvert[current];

			BaseRuntimeSignatureToThisRuntimeSignature_IMPL<ThisManager, BaseManager, current + 1, end>::apply(ret, toConvert);
		}
	};

	template<typename ThisManager, typename BaseManager, size_t end>
	struct BaseRuntimeSignatureToThisRuntimeSignature_IMPL<ThisManager, BaseManager, end, end>
	{
		static void apply(typename ThisManager::RuntimeSignature_t& ret, const typename BaseManager::RuntimeSignature_t& toConvert)
		{ }
	};

	template<
		typename ManagerType
		, typename Sequence
		, size_t ID = 0
		, bool = ManagerType::template isTag<typename boost::mpl::at_c<Sequence, ID>::type>()
		, bool = boost::mpl::size<Sequence>::type::value == ID
	>
	struct RemoveTags
	{
		using Tag_t = typename boost::mpl::at_c<Sequence, ID>::type;

		using NewSequence = typename boost::mpl::remove<Sequence, Tag_t>::type;

		using type =
			typename RemoveTags
			<
				ManagerType
				, NewSequence
				, ID
			>::type;

	};

	template<typename ManagerType, typename Sequence, size_t ID>
	struct RemoveTags<ManagerType, Sequence, ID, false, false>
	{
		using type =
			std::conditional_t
			<
				ID < boost::mpl::size<Sequence>::size::value
				,typename RemoveTags
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
	struct RemoveTags<ManagerType, Sequence, ID, false, true>
	{
		using type =
			Sequence;
	};

	
	template<
		typename ManagerType
		, typename Sequence
		, size_t ID = 0
		, bool = ManagerType::template isComponent<typename boost::mpl::at_c<Sequence, ID>::type>()
		, bool = boost::mpl::size<Sequence>::type::value == ID
	>
	struct RemoveComponents
	{
		using Component = typename boost::mpl::at_c<Sequence, ID>::type;

		using NewSequence = typename boost::mpl::remove<Sequence, Component>::type;

		using type =
			typename RemoveComponents
			<
				ManagerType
				, NewSequence
				, ID
			>::type;

	};

	template<typename ManagerType, typename Sequence, size_t ID>
	struct RemoveComponents<ManagerType, Sequence, ID, false, false>
	{
		using type =
			std::conditional_t
			<
				ID < boost::mpl::size<Sequence>::size::value
				,typename RemoveComponents
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
	struct RemoveComponents<ManagerType, Sequence, ID, false, true>
	{
		using type =
			Sequence;
	};

}

