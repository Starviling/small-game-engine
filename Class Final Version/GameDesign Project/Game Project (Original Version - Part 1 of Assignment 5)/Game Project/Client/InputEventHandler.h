#pragma once
#include "AbstractEventHandler.h"
#include "PlayerControl.h"
#include "Gravity.h"

class InputEventHandler : public AbstractEventHandler
{
private:

public:
	// & serves as an alias (modifications to event still noticed)
	virtual void onEvent(Event& event);
};