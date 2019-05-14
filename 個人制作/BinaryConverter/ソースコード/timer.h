#pragma once

#include <chrono>

class Timer
{
private:
	std::chrono::system_clock::time_point start_, stop_;

public:
	double GetTime()
	{
		return static_cast<double>(std::chrono::duration_cast<std::chrono::nanoseconds>(this->stop_ - this->start_).count()) / 1000000000;
	}

public:
	void Start()
	{
		this->start_ = std::chrono::system_clock::now();
	}
	void Stop()
	{
		this->stop_ = std::chrono::system_clock::now();
	}
};