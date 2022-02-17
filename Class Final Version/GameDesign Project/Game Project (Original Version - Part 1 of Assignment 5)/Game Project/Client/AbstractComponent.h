/**
* Abstract component of game objects.
* @author Stephen Wang (sywang)
*/
#pragma once
#include "AbstractGameObject.h"

// Forward declaration of the class
class AbstractGameObject;

class AbstractComponent
{
protected:
	// Keeps track of focus of the model
	//bool* focused;
	AbstractGameObject* object;

public:
	AbstractComponent(AbstractGameObject* object);
	// Runs the component's behavior
	virtual void run() = 0;

	// TODO: getType(), isType(), recieveEvent()?
};