/**
* @file GameTime.cpp
* @author Stephen Wang (sywang)
* GameTime timeline object
*/
#include "GameTime.h"

/**
* Constructor for GameTime
* @param anchor the timeline to anchor to
* @param tic the tick size for object
*/
GameTime::GameTime(AbstractTimeline* anchor, long double tic)
	:AbstractTimeline(tic)
{
	this->anchor = anchor;
	this->anchorTic = anchor->getTic();
	this->start_time = anchor->getTime();
	this->last_paused_time = start_time;
	this->lastDeltaTimeCall = getTime();
	this->lastDeltaTime = 0;
}

/**
* Constructor for GameTime
* @param anchor the timeline to anchor to
*/
GameTime::GameTime(AbstractTimeline* anchor) :
	GameTime(anchor, 1) {}

/**
* Returns the time for the GameTime object
* @return the time for the GameTime object as a long double
*/
long double GameTime::getTime()
{
	checkAnchorTic();
	long double curTime;
	if (!paused) {
		// If not paused, use the current time
		curTime = anchor->getTime();
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
void GameTime::pauseToggle()
{
	this->paused = !paused;
	checkAnchorTic();
	long double newTime = anchor->getTime();

	if (!paused) {
		this->ellapsed_pause_time += newTime - last_paused_time;
	}
	this->last_paused_time = newTime;
}

/**
* Sets the tic to a new value
* @param new_tic new tic value being set
*/
void GameTime::setTic(long double new_tic)
{
	if (new_tic > 0) {
		this->lastDeltaTimeCall *= tic / new_tic;
		this->tic = new_tic;
	}
}

/**
* Checks to see if the anchor tic has changed - adjusts
* Time object accordingly
*/
void GameTime::checkAnchorTic()
{
	long double checkTic = anchor->getTic();
	if (anchorTic != checkTic) {
		this->last_paused_time *= anchorTic / checkTic;
		this->start_time *= anchorTic / checkTic;
		this->ellapsed_pause_time *= anchorTic / checkTic;
		this->anchorTic = checkTic;
	}
}

/**
* Speeds the game by adjusting tic to be smaller
*/
void GameTime::speedUp() {
	setTic(this->tic / 2);
}

/**
* Slows the game by adjusting tic to be bigger
*/
void GameTime::slowDown() {
	setTic(this->tic * 2);
}

/**
* Calculates the deltatime based on last call to function in terms of gametime.
* @return the time since the last call to getDeltaTime function
*/
long double GameTime::getDeltaTime()
{
	long double now = getTime();
	long double deltaTime = now - lastDeltaTimeCall;
	lastDeltaTime = deltaTime;
	this->lastDeltaTimeCall = now;
	return deltaTime;
}
