#pragma once
#include <type_traits>

#include <boost/hana.hpp>

#include "Entity.h"

template <typename Haystack, typename Needle>
constexpr auto get_index_of_first_matching(Haystack&&, Needle&& n)
{
  using Pred = decltype(boost::hana::equal.to(n));
  using Pack = typename boost::hana::detail::make_pack<Haystack>::type;
  constexpr auto index = boost::hana::detail::index_if<Pred, Pack>::value;
  return boost::hana::int_c<index>;
}


template <typename T>
constexpr auto remove_dups(T tuple)
{
	auto pred = [](auto currentTuple, auto newItem)
		{
			return boost::hana::if_(boost::hana::contains(currentTuple, newItem), currentTuple, boost::hana::append(currentTuple, newItem));
		};
	return decltype(boost::hana::fold(tuple, boost::hana::make_tuple(), pred)){};
}

template<typename T>
constexpr auto is_tuple(T typeToCheck = T{})
{
	return boost::hana::bool_c<boost::hana::is_a<boost::hana::tuple_tag, decltype(typeToCheck)>>;
}

namespace detail 
{

template<typename Tup, typename F, typename Index>
constexpr bool for_each_index_IMPL(Tup&& tup, F&& func, Index current_index)
{
	using namespace boost::hana::literals;
	
	std::forward<F>(func)(std::forward<Tup>(tup)[current_index]);
	
	return boost::hana::if_(boost::hana::size(tup) == current_index, true, for_each_index_IMPL(std::forward<Tup>(tup), std::forward<F>(func), current_index + 1_c)); 
}
	
}

template<typename Tup, typename F>
constexpr void for_each_index(Tup&& tuple, F&& func)
{
	using namespace boost::hana::literals;
	
	return for_each_index_IMPL(std::forward<Tup>(tuple), std::forward<F>(func), 0_c);
}






