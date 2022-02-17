#include "AnimateEventHandler.h"

void AnimateEventHandler::onEvent(Event& event)
{
	// Using if statements instead of switch case because of lack of const expressions
	if (event.getType() == EventStringId::nextAnimate) {
		(dynamic_cast<Animation*> (event.getArgument(EventStringId::componentPointer).m_asComponentPtr))
			->runEvent(event.getArgument(EventStringId::newPosition).m_asVector, event.getArgument(EventStringId::oldXYTravel).m_asVector);
	}
	else if (event.getType() == EventStringId::serverUpdateAnimate) {
		// Give server update argument from event to the game object
		(dynamic_cast<AbstractGameObject*> (event.getArgument(EventStringId::gameObjectPointer).m_asGameObjectPtr))
			->setPosition(event.getArgument(EventStringId::nextAnimate).m_asVector);
	}
	else if (event.getType() == EventStringId::replayRecord) {
		if (!EventManager::getRecording() && !EventManager::getReplaying()) {
			EventManager::startRecording(event.timestamp);
		}
	}
	else if (event.getType() == EventStringId::replayTerminate) {
		if (EventManager::getRecording() && !EventManager::getReplaying()) {
			EventManager::startReplaying(event.timestamp);
		}
	}
	else {
		// Unrecognized event
	}
}
