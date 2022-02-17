#include "CollisionEventHandler.h"

void CollisionEventHandler::onEvent(Event& event)
{
	// Using if statements instead of switch case because of lack of const expressions
	if (event.getType() == EventStringId::collisionType) {
		(dynamic_cast<Collision*> (event.getArgument(EventStringId::componentPointer).m_asComponentPtr))
			->runEvent(event.getArgument(EventStringId::collisionVector).m_asVector, event.getArgument(EventStringId::oldXYTravel).m_asVector,
				event.getArgument(EventStringId::jumpBoolean).m_asBoolean);
	}
	else if (event.getType() == EventStringId::deathCollision) {
		// Death collision
		(dynamic_cast<AbstractZoneObject*> (event.getArgument(EventStringId::otherGameObjectPointer).m_asGameObjectPtr))
			->trigger(event.getArgument(EventStringId::gameObjectPointer).m_asGameObjectPtr, event.getArgument(EventStringId::otherGameObjectPointer).m_asGameObjectPtr);
	}
	else if (event.getType() == EventStringId::respawnSetCollision) {
		// Respawn set collision
		(dynamic_cast<AbstractZoneObject*> (event.getArgument(EventStringId::otherGameObjectPointer).m_asGameObjectPtr))
			->trigger(event.getArgument(EventStringId::gameObjectPointer).m_asGameObjectPtr, event.getArgument(EventStringId::otherGameObjectPointer).m_asGameObjectPtr);
	}
	else if (event.getType() == EventStringId::translateCollision) {
		// Translation collision
		(dynamic_cast<AbstractZoneObject*> (event.getArgument(EventStringId::otherGameObjectPointer).m_asGameObjectPtr))
			->trigger(event.getArgument(EventStringId::gameObjectPointer).m_asGameObjectPtr, event.getArgument(EventStringId::otherGameObjectPointer).m_asGameObjectPtr);
	}
	else {
		// Unrecognized event
	}
}
