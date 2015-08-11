#pragma once
#include "Engine.h"

#include <queue>
#include <chrono>
#include <ratio>
#include <deque>

class TimerHandle;

class TimerManager
{

	friend class TimerHandle;

public:
	using Callback_t = std::function<void()>;
	using Double_duration_t = std::chrono::duration<long double>;
	using Time_point_t = std::chrono::time_point<std::chrono::system_clock, Double_duration_t>;

	TimerManager() = default;
	TimerManager(const TimerManager& other) = delete;
	TimerManager(TimerManager&& other) = default;

	TimerManager& operator=(const TimerManager& other) = delete;
	TimerManager& operator=(TimerManager&&) = default;

	inline void update();
	inline std::shared_ptr<TimerHandle> addTimer(
		Double_duration_t timeToWait, Callback_t callback, bool loops);

private:
	inline static bool compHandle(
		const std::shared_ptr<TimerHandle>& lhs, const std::shared_ptr<TimerHandle>& rhs);

	using Queue_t = std::deque<std::shared_ptr<TimerHandle>>;
	using Comp_t = decltype(&TimerManager::compHandle);

	Queue_t queue;
};

#include <TimerHandle.h>

inline void TimerManager::update()
{
	// get the current time
	Time_point_t now = std::chrono::system_clock::now();

	// check if the queue is empty first
	// while the top element needs to be called
	while (!queue.empty() && now >= queue[0]->getEndTime())
	{

		assert(std::is_heap(queue.begin(), queue.end(), &TimerManager::compHandle));

		auto elem = queue[0];

		// if there is no element...well why?
		if (queue.size() > 1) {
			// put elem in the back of the deque while reatining heap property
			std::pop_heap(queue.begin(), queue.end(), &TimerManager::compHandle);
		}
		// actually remove that element.
		queue.pop_back();

		assert(std::is_heap(queue.begin(), queue.end(), &TimerManager::compHandle));

		// if it is empty, well then it has been removed.
		if (elem->callback) {
			elem->callback();

			// if it is a looping timer, loop it.
			if (elem->loops) {
				using namespace std::chrono_literals;

				// that would cause an infinite loop
				assert(elem->duration != 0s);

				// the element is gonna be destroyed, might as well move the callback!
				addTimer(elem->duration, std::move(elem->callback), true);
			}
		}
	}
}

inline std::shared_ptr<TimerHandle> TimerManager::addTimer(
	Double_duration_t timeToWait, Callback_t callback, bool loops)
{
	// if there is no callback, there is no point
	if (callback) {

		assert(std::is_heap(queue.begin(), queue.end(), &TimerManager::compHandle));

		queue.emplace_back(TimerHandle::create(
			*this, std::chrono::system_clock::now() + timeToWait, timeToWait, callback, loops));
		std::push_heap(queue.begin(), queue.end(), compHandle);

		assert(std::is_heap(queue.begin(), queue.end(), &TimerManager::compHandle));
	}

	return queue[queue.size() - 1];
}

bool TimerManager::compHandle(
	const std::shared_ptr<TimerHandle>& lhs, const std::shared_ptr<TimerHandle>& rhs)
{
	return *lhs > *rhs;
}
