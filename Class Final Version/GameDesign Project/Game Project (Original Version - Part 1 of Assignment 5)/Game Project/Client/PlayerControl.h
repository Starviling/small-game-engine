/**
* Component adjusts the xTravel and yTravel of player based on user input
* @author Stephen Wang (sywang)
*/
#pragma once
#include "AbstractComponent.h"
#include "AbstractGameObject.h"
#include "EventManager.h"
#include <SFML\Window\Keyboard.hpp>

class PlayerControl : public AbstractComponent
{
private:
	// The speed of the player moving (used by the animation component)
	float* speed;
	// x Travel (unit vector value multiplied by speed)
	float* xTravel;
	// y Travel (unit vector value multiplied by speed)
	float* yTravel;

	// Keeps track of time
	long double* framePeriod;
	long double* gameTimeFrame;
	// Pointer to boolean indicating whether player can jump
	bool* canJump;
	bool* focused;

public:
	// Constructor for the player object
	PlayerControl(AbstractGameObject* object);
	void run();
	void runEvent(sf::Vector2f xyEvent);
};

