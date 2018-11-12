#pragma once
class Timer
{
private:
	std::chrono::steady_clock::time_point beginTime_;
	std::chrono::steady_clock::time_point endTime_;
	std::chrono::duration<double> duration_;

public:
	Timer();
	~Timer();


	void Tick();
	int GetElapsedTime();
};

