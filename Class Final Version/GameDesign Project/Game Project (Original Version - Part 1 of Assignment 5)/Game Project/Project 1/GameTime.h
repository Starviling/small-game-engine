/**
* @file GameTime.h
* @author Stephen Wang (sywang)
* GameTime timeline object
*/
#pragma once
#include "AbstractTimeline.h"
class GameTime :
    public AbstractTimeline
{
private:
    // Anchored timeline
    AbstractTimeline* anchor;
	long double anchorTic;

	// Last time getDeltaTime() was called in gametime
	long double lastDeltaTimeCall;
	long double lastDeltaTime;

public:
	// Constructor for a GameTime timeline object
    GameTime(AbstractTimeline* anchor, long double tic);
	GameTime(AbstractTimeline* anchor);
	// Returns time of timeline
	virtual long double getTime();
	// Toggles pause for timeline
	virtual void pauseToggle();
	// Sets the tic to a new value given
	virtual void setTic(long double new_tic);
	// Checks for change in the anchor tic
	void checkAnchorTic();

	virtual void speedUp();
	virtual void slowDown();

	long double getDeltaTime();

	// Returns whether or not the timeline is paused
	bool getPaused() { return paused; }
};

