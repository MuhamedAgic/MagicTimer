#pragma once

#include <iostream>
#include <chrono>
#include <type_traits>
#include <functional>
#include <vector>
#include <algorithm>
#include <expected>
#include <print>

enum TimerState
{
	RUNNING = 0,
	STOPPED = 1
};

enum DurationFormat
{
	NANOSECONDS = 0,
	MICROSECONDS = 1,
	MILLISECONDS = 2,
	SECONDS = 3,
	MINUTES = 4,
	HOURS = 5 // Try this to see what happens with std::expected
};

// type alias for return type of invocable
template<typename Func, typename... Args>
using InvokeResult = decltype(std::invoke(std::declval<Func>(), std::declval<Args>()...));

class Timer
{
public:
	Timer();

	void start();
	Timer& stop();
	void restart();
	Timer& reset();

	const Timer& printTime(const DurationFormat& format) const;

	/**
	* @brief Takes a function with it's arguments, executes it, gives the possible return value and prints the execution time
	* @param func Any callable object (functions, member functions, lambdas, etc.)
	* @param args The function arguments
	* @param format The duration format (seconds, milliseconds, etc.)
	**/
	template<typename Func, typename... Args>
	InvokeResult<Func, Args...> timeIt(const DurationFormat& format, Func&& func, Args&&... args)
	{
		// check returntype of invocable at compile time
		if constexpr (std::is_void_v<decltype(std::invoke(std::forward<Func>(func), std::forward<Args>(args)...))>)
		{
			std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
			std::invoke(std::forward<Func>(func), std::forward<Args>(args)...);
			std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();
			std::expected<double, std::string> time = getTimerCount(start, end, format);
			printTime(time, format);
		}
		else
		{
			std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
			auto result = std::invoke(std::forward<Func>(func), std::forward<Args>(args)...);
			std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();
			std::expected<double, std::string> time = getTimerCount(start, end, format);
			printTime(time, format);
			return result;
		}
	}

	// Specialization for member functions
	template<typename ReturnType, typename ClassType, typename... Args>
	auto timeIt(const DurationFormat& format, ReturnType(ClassType::* func)(Args...), ClassType* obj, Args&&... args)
	{
		// use timeIt function, pass format, labmda where class uses member function and arguments of that member function
		return timeIt(
			format, 
			[func, obj](Args&&... args) -> ReturnType { return (obj->*func)(std::forward<Args>(args)...); },
			std::forward<Args>(args)...
		);
	}

	// Specialization for const member functions
	template<typename ReturnType, typename ClassType, typename... Args>
	auto timeIt(const DurationFormat& format, ReturnType(ClassType::* func)(Args...) const, const ClassType* obj, Args&&... args)
	{
		// use timeIt function, pass format, labmda where class uses member function and arguments of that member function
		return timeIt(
			format, 
			[func, obj](Args&&... args) -> ReturnType { return (obj->*func)(std::forward<Args>(args)...); },
			std::forward<Args>(args)...
		);
	}



private:
	std::chrono::high_resolution_clock::time_point startTime;
	std::chrono::high_resolution_clock::time_point endTime;
	TimerState state;

	std::expected<double, std::string> getTimeDelta(const std::chrono::high_resolution_clock::time_point& start,
									   const std::chrono::high_resolution_clock::time_point& end,
									   const DurationFormat& format) const;

	std::expected<double, std::string> getTimerCount(const DurationFormat& format) const;
	std::expected<double, std::string> getTimerCount(const std::chrono::high_resolution_clock::time_point& start,
										const std::chrono::high_resolution_clock::time_point& end,
										const DurationFormat& format) const;

	std::expected<std::string, std::string> durationFormatToStr(const DurationFormat& format) const;

	void printTime(const std::expected<double, std::string>& time, const DurationFormat& format) const;

};
