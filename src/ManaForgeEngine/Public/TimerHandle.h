#pragma once
#include <Engine.h>
#include <TimeManager.h>

class TimerHandle
{
public:
	inline TimerHandle(TimerManager::Queue_t::container_type::iterator it) : it(it) { };

	TimerHandle(const TimerHandle& other) = default;
	TimerHandle(TimerHandle&& other) = default;

	TimerHandle& operator=(const TimerHandle& other) = default;
	TimerHandle& operator=(TimerHandle&& other) = default;

	inline void cancel();
	inline void setLoops(bool loops);
	inline bool getLoops();

private:
	TimerManager::Queue_t::container_type::iterator it;
};

inline void TimerHandle::cancel()
{
	auto elem = *it;

	// empty the callback
	elem.callback = TimerManager::Callback_t();
}
inline void TimerHandle::setLoops(bool loops)
{
}
inline bool TimerHandle::getLoops()
{
}