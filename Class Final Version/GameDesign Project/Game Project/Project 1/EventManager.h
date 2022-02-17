/**
* Event manager handles events (raises, registers, and instructs handlers of events)
* @author Stephen Wang (sywang)
*/
#pragma once
#include <unordered_map>
#include <queue>
#include <mutex>
#include "Events.h"
#include "AbstractEventHandler.h"
#include "RealTime.h"
#include "GameTime.h"
#include "StringId.h"

#include <iostream>

/**
* Handles priority determination between events
* Priority has largest on top (order sort from last element to first)
*/
struct comparePriority {
	bool operator()(Event const* e1, Event const* e2)
	{
		if (e1->timestamp == e2->timestamp)
		{
			return e1->priority > e2->priority;

		}
		return e1->timestamp > e2->timestamp;
	}
};

/**
* Class EventManager manages events and their associated handlers
*/
class EventManager
{
private:
	// Stores the variant arguments of the event
	// StringId is a representation of the event ID
	static std::unordered_map<StringId, std::vector<AbstractEventHandler*>> handlers;
	static std::priority_queue<Event*, std::vector<Event*>, comparePriority> pQueue;
	static std::priority_queue<Event*, std::vector<Event*>, comparePriority> replayQueue;
	static bool recording;
	static long double beginRecordTime;
	static bool replaying;
	static long double beginReplayTime;
	static long double lastTicChange;
	static GameTime* gameTimeline;
	static std::mutex _mutex;

	// Set the replaying boolean
	static void setReplaying(bool boolean);

public:
	// Handles registering and unregistering eventhandlers to events
	static void registerEvent(StringId eventType, AbstractEventHandler* h);
	static void unregisterEvent(AbstractEventHandler* h, StringId eventType);

	// Getter and setter for the recording boolean
	static void setRecording(bool boolean);
	static void startRecording(long double timestamp);
	static bool getRecording() { return recording; }
	static void startReplaying(long double timestamp);
	static bool getReplaying() { return replaying; }
	static void setTicChangeTime(long double timestamp);

	// Raise the event by adding it into the priority queue
	static void raise(Event e);
	// Handle the events in the priority queue (based off of timestamps) -- Dispatch to events
	static void handleEvents();
	static void setGametimeline(GameTime* gameTime);
};

class EventStringId
{
public:
	// Keep track of the event types
	static StringId collisionType;
	static StringId deathCollision;
	static StringId translateCollision;
	static StringId respawnSetCollision;
	static StringId nextAnimate;
	static StringId serverUpdateAnimate;
	static StringId playerObjectInput;
	static StringId gravityInfluence;
	static StringId replayRecord;
	static StringId replayTerminate;

	// Keep track of the argument types
	static StringId gameObjectPointer;
	static StringId otherGameObjectPointer;
	static StringId componentPointer;
	static StringId spawnTranslationVector;
	static StringId collisionVector;
	static StringId newPosition;
	static StringId oldXYTravel;
	static StringId xyInput;
	static StringId gravityYTravel;
	static StringId jumpBoolean;
};