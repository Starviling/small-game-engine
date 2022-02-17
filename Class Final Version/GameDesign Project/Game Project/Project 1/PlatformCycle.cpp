/**
* Platform cycle updates the xTravel and yTravel based on framePeriod
* @author Stephen Wang (sywang)
*/
#include "PlatformCycle.h"
#include "DeathZoneObject.h"
#define SPACEINVADERDROP 15
#define PROJECTILESPEED -5

/**
* Constructor for the PlatformCycle
* @param object the pointer to the object this component is connected to
* @param start the starting position of the platform cycle
* @param end the ending position of the platform cycle
*/
PlatformCycle::PlatformCycle(AbstractGameObject* object, sf::Vector2f start, sf::Vector2f end)
	:AbstractComponent(object)
{
	this->speed = object->getSpeed();
	this->xTravel = object->getXSpeed();
	this->yTravel = object->getYSpeed();
	this->framePeriod = object->getFramePeriod();
	setLocations(start, end);
	setCycle();
	this->deathZoneId = internString("death");
	this->projectileId = internString("projectile");
}

/**
* Sets the locations for the start and end of animation cycle. Also
* calculates the unit vector.
*/
void PlatformCycle::setLocations(sf::Vector2f start, sf::Vector2f end)
{
	// Set the starting locations
	this->xlocS = start.x;
	this->ylocS = start.y;
	this->xlocE = end.x;
	this->ylocE = end.y;

	// Calculate the "vector"
	this->diffX = xlocE - xlocS;
	this->diffY = ylocE - ylocS;
	// Set the difference unit vector
	float magnitude = (sqrt(pow(diffX, 2) + pow(diffY, 2)));
	if (magnitude == 0)
	{
		this->unitX = 0;
		this->unitY = 0;
	}
	else {
		this->unitX = diffX / magnitude;
		this->unitY = diffY / magnitude;
	}


}

/**
* Sets the cycle calcuations
*/
void PlatformCycle::setCycle()
{
	if (unitX == 0 && unitY == 0)
	{
		this->cycle = 0;
		this->currentCycle = 0;
	}
	else {
		this->cycle = (diffX / *speed) + (diffY / *speed);
		this->currentCycle = 0;
	}
}

/**
* Runs the cycle to update the xTravel and yTravel variables
*/
void PlatformCycle::run()
{
	if (cycle != 0) {
		*(this->xTravel) = 0;
		*(this->yTravel) = 0;

		float yChange = 0;
		if (currentCycle > cycle)
		{
			float excessMovement = currentCycle - cycle;
			this->currentCycle = 0;
			this->unitX *= -1;
			this->unitY *= -1;
			*(this->xTravel) += excessMovement * unitX;
			*(this->yTravel) += excessMovement * unitY + SPACEINVADERDROP;
			yChange = SPACEINVADERDROP;
		}
		*(this->xTravel) += unitX * *speed * (float)*framePeriod;
		*(this->yTravel) += unitY * *speed * (float)*framePeriod;


		this->currentCycle += std::abs(*xTravel + *yTravel - yChange);


	}
	else if (object->getId() == deathZoneId) {
		DeathZoneObject* d = dynamic_cast<DeathZoneObject*> (object);
		if (d->getDeathZoneId() == projectileId) {
			*(this->yTravel) = PROJECTILESPEED;
		}
	}
}
