#include "DeathZoneObject.h"
#define DEFAULTLENGTH 100.f
#define DEFAULTWIDTH 5.f
#define DEFAULT_ORIENTATION 0.f

DeathZoneObject::DeathZoneObject(long double* gameTimeFrame, sf::Vector2f* xyTranslation)
	:AbstractZoneObject(gameTimeFrame, xyTranslation)
{
	this->getModel()->getShape()->setFillColor(sf::Color(0, 0, 0, 0));
	this->targetPlayerId = internString("player");
	this->objectId = internString("death");
}

DeathZoneObject::DeathZoneObject(sf::Vector2f start, sf::Color color, long double* framePeriod, long double* gameTimeFrame, bool* focused,
	sf::Vector2f* xyTranslation, std::vector<AbstractGameObject*>* objectList)
	: DeathZoneObject(start, color, DEFAULT_ORIENTATION, framePeriod, gameTimeFrame, focused, xyTranslation, objectList) {}

DeathZoneObject::DeathZoneObject(sf::Vector2f start, sf::Color color, float orientation, long double* framePeriod, long double* gameTimeFrame,
	bool* focused, sf::Vector2f* xyTranslation, std::vector<AbstractGameObject*>* objectList)
	: DeathZoneObject(start, color, orientation, start, 0, framePeriod, gameTimeFrame, focused, xyTranslation, objectList) {}

DeathZoneObject::DeathZoneObject(sf::Vector2f start, sf::Color color, float orientation, sf::Vector2f end, float speed,
	long double* framePeriod, long double* gameTimeFrame, bool* focused, sf::Vector2f* xyTranslation, std::vector<AbstractGameObject*>* objectList)
	: DeathZoneObject(start, color, orientation, end, speed, DEFAULTLENGTH, DEFAULTWIDTH, framePeriod, gameTimeFrame, focused, xyTranslation, objectList) {}

DeathZoneObject::DeathZoneObject(sf::Vector2f start, sf::Color color, float orientation, sf::Vector2f end, float speed,
	float length, float width, long double* framePeriod, long double* gameTimeFrame, bool* focused, sf::Vector2f* xyTranslation, std::vector<AbstractGameObject*>* objectList)
	: AbstractZoneObject(start, color, orientation, end, speed, length, width, framePeriod, gameTimeFrame, focused, xyTranslation, objectList)
{
	this->targetPlayerId = internString("player");
	this->objectId = internString("death");
}


void DeathZoneObject::trigger(AbstractGameObject* target)
{
	if (target->getId() == targetPlayerId) {
		PlayerObject* p = dynamic_cast<PlayerObject*> (target);
		p->killPlayer();
	}
}
