namespace detail
{
template <int numLeft, typename... TupleArgs>
struct call_with_tuple_impl
{
	template <typename Fun, typename... Args>
	static auto apply(Fun&& fun, std::tuple<TupleArgs...>& tup, Args&&... args)
	{
		return call_with_tuple_impl<numLeft - 1, TupleArgs...>::apply(
			std::forward<Fun>(fun),
			tup,
			std::forward<Args>(args)...,
			std::get<sizeof...(TupleArgs)-numLeft>(tup));
	}
};
template <typename... TupleArgs>
struct call_with_tuple_impl<0, TupleArgs...>
{
	template <typename Fun, typename... Args>
	static auto apply(Fun&& fun, std::tuple<TupleArgs...>& tup, Args&&... args)
	{
		return fun(std::forward<Args>(args)...);
	}
};
}

template <typename Fun, typename... Args>
auto callWithTuple(Fun&& fun, std::tuple<Args...>& args)
{
	return detail::call_with_tuple_impl<sizeof...(Args), Args...>::apply(std::forward<Fun>(fun), args);
}