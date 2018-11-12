#include "pch.h"
#include "Timer.h"


Timer::Timer()
	:beginTime_(std::chrono::steady_clock::now()),
	endTime_(std::chrono::steady_clock::now()),
	duration_(0.0)
{
}


Timer::~Timer()
{
}

void Timer::Tick()
{
	endTime_ = std::chrono::steady_clock::now();
	duration_ = endTime_ - beginTime_;
	beginTime_ = std::chrono::steady_clock::now();
}

int Timer::GetElapsedTime()
{
	return std::chrono::duration_cast<std::chrono::milliseconds>(duration_).count();
}
