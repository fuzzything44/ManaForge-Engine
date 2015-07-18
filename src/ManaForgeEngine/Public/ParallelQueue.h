#pragma once

#include <queue>
#include <mutex>

template<typename T>
class ParallelQueue
{
public:
	using queue_t = std::queue < T >;

	// perfect forwared it to the queue
	template<typename U,
		typename = std::enable_if<std::is_same<U, T>::value > >
	void enqueue(U&& newValue)
	{
		std::lock_gaurd<std::mutex> lock{ mutex };

		data.push(std::forward<U>(newValue));
	}
	

	T dequeue()
	{
		std::lock_guard<std::mutex> lock{ mutex };

		// make sure it isn't empty
		assert(data.size() != 0);

		auto iter = data.front();
		
		return *iter;
	}

	typename queue_t::size_type size() const
	{
		// acquire lock
		std::lock_guard<std::mutex> lock{ mutex };

		return data.size();
	}

private:
	std::mutex mutex;
	queue_t data;
};