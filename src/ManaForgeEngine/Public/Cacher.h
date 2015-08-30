#include "Engine.h"

#include <boost/optional.hpp>
#include <boost/concept_check.hpp>


#include <unordered_map>
#include <memory> // std::weak_ptr
#include <utility>


template<typename Val, typename Key = std::string, typename Container = std::unordered_map<Key, std::weak_ptr<Val>>>
class WeakCacher
{
	static_assert(std::is_same<typename Container::mapped_type, std::weak_ptr<Val>>::value);
public:
	WeakCacher() = default;
	WeakCacher(const WeakCacher& other) = delete;
	WeakCacher(WeakCacher&& other) = default;
	
	WeakCacher& operator=(const WeakCacher& other) = delete;
	WeakCacher& operator=(WeakCacher&& other) = default;

	template<typename In, typename = 
		std::enable_if_t
		<
			std::is_same
			<
				std::decay_t<In>
				, Key
			>::value
		>
	>
	std::shared_ptr<Val> boost::optional<Val> get(In&& key)
	{
		auto&& iter = values.find(std::forward<In>(key));

		if (iter != values.end())
		{
			auto&& value = iter->second;
			if (value.expired())
			{
				return nullptr;
			}
			return value;
		}

		return nullptr;
	}

	template<typename InVal, InKey, typename =
		std::enable_if_t
		<			
			std::is_same
			<
				std::decay_t<InKey>
				, Key
			>::value
				&&
			std::is_convertable
			<
				std::decay_t<InVal>
				, std::weak_ptr<Val>
			>::value
		>
	>
	void set(InKey&& key, InVal&& val)
	{
		values[std::forward<InKey>(key)] = std::forward<inVal>(val);
	}

private:
	Container values;
};

template<typename Val, typename Key = std::string, typename Container = std::unordered_map<Key, std::shared_ptr<Val>>>
class StrongCacher
{
	static_assert(std::is_same<typename Container::mapped_type, std::shared_ptr<Val>>::value);
public:
	StrongCacher() = default;
	StrongCacher(const StrongCacher& other) = delete;
	StrongCacher(StrongCacher&& other) = default;
	
	StrongCacher& operator=(const StrongCacher& other) = delete;
	StrongCacher& operator=(StrongCacher&& other) = default;

	template<typename In, typename = 
		std::enable_if_t
		<
			std::is_same
			<
				std::decay_t<In>
				, Key
			>::value
		>
	>
	std::shared_ptr<Val> boost::optional<Val> get(In&& key)
	{
		auto&& iter = values.find(std::forward<In>(key));

		if (iter != values.end())
		{
			
			return iter->second;
		}

		return nullptr;
	}

	template<typename InVal, InKey, typename =
		std::enable_if_t
		<			
			std::is_same
			<
				std::decay_t<InKey>
				, Key
			>::value
				&&
			std::is_convertable
			<
				std::decay_t<InVal>
				, std::shared_ptr<Val>
			>::value
		>
	>
	void set(InKey&& key, InVal&& val)
	{
		values[std::forward<InKey>(key)] = std::forward<inVal>(val);
	}

private:
	Container values;
};

