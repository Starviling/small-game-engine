/**
* Collision component of a game object - handles collisions
* @author Stephen Wang (sywang)
*/
#pragma once
#include "AbstractComponent.h"
#include "AbstractGameObject.h"
#include "PlayerObject.h"
#include "AbstractZoneObject.h"
#include <vector>
class Collision : public AbstractComponent
{
private:
	std::vector<AbstractGameObject*> *objectList;
	AbstractModel* model;
	float* xTravel;
	float* yTravel;

	long double* gameTimeFrame;
	bool* focused;
	bool* collision;
	// Pointer to boolean indicating whether the object can jump
	bool* canJump;

	StringId playerId;
	StringId deathId;
	StringId spawnId;
	StringId translateId;

public:
	Collision(AbstractGameObject* mainObject);
	void run();
	void runEvent(sf::Vector2f xyEvent, sf::Vector2f oldTravel, bool jumpBoolean);
};