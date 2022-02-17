/**
* Abstract game object representation. Holds pointers to frameperiod and focused
* @author Stephen Wang (sywang)
*/
#include "AbstractGameObject.h"

/**
* Constructor for the abstract game object
* @param framePeriod the pointer to the frameperiod
* @Param focused the pointer to the indicator whether the window is focused
* @param collision the boolean indicating whether the object will "collide" with another
* @param xyTranslation a pointer to the vector holding translation data
* @param objectList the pointer to all other interactable objects
*/
AbstractGameObject::AbstractGameObject(long double* framePeriod, long double* gameTimeFrame, bool* focused, bool collision, sf::Vector2f* xyTranslation,
	std::vector<AbstractGameObject*>* objectList)
{
	this->framePeriod = framePeriod;
	this->gameTimeFrame = gameTimeFrame;
	this->focused = focused;
	this->collisionToggle = collision;
	this->untranslatedXY = sf::Vector2f(0, 0);
	this->xyTranslation = xyTranslation;
	this->objectList = objectList;
	setSpeed(0);
	setXYSpeed(0, 0);
}

AbstractGameObject::~AbstractGameObject()
{
	for (AbstractComponent* c : threadSafeList) {
		delete c;
	}
	for (AbstractComponent* c : threadUnsafeList) {
		delete c;
	}
}

/**
* Runs all of the thread safe components
*/
void AbstractGameObject::runThreadSafeComp()
{
	for (AbstractComponent* c : this->threadSafeList) {
		c->run();
	}
}

/**
* Runs all of the thread unsafe components
*/
void AbstractGameObject::runThreadUnsafeComp()
{
	for (AbstractComponent* c : this->threadUnsafeList) {
		c->run();
	}
}

/**
* Sets the speed of the object
* @param newSpeed the new speed set to the object
*/
void AbstractGameObject::setSpeed(float newSpeed)
{
	this->speed = newSpeed;
}

/**
* Sets the XY speed of the object
* @param xSpeed the new x speed of the object
* @param ySpeed the new y speed of the object
*/
void AbstractGameObject::setXYSpeed(float xSpeed, float ySpeed)
{
	this->xTravel = xSpeed;
	this->yTravel = ySpeed;
}

/**
* Toggles whether or not the object can collide with other objects
*/
void AbstractGameObject::toggleCollision()
{
	this->collisionToggle = !collisionToggle;
}

/**
* Sets the position of the object to the new location
* @param position the vector position to be set
*/
void AbstractGameObject::setPosition(sf::Vector2f position)
{
	this->untranslatedXY.x = position.x + xyTranslation->x;
	this->untranslatedXY.y = position.y + xyTranslation->y;
	this->getModel()->getShape()->setPosition(untranslatedXY.x, untranslatedXY.y);
}
