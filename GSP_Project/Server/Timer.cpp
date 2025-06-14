#include "pch.h"
#include "Timer.h"
#include "IocpCore.h"
#include "Session.h"
#include "Over.h"

std::priority_queue<TimerEvent> TimerQueue;
std::mutex timer_lock;

void Timer::Start()
{
	if (_running) return;
	_running = true;
	_timerThread = std::thread([this]() { Run(); });
}

void Timer::Stop()
{
	if (false == _running) return;
	_running = false;
	if (_timerThread.joinable()) {
		_timerThread.join();
	}
}

void Timer::Run()
{
	using namespace std::chrono;

    while (_running) {
        while (true) {
            std::unique_lock<std::mutex> lock(timer_lock);
            if (TimerQueue.empty()) break;

            auto& k = TimerQueue.top();
            if (k.wakeup_time > high_resolution_clock::now()) break;

            EX_OVER* o = new EX_OVER();
            switch (k.event) {
            case EV_MOVE: {
                o->eventType = EventType::NPC_MOVE;
                break;
            }
            case EV_HEAL: break;
            case EV_ATTACK: break;
            }

            _iocpCore->PQCS(o, k.obj_id);
            TimerQueue.pop();
        }
        std::this_thread::sleep_for(10ms);
    }
}
