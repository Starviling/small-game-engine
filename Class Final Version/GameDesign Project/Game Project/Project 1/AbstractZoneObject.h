/**
* Zone object holds model for zone and defines trigger on target.
* @author Stephen Wang (sywang)
*/
#pragma once
#include "AbstractGameObject.h"
#include "PlatformObject.h"

class AbstractZoneObject : public PlatformObject
{
public:
	// Spawns a platform with no details
	AbstractZoneObject(long double* framePeriod, sf::Vector2f* xyTranslation, std::vector<int>* deleteList);
	// The most simple constructor for the platform object
	AbstractZoneObject(sf::Vector2f start, sf::Color color, long double* framePeriod, long double* gameTimeFrame, bool* focused, sf::Vector2f* xyTranslation,
		std::vector<AbstractGameObject*>* objectList, std::vector<int>* deleteList);
	// Simple constructor for the platform object
	AbstractZoneObject(sf::Vector2f start, sf::Color color, float orientation, long double* framePeriod, long double* gameTimeFrame, bool* focused, sf::Vector2f* xyTranslation,
		std::vector<AbstractGameObject*>* objectList, std::vector<int>* deleteList);
	// Constructor for the platform object
	AbstractZoneObject(sf::Vector2f start, sf::Color color, float orientation, sf::Vector2f end, float speed,
		long double* framePeriod, long double* gameTimeFrame, bool* focused, sf::Vector2f* xyTranslation, std::vector<AbstractGameObject*>* objectList,
		std::vector<int>* deleteList);
	// Complicated constructor for the platform object
	AbstractZoneObject(sf::Vector2f start, sf::Color color, float orientation, sf::Vector2f end, float speed,
		float length, float width, long double* framePeriod, long double* gameTimeFrame, bool* focused, sf::Vector2f* xyTranslation,
		std::vector<AbstractGameObject*>* objectList, std::vector<int>* deleteList);
	virtual void trigger(AbstractGameObject* target, AbstractGameObject* otherTarget) = 0;
};