#pragma once
#include "AbstractEventHandler.h"
#include "Animation.h"
#include "AbstractGameObject.h"

class AnimateEventHandler : public AbstractEventHandler
{
private:
public:
	// & serves as an alias (modifications to event still noticed)
	virtual void onEvent(Event& event);
};