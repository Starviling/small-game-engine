/**
* @file AbstractTimeline.cpp
* @author Stephen Wang (sywang)
* Abstract class for handling timeline creation
*/
#pragma once
#include <mutex>

class AbstractTimeline {
protected:
	// The time of the anchor at start
	long double start_time;
	// The step size
	long double tic;
	// Indicator for whether Timeline is paused
	bool paused;
	// Last pause for anchored time
	long double last_paused_time;
	// How much anchor time this timeline has been paused
	long double ellapsed_pause_time;

	// Mutex - incase needed
	std::mutex _mutex;

public:
	// Constructor for timeline
	AbstractTimeline(long double tic) {

		this->paused = false;
		this->tic = tic;
		this->ellapsed_pause_time = 0;
		this->start_time = 0;
		this->last_paused_time = 0;
	}
	// Returns time of timeline
	virtual long double getTime() = 0;
	// Toggles pause for timeline
	virtual void pauseToggle() = 0;
	// Sets the tic to a new value given
	virtual void setTic(long double new_tic) = 0;
	// Long double returns the tic of the Timeline
	long double getTic() { return tic; }

	virtual void speedUp() = 0;
	virtual void slowDown() = 0;
};