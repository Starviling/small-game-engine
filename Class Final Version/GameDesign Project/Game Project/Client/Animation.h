/**
* Animation component of a game object.
* @author Stephen Wang (sywang)
*/
#pragma once
#include "AbstractComponent.h"
#include "AbstractModel.h"
#include "AbstractGameObject.h"
#include "EventManager.h"

class Animation : public AbstractComponent
{
private:
	// The player model to animate
	AbstractModel* model;
	// x Travel (unit vector value multiplied by speed)
	float* xTravel;
	// y Travel (unit vector value multiplied by speed)
	float* yTravel;
	// Untranslated XY coordinate pointer
	sf::Vector2f* untranslatedXY;
	// Translation XY coordinate pointer
	sf::Vector2f* xyTranslation;
	long double* gameTimeFrame;
	StringId objectId;
	StringId testing = internString("player");

public:

	// Constructor for the Animation component
	Animation(AbstractGameObject* object);
	// Generates an animate event and passes it into the event manager
	void run();
	// Called by event handler 
	void runEvent(sf::Vector2f newPosition, sf::Vector2f untranslated);
};

