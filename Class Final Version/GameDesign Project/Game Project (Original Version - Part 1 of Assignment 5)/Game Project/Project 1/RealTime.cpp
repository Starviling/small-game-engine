/**
* @file RealTime.cpp
* @author Stephen Wang (sywang)
* GameTime timeline object
*/
#include "RealTime.h"

/**
* Constructor for RealTime
* @param tic the ticks for the object
*/
RealTime::RealTime(long double tic)
	:AbstractTimeline(tic)
{
	// Get steady clock cur time for calculations in milliseconds
	auto now = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now());
	auto sysTime = now.time_since_epoch();

	this->start_time = (long double) sysTime.count();
	this->last_paused_time = start_time;
	this->lastDeltaTimeCall = getTime();
}

/**
* Default constructor for RealTime - utilizes 1000 as default tick value
*/
RealTime::RealTime() :
	RealTime(10) {}

/**
* Returns the time for the GameTime object
* @return the time for the GameTime object as a long double
*/
long double RealTime::getTime()
{
	long double curTime;
	if (!paused) {
		// If not paused, use the current time
		auto now = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now());
		auto sysTime = now.time_since_epoch();
		curTime = (long double) sysTime.count();
	}
	else {
		// If paused, use the time paused
		curTime = last_paused_time;
	}
	long double ellapsed = curTime - start_time - ellapsed_pause_time;
	return ellapsed / tic;
}

/**
* Toggles the pause for the game
*/
void RealTime::pauseToggle()
{
	this->paused = !paused;

	auto now = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now());
	auto sysTime = now.time_since_epoch();
	long double newTime = (long double)sysTime.count();
	if (!paused) {
		this->ellapsed_pause_time += newTime - last_paused_time;
	}
	this->last_paused_time = newTime;
}

/**
* Sets the tic to a new value
* @param new_tic new tic value being set
*/
void RealTime::setTic(long double new_tic)
{
	if (new_tic > 0) {
		this->lastDeltaTimeCall *= tic / new_tic;
		this->tic = new_tic;
	}
}

/**
* Speeds the game by adjusting tic to be smaller
*/
void RealTime::speedUp() {
	setTic(this->tic / 2);
}

/**
* Slows the game by adjusting tic to be bigger
*/
void RealTime::slowDown() {
	setTic(this->tic * 2);
}

/**
* Calculates the deltatime based on last call to function.
* @return the time since the last call to getDeltaTime function
*/
long double RealTime::getDeltaTime()
{
	long double now = getTime();
	long double deltaTime = now - lastDeltaTimeCall;
	this->lastDeltaTimeCall = now;
	return deltaTime;
}
