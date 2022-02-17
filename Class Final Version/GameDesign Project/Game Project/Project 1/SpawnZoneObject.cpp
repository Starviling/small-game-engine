#include "SpawnZoneObject.h"
#define DEFAULTLENGTH 100.f
#define DEFAULTWIDTH 5.f
#define DEFAULT_ORIENTATION 0.f

SpawnZoneObject::SpawnZoneObject(long double* gameTimeFrame, sf::Vector2f* xyTranslation, std::vector<int>* deleteList)
	:AbstractZoneObject(gameTimeFrame, xyTranslation, deleteList)
{
	this->getModel()->getShape()->setFillColor(sf::Color(0, 0, 0, 0));
	this->targetPlayerId = internString("player");
	this->objectId = internString("spawn");
	this->spawnXYTranslation = sf::Vector2f(*xyTranslation);
}

SpawnZoneObject::SpawnZoneObject(sf::Vector2f start, sf::Color color, long double* framePeriod, long double* gameTimeFrame, bool* focused,
	sf::Vector2f* xyTranslation, std::vector<AbstractGameObject*>* objectList, std::vector<int>* deleteList)
	: SpawnZoneObject(start, color, DEFAULT_ORIENTATION, framePeriod, gameTimeFrame, focused, xyTranslation, objectList, deleteList) {}

SpawnZoneObject::SpawnZoneObject(sf::Vector2f start, sf::Color color, float orientation, long double* framePeriod, long double* gameTimeFrame,
	bool* focused, sf::Vector2f* xyTranslation, std::vector<AbstractGameObject*>* objectList, std::vector<int>* deleteList)
	: SpawnZoneObject(start, color, orientation, start, 0, framePeriod, gameTimeFrame, focused, xyTranslation, objectList, deleteList) {}

SpawnZoneObject::SpawnZoneObject(sf::Vector2f start, sf::Color color, float orientation, sf::Vector2f end, float speed,
	long double* framePeriod, long double* gameTimeFrame, bool* focused, sf::Vector2f* xyTranslation, std::vector<AbstractGameObject*>* objectList,
	std::vector<int>* deleteList)
	: SpawnZoneObject(start, color, orientation, end, speed, DEFAULTLENGTH, DEFAULTWIDTH, framePeriod, gameTimeFrame, focused, xyTranslation, objectList, deleteList) {}

SpawnZoneObject::SpawnZoneObject(sf::Vector2f start, sf::Color color, float orientation, sf::Vector2f end, float speed,
	float length, float width, long double* framePeriod, long double* gameTimeFrame, bool* focused, sf::Vector2f* xyTranslation, std::vector<AbstractGameObject*>* objectList,
	std::vector<int>* deleteList)
	: AbstractZoneObject(start, color, orientation, end, speed, length, width, framePeriod, gameTimeFrame, focused, xyTranslation, objectList, deleteList)
{
	this->targetPlayerId = internString("player");
	this->objectId = internString("spawn");
	this->spawnXYTranslation = sf::Vector2f(*xyTranslation);
}


void SpawnZoneObject::trigger(AbstractGameObject* target, AbstractGameObject* otherTarget)
{
	if (target->getId() == targetPlayerId) {
		PlayerObject* p = dynamic_cast<PlayerObject*> (target);
		this->spawnXYTranslation = sf::Vector2f(*xyTranslation);
		p->setSpawn(this->getModel()->getShape()->getPosition(), spawnXYTranslation);
	}
}
