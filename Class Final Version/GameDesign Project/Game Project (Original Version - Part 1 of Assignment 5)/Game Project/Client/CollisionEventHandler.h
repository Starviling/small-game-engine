#pragma once
#include "AbstractEventHandler.h"
#include "Collision.h"
#include "AbstractGameObject.h"
#include "AbstractZoneObject.h"

class CollisionEventHandler : public AbstractEventHandler
{
private:
public:
	// & serves as an alias (modifications to event still noticed)
	virtual void onEvent(Event& event);
};