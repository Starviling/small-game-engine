/**
* @file RealTime.h
* @author Stephen Wang (sywang)
* GameTime timeline object
*/
#pragma once
#include "AbstractTimeline.h"
class RealTime :
	public AbstractTimeline
{
private:
	// Anchors to system lib
	// Last time getDeltaTime() was called in realtime
	long double lastDeltaTimeCall;
public:
	// Constructor for a RealTime timeline object
	RealTime(long double tic);
	RealTime();
	// Returns time of timeline
	virtual long double getTime();
	// Toggles pause for timeline
	virtual void pauseToggle();
	// Sets the tic to a new value given
	virtual void setTic(long double new_tic);
	virtual void speedUp();
	virtual void slowDown();

	long double getDeltaTime();
};
