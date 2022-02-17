/**
* Gravity component of a game object - handles gravity calculations
* @author Stephen Wang (sywang)
*/
#include "Gravity.h"
#define DEFAULTGRAVITY 0.25
#define MAXVELOCITY 10

/**
* Constructor for gravity sets default gravity
* @param object the pointer to the object this component is connected to
*/
Gravity::Gravity(AbstractGameObject* object)
	:AbstractComponent(object)
{
	this->xTravel = object->getXSpeed();
	this->yTravel = object->getYSpeed();
	this->framePeriod = object->getFramePeriod();
	this->gameTimeFrame = object->getGameTimeFrame();
	this->canJump = object->getCanJump();
	setGravity(DEFAULTGRAVITY);
}

/**
* Sets the gravity to the given value
* @param gravity the new value of gravity to be set
*/
void Gravity::setGravity(float gravity)
{
	this->gravity = gravity;
}

/**
* Runs the component
*/
void Gravity::run()
{
	float temp = *(this->yTravel);
	if (*(this->yTravel) < MAXVELOCITY * (float)*framePeriod)
	{
		//*(this->yTravel) += gravity * (float)*framePeriod;
		temp += gravity * (float)*framePeriod;
	}
	else {
		//*(this->yTravel) = MAXVELOCITY * (float)*framePeriod;
		temp = MAXVELOCITY * (float)*framePeriod;
	}
	Event e = Event(EventStringId::gravityInfluence);
	e.addComponent(EventStringId::componentPointer, this);
	e.addFloat(EventStringId::gravityYTravel, temp);
	e.timestamp = *gameTimeFrame;
	EventManager::raise(e);

	//Not a perfect way of handling pause, but works well enough for now (will interrupt mid-jump)
	//if (*framePeriod != 0) {
	//	if (*(this->yTravel) < MAXVELOCITY * (float)*framePeriod && !(*canJump))
	//	{
	//		*(this->yTravel) += gravity * (float)*framePeriod;
	//	}
	//	else if (*canJump) {
	//		*(this->yTravel) = MAXVELOCITY * (float)*framePeriod;
	//	}
	//}
	//else {
	//	*(this->yTravel) = 0;
	//}
}

void Gravity::runEvent(float ySpeed)
{
	*(this->yTravel) = ySpeed;
}
