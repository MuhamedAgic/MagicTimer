#include "MagicTimer.h"

Timer::Timer()
{
	reset();
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
		std::cout << "Timer was not running, end time set to now \n";
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
	this->endTime = this->startTime;
	this->state = TimerState::STOPPED;
	return *this;
}

std::expected<double, std::string> Timer::getTimeDelta(const std::chrono::high_resolution_clock::time_point& start,
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
			return std::unexpected("Unable to calculate duration, please provide a valid duration format \n");
	}
	return duration;
}

std::expected<double, std::string> Timer::getTimerCount(const DurationFormat& format) const
{
	std::chrono::high_resolution_clock::time_point now = std::chrono::high_resolution_clock::now();
	std::expected<double, std::string> timerCount = getTimerCount(this->startTime, now, format);
	return timerCount;
}

std::expected<double, std::string> Timer::getTimerCount(const std::chrono::high_resolution_clock::time_point& start,
										   const std::chrono::high_resolution_clock::time_point& end,
										   const DurationFormat& format) const
{
	std::expected<double, std::string> duration = getTimeDelta(start, end, format);
	if (duration.has_value())
	{
		double timeValue = duration.value();
		return timeValue;
	}
	return std::unexpected("Error in calculating time!");
}

std::expected<std::string, std::string> Timer::durationFormatToStr(const DurationFormat& format) const
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
			return std::unexpected("Unexpected DurationFormat Error");
	}
	return durationFormatStr;
}

const Timer& Timer::printTime(const DurationFormat& format) const
{
	std::expected<double, std::string> time = getTimerCount(format);
	printTime(time, format);
	return *this;
}

void Timer::printTime(const std::expected<double, std::string>& time, const DurationFormat& format) const
{
	if (!time.has_value())
	{
		std::cout << "Unable to print elapsed time. Error " << time.error();
		return;
	}

	std::expected<std::string, std::string> durationFormat = durationFormatToStr(format);
	if (!durationFormat.has_value())
	{
		std::cout << "Unable to print elapsed time. Error: " << durationFormat.error();
	}
	std::cout << "Duration: " << time.value() << " " << * durationFormat << "\n";
}