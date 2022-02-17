#include "DeathZoneObject.h"
#define DEFAULTLENGTH 100.f
#define DEFAULTWIDTH 5.f
#define DEFAULT_ORIENTATION 0.f

DeathZoneObject::DeathZoneObject(long double* gameTimeFrame, sf::Vector2f* xyTranslation, std::vector<int>* deleteList)
	:AbstractZoneObject(gameTimeFrame, xyTranslation, deleteList)
{
	this->getModel()->getShape()->setFillColor(sf::Color(255, 0, 0, 255));
	this->targetPlayerId = internString("player");
	this->objectId = internString("death");
	this->planetId = internString("planet");
	this->spaceId = internString("space");
	this->projectileId = internString("projectile");
	setDeathZoneId(internString("invader"));
}

DeathZoneObject::DeathZoneObject(sf::Vector2f start, sf::Color color, long double* framePeriod, long double* gameTimeFrame, bool* focused,
	sf::Vector2f* xyTranslation, std::vector<AbstractGameObject*>* objectList, std::vector<int>* deleteList)
	: DeathZoneObject(start, color, DEFAULT_ORIENTATION, framePeriod, gameTimeFrame, focused, xyTranslation, objectList, deleteList) {}

DeathZoneObject::DeathZoneObject(sf::Vector2f start, sf::Color color, float orientation, long double* framePeriod, long double* gameTimeFrame,
	bool* focused, sf::Vector2f* xyTranslation, std::vector<AbstractGameObject*>* objectList, std::vector<int>* deleteList)
	: DeathZoneObject(start, color, orientation, start, 0, framePeriod, gameTimeFrame, focused, xyTranslation, objectList, deleteList) {}

DeathZoneObject::DeathZoneObject(sf::Vector2f start, sf::Color color, float orientation, sf::Vector2f end, float speed,
	long double* framePeriod, long double* gameTimeFrame, bool* focused, sf::Vector2f* xyTranslation, std::vector<AbstractGameObject*>* objectList, std::vector<int>* deleteList)
	: DeathZoneObject(start, color, orientation, end, speed, DEFAULTLENGTH, DEFAULTWIDTH, framePeriod, gameTimeFrame, focused, xyTranslation, objectList, deleteList) {}

DeathZoneObject::DeathZoneObject(sf::Vector2f start, sf::Color color, float orientation, sf::Vector2f end, float speed,
	float length, float width, long double* framePeriod, long double* gameTimeFrame, bool* focused, sf::Vector2f* xyTranslation, std::vector<AbstractGameObject*>* objectList, std::vector<int>* deleteList)
	: AbstractZoneObject(start, color, orientation, end, speed, length, width, framePeriod, gameTimeFrame, focused, xyTranslation, objectList, deleteList)
{
	this->targetPlayerId = internString("player");
	this->objectId = internString("death");
	this->planetId = internString("planet");
	this->projectileId = internString("projectile");
	this->spaceId = internString("space");
	setDeathZoneId(internString("invader"));
	setDeleteList(deleteList);
	this->threadUnsafeList.push_back(new Collision(this));
}


void DeathZoneObject::trigger(AbstractGameObject* target, AbstractGameObject* otherTarget)
{
	if (target->getId() == targetPlayerId && getDeathZoneId() != projectileId) {
		PlayerObject* p = dynamic_cast<PlayerObject*> (target);
		p->killPlayer();
	}
	if (target->getId() == objectId && otherTarget->getId() == objectId) {
		DeathZoneObject* t = dynamic_cast<DeathZoneObject*> (target);
		DeathZoneObject* otherT = dynamic_cast<DeathZoneObject*> (otherTarget);
		if (otherT->getDeathZoneId() == projectileId) {
			deleteList->push_back(target->getIndex());
			deleteList->push_back(otherTarget->getIndex());
		}
		else if (otherT->getDeathZoneId() == planetId) {
			deleteList->push_back(target->getIndex());
		}
		else if (otherT->getDeathZoneId() == spaceId) {
			deleteList->push_back(target->getIndex());
		}

	}
}

void DeathZoneObject::setDeathZoneId(StringId deathZoneId)
{
	this->deathZoneId = deathZoneId;
}
