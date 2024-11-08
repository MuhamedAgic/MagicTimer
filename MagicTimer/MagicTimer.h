#pragma once

#include <iostream>
#include <chrono>
#include <concepts>

enum TimerState
{
	RUNNING = 0,
	STOPPED = 1,
	PAUSED = 2
};

enum DurationFormat
{
	NANOSECONDS = 0,
	MICROSECONDS = 1,
	MILLISECONDS = 2,
	SECONDS = 3,
	MINUTES = 4,
	HOURS = 5,
	DAYS = 6
};

class Timer
{
public:
	Timer();

	void start();
	Timer& stop();
	void restart();
	Timer& reset();

	std::optional<std::chrono::nanoseconds> getTimeDelta(const std::chrono::high_resolution_clock::time_point& start,
														 const std::chrono::high_resolution_clock::time_point& end,
														 const DurationFormat& format) const;

	std::optional<int64_t> getTimerCount(const DurationFormat& format) const;
	std::optional<int64_t> getTimerCount(const std::chrono::high_resolution_clock::time_point& start,
										 const std::chrono::high_resolution_clock::time_point& end,
										 const DurationFormat& format) const;

	std::string durationFormatToStr(const DurationFormat& format) const;
	const Timer& printTime(const DurationFormat& format) const;
	const Timer& printTime(const std::optional<int64_t>& time, const DurationFormat& format) const;

	/**
	* @brief Takes an invocable and duration format, executes the function and prints the time taken for the invocable to execute
	* @param function The invocable
	* @param format The duration format
	**/
	template<typename Func> requires std::invocable<Func>
	void timeIt(Func function, const DurationFormat& format) const
	{
		std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
		function();
		std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();
		std::optional<uint64_t> time = getTimerCount(start, end, format);
		printTime(time, format);
	}

private:
	std::chrono::high_resolution_clock::time_point startTime;
	std::chrono::high_resolution_clock::time_point endTime;
	TimerState state;
};
