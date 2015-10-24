#pragma once

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
#include <boost/mpl/erase.hpp>
#include <boost/mpl/transform.hpp>
#include <boost/mpl/copy.hpp>
#include <boost/mpl/set.hpp>
#include <boost/mpl/pop_front.hpp>

#include <tuple>


// VECTOR TO PARAM PACK
namespace detail
{

	template<typename CurrentIter, typename EndIter, template<typename...> class ApplyTo, typename... CurrentArgs>
	struct ExpandSequenceToVaraidic
	{

		using CurrentType =
			typename boost::mpl::deref<CurrentIter>::type;

		using NextIter =
			typename boost::mpl::next<CurrentIter>::type;

		using type =
			typename ExpandSequenceToVaraidic
			<
			NextIter						// pass the next iter
			, EndIter						// and the same end iter
			, ApplyTo						// keep the same applyTo, just trying to get it to the end
			, CurrentArgs..., CurrentType	// pass the currentArgs with the currentType added to the beginning. 
			>::type
			;
	};
	template<typename CurrentAndEndIter, template<typename...> class ApplyTo, typename... CurrentArgs>
	struct ExpandSequenceToVaraidic<CurrentAndEndIter, CurrentAndEndIter, ApplyTo, CurrentArgs...>
	{
		using type = ApplyTo<CurrentArgs...>;
	};
}
template<typename Sequence, template<typename...> class ApplyTo>
using ExpandSequenceToVaraidic_t =
typename detail::ExpandSequenceToVaraidic<typename boost::mpl::begin<Sequence>::type, typename boost::mpl::end<Sequence>::type, ApplyTo>::type;
template<typename Begin, typename End, template<typename...> class ApplyTo>
using ExpandSequenceToVaraidicIter_t = typename detail::ExpandSequenceToVaraidic<Begin, End, ApplyTo>::type;

// UNDUPLICATE
namespace detail
{
	template<typename CurrentWriteSequence, typename CurrentIter, typename EndIter>
	struct Unduplicate
	{
		using NextIter = typename boost::mpl::next<CurrentIter>::type;

		using CurrentIterValue = typename boost::mpl::deref<CurrentIter>::type;

		static constexpr bool isDuplicate =
			!std::is_same
			<
			typename boost::mpl::find<CurrentWriteSequence, CurrentIterValue>::type
			, typename boost::mpl::end<CurrentWriteSequence>::type
			>::value
			;

		using WriteSequenceWithAppend =
			std::conditional_t
			<
			isDuplicate
			, CurrentWriteSequence
			, typename boost::mpl::push_back<CurrentWriteSequence, CurrentIterValue>::type
			>
			;

		using type =
			typename Unduplicate
			<
			WriteSequenceWithAppend
			, NextIter
			, EndIter
			>::type
			;
	};

	template<typename CurrentWriteSequence, typename EndIter>
	struct Unduplicate<CurrentWriteSequence, EndIter, EndIter>
	{
		using type = CurrentWriteSequence;
	};
}
template<typename ForwardSequence>
using Unduplicate_t =
typename detail::Unduplicate
<
	boost::mpl::vector<>
	, typename boost::mpl::begin<ForwardSequence>::type
	, typename boost::mpl::end<ForwardSequence>::type
>::type
;

// CONCATINATE SEQUENCES
namespace detail
{
	template<typename... Args>
	struct CatSequences;
	template<typename First, typename Second, typename... Rest>
	struct CatSequences<First, Second, Rest...>
	{
		static_assert(boost::mpl::is_sequence<First>::value, "First must be a sequence");
		static_assert(boost::mpl::is_sequence<Second>::value, "Second must be a sequence");

		using type = typename CatSequences
			<
			typename boost::mpl::copy
			<
			First
			, boost::mpl::back_inserter<Second>
			>::type
			, Rest...
			>::type
			;
	};
	template<typename First, typename Second>
	struct CatSequences<First, Second>
	{
		static_assert(boost::mpl::is_sequence<First>::value, "First must be a sequence");
		static_assert(boost::mpl::is_sequence<Second>::value, "Second must be a sequence");

		using type =
			typename boost::mpl::copy
			<
			First
			, boost::mpl::back_inserter<Second>
			>::type
			;
	};
	template<typename Only>
	struct CatSequences<Only>
	{
		static_assert(boost::mpl::is_sequence<Only>::value, "Only must be a sequence");

		using type = Only;
	};
	template<>
	struct CatSequences<>
	{
		using type = boost::mpl::vector<>;
	};
}
template<typename... Args>
using CatSequences_t = typename detail::CatSequences<Args...>::type;

template<typename Sequence, typename F>
void for_each_no_construct_ptr(F&& functor)
{
	static_assert(boost::mpl::is_sequence<Sequence>::value, "sequence must be a sequence");
	boost::mpl::for_each
		<
		typename boost::mpl::transform
		<
		Sequence
		, std::add_pointer<boost::mpl::placeholders::_1>
		>::type
		>(functor);
}


namespace detail
{
	template<size_t currentIndex, size_t endIndex>
	struct tuple_for_each_with_index_impl
	{
		template<typename F, typename Tuple>
		static void apply(Tuple&& tup, F&& functor)
		{
			std::forward<F>(functor)(std::get<currentIndex>(std::forward<Tuple>(tup)), boost::mpl::int_<currentIndex>{});

			tuple_for_each_with_index_impl<currentIndex + 1, endIndex>::apply(std::forward<Tuple>(tup), std::forward<F>(functor));
		}
	};
	template<size_t endIndex>
	struct tuple_for_each_with_index_impl<endIndex, endIndex>
	{
		template<typename F, typename Tuple>
		static void apply(Tuple&& tup, F&& functor)
		{ }
	};
}
template<typename Tuple, typename F>
void tuple_for_each_with_index(Tuple&& tup, F&& functor)
{
	detail::tuple_for_each_with_index_impl<0, std::tuple_size<std::decay_t<Tuple>>::value>::apply(tup, functor);
}
