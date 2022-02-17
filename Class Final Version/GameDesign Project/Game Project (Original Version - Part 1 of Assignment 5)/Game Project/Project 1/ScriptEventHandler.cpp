#include "ScriptEventHandler.h"

void ScriptEventHandler::onEvent(Event& event)
{
	// Using if statements instead of switch case because of lack of const expressions
	if (event.getType() == eventId) {
		std::cout << event.getBool(internString("script_bool")) << std::endl;
		eventScripts->runEventScript("handleEvent", 1, &event);
	}
	else {
		// Unrecognized event
	}
}

void ScriptEventHandler::setEventIdCheck(StringId eventId)
{
	this->eventId = eventId;
}

void ScriptEventHandler::setScriptManager(ScriptManager* eventScripts)
{
	this->eventScripts = eventScripts;
}
