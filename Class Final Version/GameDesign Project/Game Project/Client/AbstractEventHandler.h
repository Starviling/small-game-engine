/**
* Abstract event handler class
* @author Stephen Wang (sywang)
*/
#pragma once
#include "Events.h"

class AbstractEventHandler
{
public:
	virtual void onEvent(Event& event) = 0;
};