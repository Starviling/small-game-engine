/**
* Gravity component of a game object - handles gravity calculations
* @author Stephen Wang (sywang)
*/
#pragma once
#include "AbstractComponent.h"
#include "AbstractGameObject.h"
#include "EventManager.h"

class Gravity : public AbstractComponent
{
private:
	// Pointers to speeds
	float* xTravel;
	float* yTravel;

	// Pointer to frame period
	long double* framePeriod;
	long double* gameTimeFrame;
	// The gravity (acceleration downwards discounting mass)
	float gravity;
	// Indicator for whether the object can jump
	bool* canJump;

public:
	// Constructs gravity with default gravity value
	Gravity(AbstractGameObject* object);

	// Sets the gravity value
	void setGravity(float gravity);
	// Retrieves the gravity value
	float getGravity() { return gravity; }
	void run();
	void runEvent(float ySpeed);
};

