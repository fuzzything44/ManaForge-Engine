#include "Engine.h"

#include <unordered_map>
#include <memory> // std::weak_ptr
#include <utility>
#include <type_traits>

template <typename Key, typename Val, typename Container = std::unordered_map<Key, std::weak_ptr<Val>>>
class WeakCacher
{
	static_assert(std::is_same<typename Container::mapped_type, std::weak_ptr<Val>>::value,
		"Wrong Container types. Value must be a std::weak_ptr<Val>");

public:
	WeakCacher() = default;
	WeakCacher(const WeakCacher& other) = delete;
	WeakCacher(WeakCacher&& other) = default;

	WeakCacher& operator=(const WeakCacher& other) = delete;
	WeakCacher& operator=(WeakCacher&& other) = default;

	template <typename In, typename = std::enable_if_t<std::is_same<std::decay_t<In>, Key>::value>>
	std::shared_ptr<Val> get(In&& key)
	{
		auto&& iter = values.find(std::forward<In>(key));

		if (iter != values.end()) {
			auto&& value = iter->second;
			if (value.expired()) {
				return std::shared_ptr<Val>(nullptr);
			}
			return std::shared_ptr<Val>(value);
		}

		return std::shared_ptr<Val>(nullptr);
	}

	template <typename InKey,
		typename InVal,
		typename = std::enable_if_t<std::is_same<std::decay_t<InKey>, Key>::value
									&& std::is_convertible<std::decay_t<InVal>, std::weak_ptr<Val>>::value>>
	std::shared_ptr<Val> set(InKey&& key, InVal&& val)
	{
		values[std::forward<InKey>(key)] = std::forward<InVal>(val);

		return std::shared_ptr<Val>(val);
	}

private:
	Container values;
};

template <typename Key, typename Val, typename Container = std::unordered_map<Key, Val*>>
class StrongCacher
{
	static_assert(std::is_same<typename Container::mapped_type, Val*>::value,
		"Wrong Container types. Value must be a Val*");

public:
	StrongCacher() = default;
	StrongCacher(const StrongCacher& other) = delete;
	StrongCacher(StrongCacher&& other) = default;

	StrongCacher& operator=(const StrongCacher& other) = delete;
	StrongCacher& operator=(StrongCacher&& other) = default;

	~StrongCacher()
	{
		std::for_each(std::begin(values),
			std::end(values),
			[](Container::value_type elem)
			{
				delete elem.second;
			});
	}

	template <typename In, typename = std::enable_if_t<std::is_same<std::decay_t<In>, Key>::value>>
	Val* get(In&& key)
	{
		auto&& iter = values.find(std::forward<In>(key));

		if (iter != values.end()) {

			return iter->second;
		}

		return nullptr;
	}

	template <typename InKey,
		typename InVal,
		typename = std::enable_if_t<std::is_same<std::decay_t<InKey>, Key>::value
									&& std::is_convertible<std::decay_t<InVal>, Val*>::value>>
	Val* set(InKey&& key, InVal&& val)
	{
		values[std::forward<InKey>(key)] = std::forward<InVal>(val);

		return val;
	}

private:
	Container values;
};
