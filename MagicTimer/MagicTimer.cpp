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

std::optional<std::chrono::nanoseconds> Timer::getTimeDelta(const std::chrono::high_resolution_clock::time_point& start,
															const std::chrono::high_resolution_clock::time_point& end,
															const DurationFormat& format) const
{
	std::chrono::nanoseconds duration;
	switch (format)
	{
		case NANOSECONDS:
			duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
			break;
		case MICROSECONDS:
			duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
			break;
		case MILLISECONDS:
			duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
			break;
		case SECONDS:
			duration = std::chrono::duration_cast<std::chrono::seconds>(end - start);
			break;
		case MINUTES:
			duration = std::chrono::duration_cast<std::chrono::minutes>(end - start);
			break;
		case HOURS:
			duration = std::chrono::duration_cast<std::chrono::hours>(end - start);
			break;
		case DAYS:
			duration = std::chrono::duration_cast<std::chrono::days>(end - start);
			break;
		default:
			std::cout << "Unable to calculate duration, please provide a valid duration format \n";
			return std::nullopt;
	}
	return duration;
}

std::optional<int64_t> Timer::getTimerCount(const DurationFormat& format) const
{
	std::chrono::high_resolution_clock::time_point now = std::chrono::high_resolution_clock::now();
	std::optional<std::chrono::nanoseconds> duration = getTimeDelta(this->startTime, now, format);
	if (duration.has_value())
	{
		int64_t timeValue = duration.value().count();
		return timeValue;
	}
	return std::nullopt;
}

std::optional<int64_t> Timer::getTimerCount(const std::chrono::high_resolution_clock::time_point& start,
											const std::chrono::high_resolution_clock::time_point& end,
											const DurationFormat& format) const
{
	std::optional<std::chrono::nanoseconds> duration = getTimeDelta(start, end, format);
	if (duration.has_value())
	{
		int64_t timeValue = duration.value().count();
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
		case HOURS:
			durationFormatStr = "hours";
			break;
		case DAYS:
			durationFormatStr = "days";
			break;
		default:
			durationFormatStr = "unknown";
	}
	return durationFormatStr;
}

const Timer& Timer::printTime(const DurationFormat& format) const
{
	std::optional<uint64_t> time = getTimerCount(format);
	if (time.has_value())
	{
		std::cout << "Duration: " << time.value() << " " << durationFormatToStr(format) << "\n";
	}
	else
	{
		std::cout << "Duration unknown \n";
	}
	return *this;
}

const Timer& Timer::printTime(const std::optional<int64_t>& time, const DurationFormat& format) const
{
	if (time.has_value())
	{
		std::cout << "Duration: " << time.value() << " " << durationFormatToStr(format) << "\n";
	}
	else
	{
		std::cout << "Duration unknown \n";
	}
	return *this;
}