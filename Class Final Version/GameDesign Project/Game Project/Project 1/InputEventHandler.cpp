#include "InputEventHandler.h"

void InputEventHandler::onEvent(Event& event)
{
	// Using if statements instead of switch case because of lack of const expressions
	if (event.getType() == EventStringId::playerObjectInput) {
		(dynamic_cast<PlayerControl*> (event.getArgument(EventStringId::componentPointer).m_asComponentPtr))
			->runEvent(event.getArgument(EventStringId::xyInput).m_asVector);
	}
	else if (event.getType() == EventStringId::gravityInfluence) {
		(dynamic_cast<Gravity*> (event.getArgument(EventStringId::componentPointer).m_asComponentPtr))
			->runEvent(event.getArgument(EventStringId::gravityYTravel).m_asFloat);
	}
	else {
		// Unrecognized event
	}
}
