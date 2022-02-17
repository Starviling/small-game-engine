/**
* Zone object holds model for zone and defines trigger on target.
* @author Stephen Wang (sywang)
*/
#pragma once
#include "AbstractGameObject.h"
#include "AbstractZoneObject.h"
#include "PlayerObject.h"

class DeathZoneObject : public AbstractZoneObject
{
private:
	StringId targetPlayerId;
	// More specifc ID for object indicating what type of death zone
	StringId deathZoneId;

	// Out of sheer laziness, these are the types of deathZoneIDs
	StringId invaderId;
	StringId planetId;
	StringId projectileId;
	StringId spaceId;

public:
	// Spawns a platform with no details
	DeathZoneObject(long double* gameTimeFrame, sf::Vector2f* xyTranslation, std::vector<int>* deleteList);
	// The most simple constructor for the platform object
	DeathZoneObject(sf::Vector2f start, sf::Color color, long double* framePeriod, long double* gameTimeFrame, bool* focused, sf::Vector2f* xyTranslation,
		std::vector<AbstractGameObject*>* objectList, std::vector<int>* deleteList);
	// Simple constructor for the platform object
	DeathZoneObject(sf::Vector2f start, sf::Color color, float orientation, long double* framePeriod, long double* gameTimeFrame, bool* focused, sf::Vector2f* xyTranslation,
		std::vector<AbstractGameObject*>* objectList, std::vector<int>* deleteList);
	// Constructor for the platform object
	DeathZoneObject(sf::Vector2f start, sf::Color color, float orientation, sf::Vector2f end, float speed,
		long double* framePeriod, long double* gameTimeFrame, bool* focused, sf::Vector2f* xyTranslation, std::vector<AbstractGameObject*>* objectList,
		std::vector<int>* deleteList);
	// Complicated constructor for the platform object
	DeathZoneObject(sf::Vector2f start, sf::Color color, float orientation, sf::Vector2f end, float speed,
		float length, float width, long double* framePeriod, long double* gameTimeFrame, bool* focused, sf::Vector2f* xyTranslation, std::vector<AbstractGameObject*>* objectList,
		std::vector<int>* deleteList);

	virtual void trigger(AbstractGameObject* target, AbstractGameObject* otherTarget);

	void setDeathZoneId(StringId deathZoneId);
	StringId getDeathZoneId() { return deathZoneId; }
};