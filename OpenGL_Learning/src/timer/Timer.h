#pragma once

// Timer class.
class Timer {
	unsigned int endTime;

	// Returns current time in milliseconds
	unsigned int getTime();
public:
	Timer();

	// Sets timer active till endTime in milliseconds
	void setTimer(int endTimeMilliseconds);

	// Returns 'true' if currentTime < endTime. Otherwise returns 'false'
	bool active();

	// Sets renderEnd to 0, to make active() return 'false'
	void clear();
};