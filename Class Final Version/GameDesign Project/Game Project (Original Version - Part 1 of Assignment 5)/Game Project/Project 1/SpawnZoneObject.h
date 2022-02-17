/**
* Zone object holds model for zone and defines trigger on target.
* @author Stephen Wang (sywang)
*/
#pragma once
#include "AbstractGameObject.h"
#include "AbstractZoneObject.h"
#include "PlayerObject.h"

class SpawnZoneObject : public AbstractZoneObject
{
private:
	StringId targetPlayerId;
	sf::Vector2f spawnXYTranslation;

public:
	// Spawns a platform with no details
	SpawnZoneObject(long double* gameTimeFrame, sf::Vector2f* xyTranslation);
	// The most simple constructor for the platform object
	SpawnZoneObject(sf::Vector2f start, sf::Color color, long double* framePeriod, long double* gameTimeFrame, bool* focused, sf::Vector2f* xyTranslation,
		std::vector<AbstractGameObject*>* objectList);
	// Simple constructor for the platform object
	SpawnZoneObject(sf::Vector2f start, sf::Color color, float orientation, long double* framePeriod, long double* gameTimeFrame, bool* focused, sf::Vector2f* xyTranslation,
		std::vector<AbstractGameObject*>* objectList);
	// Constructor for the platform object
	SpawnZoneObject(sf::Vector2f start, sf::Color color, float orientation, sf::Vector2f end, float speed,
		long double* framePeriod, long double* gameTimeFrame, bool* focused, sf::Vector2f* xyTranslation, std::vector<AbstractGameObject*>* objectList);
	// Complicated constructor for the platform object
	SpawnZoneObject(sf::Vector2f start, sf::Color color, float orientation, sf::Vector2f end, float speed,
		float length, float width, long double* framePeriod, long double* gameTimeFrame, bool* focused, sf::Vector2f* xyTranslation, std::vector<AbstractGameObject*>* objectList);

	virtual void trigger(AbstractGameObject* target);
};