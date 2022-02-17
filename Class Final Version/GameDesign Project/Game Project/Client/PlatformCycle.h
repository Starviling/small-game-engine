/**
* Platform cycle updates the xTravel and yTravel based on framePeriod
* @author Stephen Wang (sywang)
*/
#pragma once
#include "AbstractComponent.h"
#include "AbstractGameObject.h"
#include <math.h>
#include <SFML\System\Vector2.hpp>
#include <cmath>

class PlatformCycle : public AbstractComponent
{
private:
	// Starting location for the platform
	float xlocS;
	float ylocS;
	// Ending location for the platform
	float xlocE;
	float ylocE;

	// End - start
	float diffX;
	float diffY;
	// Unit vector form of the end - start
	float unitX;
	float unitY;

	// The speed of the platform moving (used by the animation component)
	float* speed;
	// x Travel (unit vector value multiplied by speed)
	float* xTravel;
	// y Travel (unit vector value multiplied by speed)
	float* yTravel;
	// Keeps track of time
	long double* framePeriod;

	// The duration of half a cycle
	float cycle;
	float currentCycle;

	StringId deathZoneId;
	StringId projectileId;

public:
	PlatformCycle(AbstractGameObject* object, sf::Vector2f start, sf::Vector2f end);
	// Sets the starting location of the platform cycle
	void setLocations(sf::Vector2f start, sf::Vector2f end);
	// Sets the cycle animation
	void setCycle();

	// Returns the start location
	sf::Vector2f getStartLoc() { return sf::Vector2f(xlocS, ylocS); }
	// Returns the end location
	sf::Vector2f getEndLoc() { return sf::Vector2f(xlocE, ylocE); };
	// Runs the component
	void run();
};

