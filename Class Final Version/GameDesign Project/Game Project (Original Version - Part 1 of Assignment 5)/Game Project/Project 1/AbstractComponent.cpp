/**
* Abstract component for game objects
* @author Stephen Wang (sywang)
*/
#include "AbstractComponent.h"

AbstractComponent::AbstractComponent(AbstractGameObject* object)
{
	//this->framePeriod = framePeriod;
	this->object = object;
}
