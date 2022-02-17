/**
* Eventmanager class handles registering, unregistering, and dispatching events
* to eventhandlers through a priority queue. The event manager needs the pointer
* to a game timeline to be set in order to operate - the event manager assigns
* timestamps to events and determines when to dispatch to handlers
* @author Stephen Wang (sywang)
*/
#include "EventManager.h"


// Declaration of the static variables
std::unordered_map<StringId, std::vector<AbstractEventHandler*>> EventManager::handlers;
std::priority_queue<Event*, std::vector<Event*>, comparePriority> EventManager::pQueue;
std::priority_queue<Event*, std::vector<Event*>, comparePriority> EventManager::replayQueue;
bool EventManager::recording = false;
long double EventManager::beginRecordTime = 0;
bool EventManager::replaying = false;
long double EventManager::beginReplayTime = 0;
long double EventManager::lastTicChange = 1;
GameTime* EventManager::gameTimeline;
std::mutex EventManager::_mutex;

// Keep track of the event types
StringId EventStringId::collisionType = internString("collision");
StringId EventStringId::deathCollision = internString("death");
StringId EventStringId::translateCollision = internString("translate");
StringId EventStringId::respawnSetCollision = internString("respawn_set");
StringId EventStringId::nextAnimate = internString("next");
StringId EventStringId::serverUpdateAnimate = internString("update");
StringId EventStringId::playerObjectInput = internString("main_character");
StringId EventStringId::gravityInfluence = internString("gravity");
StringId EventStringId::replayRecord = internString("replay_start");
StringId EventStringId::replayTerminate = internString("replay_terminate");

// Keep track of the argument types
StringId EventStringId::gameObjectPointer = internString("game_object");
StringId EventStringId::otherGameObjectPointer = internString("other_game_object");
StringId EventStringId::componentPointer = internString("component");
StringId EventStringId::spawnTranslationVector = internString("spawnXY_translation");
StringId EventStringId::collisionVector = internString("collision_calculation");
StringId EventStringId::newPosition = internString("new_position");
StringId EventStringId::oldXYTravel = internString("untranslated_position");
StringId EventStringId::xyInput = internString("user_xy_input");
StringId EventStringId::gravityYTravel = internString("y_travel");
StringId EventStringId::jumpBoolean = internString("jump_boolean");


/**
* Registers the eventtype with its eventhandler
* @param eventType the StringId of the event
* @param h the pointer to the eventhandler
*/
void EventManager::registerEvent(StringId eventType, AbstractEventHandler* h)
{
	handlers[eventType].push_back(h);
}

/**
* Unregisters and eventhandler from the associated eventType
* @param h the pointer to the eventhandler
* @param eventType the StringId of the event
*/
void EventManager::unregisterEvent(AbstractEventHandler* h, StringId eventType)
{
	std::vector<AbstractEventHandler*>::iterator it = std::find(handlers[eventType].begin(), handlers[eventType].end(), h);
	if (it != handlers[eventType].end()) {
		delete* it;
		handlers[eventType].erase(it);
	}
}

/**
* Tells the event manager to record (duplicate events into replayQueue) or to stop recording
* @param boolean the indicator for whether to record
*/
void EventManager::setRecording(bool boolean)
{
	recording = boolean;
}

void EventManager::startRecording(long double timestamp)
{
	setRecording(true);
	beginRecordTime = timestamp;
}

/**
* Tells the event manager to begin replay or to end replay
* @param boolean the indicator for whether to replay
*/
void EventManager::setReplaying(bool boolean)
{
	replaying = boolean;
}

void EventManager::startReplaying(long double timestamp)
{
	lastTicChange = gameTimeline->getTime() * gameTimeline->getTic();
	setReplaying(true);
	setRecording(false);
	beginReplayTime = timestamp - beginRecordTime;
}

void EventManager::setTicChangeTime(long double timestamp)
{
	lastTicChange = timestamp;
}

/**
* Raises an event by adding it to the queue (deep copies it)
* @param e the event to be raised
*/
void EventManager::raise(Event e)
{
	Event* eCopy = new Event(e);
	eCopy->timestamp;
	if (e.getType() == EventStringId::gravityInfluence) {
		eCopy->priority = 0;
	}
	else if (e.getType() == EventStringId::playerObjectInput) {
		eCopy->priority = 1;
	}
	else if (e.getType() == EventStringId::deathCollision || e.getType() == EventStringId::respawnSetCollision
		|| e.getType() == EventStringId::translateCollision) {
		eCopy->priority = 2;
		if (recording) {
			Event* eCopyTwo = new Event(*eCopy);
			replayQueue.push(eCopyTwo);
		}
	}
	else if (e.getType() == EventStringId::collisionType) {
		eCopy->priority = 3;
	}
	else if (e.getType() == EventStringId::serverUpdateAnimate) {
		eCopy->priority = 4;
		if (recording) {
			Event* eCopyTwo = new Event(*eCopy);
			replayQueue.push(eCopyTwo);
		}
	}
	else if (e.getType() == EventStringId::nextAnimate) {
		eCopy->priority = 5;
		if (recording) {
			Event* eCopyTwo = new Event(*eCopy);
			replayQueue.push(eCopyTwo);
		}
	}
	else if (e.getType() == EventStringId::replayRecord || e.getType() == EventStringId::replayTerminate) {
		eCopy->priority = 6;
	}
	else {
		eCopy->priority = 7;
	}
	{
		std::unique_lock<std::mutex> lock(_mutex);
		pQueue.push(eCopy);
	}
}

/**
* Handle all events in queue
*/
void EventManager::handleEvents()
{
	{
		std::unique_lock<std::mutex> lock(_mutex);
		while (!pQueue.empty()) {
			Event* e = pQueue.top();

			// Check to see if the event should be dispatched
			if (e->timestamp / gameTimeline->getTic() < gameTimeline->getTime()) {
				pQueue.pop();
				for (AbstractEventHandler* h : handlers[e->getType()]) {
					h->onEvent(*e);
				}
				delete e;
			}
			else {
				break;
			}
		}
		if (replaying) {
			while (!replayQueue.empty()) {
				Event* e = replayQueue.top();
				// Check to see if the event should be dispatched
				if (beginReplayTime + e->timestamp < gameTimeline->getTime() * gameTimeline->getTic()) {
					replayQueue.pop();
					for (AbstractEventHandler* h : handlers[e->getType()]) {
						h->onEvent(*e);
					}
					delete e;
				}
				else {
					break;
				}
			}
			if (replayQueue.empty()) {
				setReplaying(false);
			}
		}
	}
}

/**
* Sets a new gametimeline for the eventmanager to keep track of
* @param gameTime the pointer to the new gametime to keep track of
*/
void EventManager::setGametimeline(GameTime* gameTime)
{
	gameTimeline = gameTime;
}
