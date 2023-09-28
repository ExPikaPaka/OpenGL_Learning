#include "Timer.h"
#include <chrono>

unsigned int Timer::getTime() {
	return static_cast<unsigned int>(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count());
}

Timer::Timer() {
	endTime = 0;
}

void Timer::setTimer(int endTimeMilliseconds) {
	endTime = getTime() + endTimeMilliseconds;

}

bool Timer::active() {
	return getTime() < endTime && endTime;
}

void Timer::clear() {
	endTime = 0;
}

