/**
* Player control component modifies values in xTravel and yTravel for animation component to animate
* @author Stephen Wang (sywang)
*/
#include "PlayerControl.h"
#define INERTIA 1.25

/**
* Constructor for player control component (Only add to models that are being controlled by the player)
* @param object the pointer to the connect object of the component
*/
PlayerControl::PlayerControl(AbstractGameObject* object)
	:AbstractComponent(object)
{
	this->speed = object->getSpeed();
	this->xTravel = object->getXSpeed();
	this->yTravel = object->getYSpeed();
	this->framePeriod = object->getFramePeriod();
	this->gameTimeFrame = object->getGameTimeFrame();
	this->canJump = object->getCanJump();
	this->focused = object->getFocus();
}

/**
* Runs the component
*/
void PlayerControl::run()
{
	// Store the calculations for travelling
	float xTemp = 0;
	float yTemp = 0;
	// check ahead in direction
	float timedSpeed = *speed * (float)*framePeriod;

	// Only execute if the window is focused
	if (*focused)
	{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		{
			// left key is pressed: move our character
			xTemp += -timedSpeed;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		{
			// right key is pressed: move our character
			xTemp += timedSpeed;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && *canJump)
		{
			*canJump = false;
			// up key is pressed: move our character
			yTemp += -*speed * 5;
		}
		//if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
		//{
		//	// down key is pressed: move our character
		//	yTemp += timedSpeed;
		//}
	}

	/*
	// Set the travel according to calculated values (halving for inertia effect)
	*(this->xTravel) = xTemp + *(this->xTravel) / INERTIA;
	if (yTemp != 0)
	{
		// Set the travel according to calculated values
		*(this->yTravel) = yTemp;
	}
	*/
	Event e = Event(EventStringId::playerObjectInput);
	e.addComponent(EventStringId::componentPointer, this);
	e.addVector(EventStringId::xyInput, sf::Vector2f(xTemp + *(this->xTravel) / INERTIA, yTemp));
	e.timestamp = *gameTimeFrame;
	EventManager::raise(e);
}

void PlayerControl::runEvent(sf::Vector2f xyEvent)
{
	//std::cout << "PlayerControl:" << std::endl;
	//std::cout << "old xtravel ytravel" << std::endl;
	//std::cout << *xTravel << " : " << *yTravel << std::endl;
	//std::cout << "new xtravel ytravel" << std::endl;
	//std::cout << xyEvent.x << " : " << xyEvent.y << std::endl;

	*(this->xTravel) = xyEvent.x;
	if (xyEvent.y != 0)
	{
		// Set the travel according to calculated values
		*(this->yTravel) = xyEvent.y;
	}
}
