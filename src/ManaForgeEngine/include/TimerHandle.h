#pragma once
#include <Engine.h>
#include <TimerManager.h>

class TimerHandle
{
	friend TimerManager;

public:
	template <typename... Args>
	static std::shared_ptr<TimerHandle> create(Args&&... args);

	TimerHandle& operator=(const TimerHandle& other) = delete;
	TimerHandle& operator=(TimerHandle&& other)
	{
		// we don't want to copy this if they dont use the exact same manager -- so check the mem addresses!
		assert(&(other.manager) == &manager);

		return *this;
	}

	inline void cancel();
	inline void setLoops(bool loops);
	inline bool getLoops() const;

	inline TimerManager::Double_duration_t getDuration() const;
	inline TimerManager::Double_duration_t getTimeRemaining() const;

	inline TimerManager::Time_point_t getEndTime() const { return endTime; }

	// convienece function that cancels the current TimerHandle and starts a new one with the same properties.
	inline std::shared_ptr<TimerHandle> reset();

	bool operator<(const TimerHandle& other) const { return endTime < other.endTime; }
	bool operator>(const TimerHandle& other) const { return endTime > other.endTime; }

private:
	inline TimerHandle(TimerManager& manager,
		TimerManager::Time_point_t endTime,
		TimerManager::Double_duration_t duration,
		TimerManager::Callback_t callback,
		bool loops)
		: manager(manager)
		, endTime(endTime)
		, duration(duration)
		, callback(callback)
		, loops(loops){};

	TimerHandle(const TimerHandle& other) = delete;
	TimerHandle(TimerHandle&& other)
		: manager(other.manager)
	{
	}

	TimerManager::Time_point_t endTime;
	TimerManager::Callback_t callback;
	TimerManager::Double_duration_t duration;
	bool loops;

	TimerManager& manager;
};

template <typename... Args>
std::shared_ptr<TimerHandle> TimerHandle::create(Args&&... args)
{
	return std::shared_ptr<TimerHandle>(new TimerHandle(std::forward<Args>(args)...));
}

inline void TimerHandle::cancel()
{
	// empty the callback
	callback = TimerManager::Callback_t();
}
inline void TimerHandle::setLoops(bool loops) { this->loops = loops; }

inline bool TimerHandle::getLoops() const { return this->loops; }

inline TimerManager::Double_duration_t TimerHandle::getDuration() const { return this->duration; }

inline TimerManager::Double_duration_t TimerHandle::getTimeRemaining() const
{
	auto now = TimerManager::Time_point_t{std::chrono::system_clock::now()};

	return now - endTime;
}
inline std::shared_ptr<TimerHandle> TimerHandle::reset()
{
	cancel();

	return manager.addTimer(duration, callback, loops);
}