#pragma once
#include <type_traits>

#include "Entity.h"

template <typename Haystack, typename Needle>
constexpr auto get_index_of_first_matching(Haystack&&, Needle&& n)
{
  using Pred = decltype(boost::hana::equal.to(n));
  using Pack = typename boost::hana::detail::make_pack<Haystack>::type;
  constexpr auto index = boost::hana::detail::index_if<Pred, Pack>::value;
  return boost::hana::int_c<index>;
}
