/**
* The player object. Contains Animation and Collision components.
* @author Stephen Wang (sywang)
*/
#include "PlayerObject.h"
#define DEFAULTSPEED 2

/**
* Player object not controlled by the client
* @param xloc the starting x location of the object
* @param yloc the starting y location of the object
*/
PlayerObject::PlayerObject(float xloc, float yloc, long double* gameTimeFrame, sf::Vector2f* xyTranslation)
	:AbstractGameObject(NULL, gameTimeFrame, NULL, false, xyTranslation, NULL)
{
	this->pModel = new PlayerModel(xloc, yloc);
	this->untranslatedXY.x = xloc;
	this->untranslatedXY.y = yloc;
	this->objectId = internString("player");
	this->canJump = false;
}

/**
* Constructor for the player object (controlled by client)
* @param xloc the starting x location of the player object
* @param yloc the starting y location of the player object
* @param framerPeriod the pointer to the indicator for the frame period
* @param focused the pointer to the indicator of whether the window is focused or not
* @param objectList the list of objects that the player could collide with
*/
PlayerObject::PlayerObject(float xloc, float yloc, long double* framePeriod, long double* gameTimeFrame, bool* focused, std::vector<AbstractGameObject*>* objectList,
	sf::Vector2f* xyTranslation)
	:AbstractGameObject(framePeriod, gameTimeFrame, focused, true, xyTranslation, objectList)
{
	this->pModel = new PlayerModel(xloc, yloc);
	setSpawn(sf::Vector2f(0, 0), sf::Vector2f(0, 0));
	this->untranslatedXY.x = xloc;
	this->untranslatedXY.y = yloc;
	this->setSpeed(DEFAULTSPEED);
	setXYSpeed(0, 0);

	this->objectId = internString("player");

	// Add Player control component to player
	this->threadSafeList.push_back(new PlayerControl(this));
	// Add Gravity component to player
	this->threadUnsafeList.push_back(new Gravity(this));
	// Add Collision component to player before animation
	this->threadUnsafeList.push_back(new Collision(this));
	// Add Animation component to player last
	this->threadUnsafeList.push_back(new Animation(this));
}

PlayerObject::~PlayerObject()
{
	delete pModel;
}

void PlayerObject::setSpawn(sf::Vector2f spawn, sf::Vector2f spawnTranslation)
{
	this->spawnLocation = spawn;
	// Spawn translation returns the screen translation back to before death
	this->spawnTranslation = spawnTranslation;
}

void PlayerObject::killPlayer()
{
	this->xyTranslation->x = spawnTranslation.x;
	this->xyTranslation->y = spawnTranslation.y;
	this->setPosition(spawnLocation);
}

/**
* Sets the speed for the player object (here primarily to work around the dukglue issue with calling derived functions)
* @param newSpeed the new speed being set
*/
void PlayerObject::setSpeed(float newSpeed)
{
	AbstractGameObject::setSpeed(newSpeed);
}

/**
* Returns the json containing updated info on the object
* @return json containing updated info
*/
json PlayerObject::getUpdateInfo()
{
	sf::Vector2f position = pModel->getPosition();
	sf::Vector2f size(pModel->getRadius(), 0);
	sf::Color color = pModel->getFillColor();

	json update = {
		{"id", objectId},
		{"position", {untranslatedXY.x, untranslatedXY.y}},
		{"xyTravel" , {xTravel, yTravel}},
		{"orientation", pModel->getRotation()},
		{"size", {size.x, size.y}},
		{"color", {color.r, color.g, color.b}}
	};
	return update;
}

/**
* Updates the object with the given json information
* @param updateInfo the json containing update information
*/
void PlayerObject::serverUpdate(json updateInfo)
{
	// Update the position
	untranslatedXY.x = updateInfo["position"][0];
	untranslatedXY.y = updateInfo["position"][1];
	pModel->setPosition(untranslatedXY.x + xyTranslation->x, untranslatedXY.y + xyTranslation->y);

	// Update the XY travel
	xTravel = updateInfo["xyTravel"][0];
	yTravel = updateInfo["xyTravel"][1];
	// Update the orientation of the object
	pModel->setRotation(updateInfo["orientation"]);
	// Update the size of the object
	pModel->setRadius(updateInfo["size"][0]);
	// Update the color of the object
	pModel->setFillColor(sf::Color(updateInfo["color"][0], updateInfo["color"][1], updateInfo["color"][2]));
}