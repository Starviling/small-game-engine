#pragma once
#include "AbstractEventHandler.h"
#include "AbstractGameObject.h"
#include "ScriptManager.h"

class ScriptEventHandler : public AbstractEventHandler
{
private:
	// It is possible to add a map here to provide a more flexible implementation, but in the interest of time I won't do that
	StringId eventId;
	// The event script manager
	ScriptManager* eventScripts;
public:
	// & serves as an alias (modifications to event still noticed)
	virtual void onEvent(Event& event);
	void setEventIdCheck(StringId eventId);
	void setScriptManager(ScriptManager* eventScripts);
};