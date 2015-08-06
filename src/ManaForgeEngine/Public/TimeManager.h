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

	inline void update();
	inline void addTimer(Double_duration_t timeToWait, Callback_t callback, bool loops);

private:
	
	struct Elem_t
	{
		Elem_t(Time_point_t endTime, Double_duration_t duration, Callback_t callback, bool loops)
			: endTime(endTime)
			, duration(duration)
			, callback(callback)
			, loops(loops)
		{
		}

		bool operator<(const Elem_t& other) const { return endTime < other.endTime; }
		bool operator>(const Elem_t& other) const { return endTime > other.endTime; }

		Time_point_t endTime;
		Callback_t callback;
		Double_duration_t duration;
		bool loops;
	}; 
	using Queue_t = std::priority_queue<Elem_t, std::deque<Elem_t>, std::greater<Elem_t>>;


	Queue_t queue;
};

#include <TimerHandle.h>


inline void TimerManager::update()
{
	// get the current time
	Time_point_t now = std::chrono::system_clock::now();

	// check if the queue is empty first
	// while the top element needs to be called
	while (!queue.empty() && now >= queue.top().endTime)
	{
		auto elem = Elem_t{queue.top()};
		// call the callback
		elem.callback();

		queue.pop();

		// if it is a looping timer, loop it.
		if (elem.loops) {
			using namespace std::chrono_literals;

			// that would cause an infinite loop
			assert(elem.duration != 0s);

			queue.emplace(elem.endTime + elem.duration, elem.duration, elem.callback, true);
		}
	}
}

inline void TimerManager::addTimer(Double_duration_t timeToWait, Callback_t callback, bool loops)
{
	queue.emplace(std::chrono::system_clock::now() + timeToWait, timeToWait, callback, loops);
}