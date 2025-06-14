#pragma once
#include <chrono>

enum T_EVENT_TYPE { EV_MOVE, EV_HEAL, EV_ATTACK };
struct TimerEvent {
	int obj_id;
	std::chrono::high_resolution_clock::time_point wakeup_time;
	T_EVENT_TYPE event;
	int target_id;

	constexpr bool operator < (const TimerEvent& _Left) const
	{
		return (wakeup_time > _Left.wakeup_time);
	}
};

class Timer
{
public:
	Timer(IocpCoreRef iocpCore) : _iocpCore(iocpCore) {};
	~Timer() { if (_running) Stop(); };

	void Start();
	void Stop();

private:
	void Run();  // do_timer_2의 내용을 여기에

private:
	IocpCoreRef _iocpCore;
	std::thread _timerThread;
	std::atomic_bool _running = false;
};

extern std::priority_queue<TimerEvent> TimerQueue;
extern std::mutex timer_lock;

