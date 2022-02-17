/**
* Zone object holds model for zone and defines trigger on target.
* @author Stephen Wang (sywang)
*/
#pragma once
#include "AbstractGameObject.h"
#include "AbstractZoneObject.h"
#include "PlayerObject.h"

class TranslateZoneObject : public AbstractZoneObject
{
private:
	StringId targetPlayerId;
	int translateX;
	int translateY;

public:
	// Spawns a zone with no details
	TranslateZoneObject(long double* gameTimeFrame, sf::Vector2f* xyTranslation, std::vector<int>* deleteList);
	TranslateZoneObject(long double* gameTimeFrame, sf::Vector2f* xyTranslation, int translateX, int translateY, std::vector<int>* deleteList);
	// The most simple constructor for the zone object
	TranslateZoneObject(sf::Vector2f start, sf::Color color, long double* framePeriod, long double* gameTimeFrame, bool* focused, sf::Vector2f* xyTranslation,
		std::vector<AbstractGameObject*>* objectList, int translateX, int translateY, std::vector<int>* deleteList);

	virtual void trigger(AbstractGameObject* target, AbstractGameObject* otherTarget);
	void translateObjects(int x, int y);

	// Generates information about game object for server to pass to clients
	virtual json getUpdateInfo();
	// Retrieves information from server update and updates object
	virtual void serverUpdate(json updateInfo);
};