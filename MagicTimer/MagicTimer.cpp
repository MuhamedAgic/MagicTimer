#include "MagicTimer.h"

Timer::Timer()
{
	this->state = TimerState::STOPPED;
}

void Timer::start()
{
	if (this->state == TimerState::RUNNING)
	{
		std::cout << "Timer already running! Stop the current run before starting a new one \n";
		return;
	}
	this->state = TimerState::RUNNING;
	this->startTime = std::chrono::high_resolution_clock::now();
}

Timer& Timer::stop()
{
	this->endTime = std::chrono::high_resolution_clock::now();
	if (this->state != TimerState::RUNNING)
	{
		std::cout << "Timer not running \n";
	}
	this->state = TimerState::STOPPED;
	return *this;
}

void Timer::restart()
{
	stop();
	start();
}

Timer& Timer::reset()
{
	this->startTime = std::chrono::high_resolution_clock::now();
	this->endTime = startTime;
	this->state = TimerState::STOPPED;
	return *this;
}

std::optional<double> Timer::getTimeDelta(const std::chrono::high_resolution_clock::time_point& start,
										  const std::chrono::high_resolution_clock::time_point& end,
										  const DurationFormat& format) const
{
	double duration = (double)std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
	switch (format)
	{
		case NANOSECONDS:
			break;
		case MICROSECONDS:
			duration /= 1000;
			break;
		case MILLISECONDS:
			duration /= 1000000;
			break;
		case SECONDS:
			duration /= 1000000000;
			break;
		case MINUTES:
			duration = (double)std::chrono::duration_cast<std::chrono::seconds>(end - start).count();
			duration /= 60;
			break;
		default:
			std::cout << "Unable to calculate duration, please provide a valid duration format \n";
			return std::nullopt;
	}
	return duration;
}

std::optional<double> Timer::getTimerCount(const DurationFormat& format) const
{
	std::chrono::high_resolution_clock::time_point now = std::chrono::high_resolution_clock::now();
	std::optional<double> timerCount = getTimerCount(this->startTime, now, format);
	return timerCount;
}

std::optional<double> Timer::getTimerCount(const std::chrono::high_resolution_clock::time_point& start,
										   const std::chrono::high_resolution_clock::time_point& end,
										   const DurationFormat& format) const
{
	std::optional<double> duration = getTimeDelta(start, end, format);
	if (duration.has_value())
	{
		double timeValue = duration.value();
		return timeValue;
	}
	return std::nullopt;
}

std::string Timer::durationFormatToStr(const DurationFormat& format) const
{
	std::string durationFormatStr = "";
	switch (format)
	{
		case NANOSECONDS:
			durationFormatStr = "nanoseconds";
			break;
		case MICROSECONDS:
			durationFormatStr = "microseconds";
			break;
		case MILLISECONDS:
			durationFormatStr = "milliseconds";
			break;
		case SECONDS:
			durationFormatStr = "seconds";
			break;
		case MINUTES:
			durationFormatStr = "minutes";
			break;
		default:
			durationFormatStr = "unknown";
	}
	return durationFormatStr;
}

const Timer& Timer::printTime(const DurationFormat& format) const
{
	std::optional<double> time = getTimerCount(format);
	printTime(time, format);
	return *this;
}

void Timer::printTime(const std::optional<double>& time, const DurationFormat& format) const
{
	if (time.has_value())
	{
		std::cout << "Duration: " << time.value() << " " << durationFormatToStr(format) << "\n";
	}
	else
	{
		std::cout << "Duration unknown \n";
	}
}