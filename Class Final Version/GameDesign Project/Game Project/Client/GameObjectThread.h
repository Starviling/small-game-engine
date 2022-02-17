/**
* @file GameObjectThread.h
* @author Stephen Wang (sywang)
* Thread handles game object updating
*/
#pragma once
#include "AbstractThread.h"
#include <vector>
#include "AbstractGameObject.h"
#include <iostream>
#include "GameTime.h"
#include "EventManager.h"

class GameObjectThread : public AbstractThread
{
private:
	// Pointer to list of objects to update
	std::vector<AbstractGameObject*>* objectList;
	// Indicates whether program is still running
	bool* running;
	// Signify the next thread that they can start running
	std::condition_variable* _next_variable;
	GameTime* gameTimeline;

public:
	// Constructor for object threads
	GameObjectThread(std::vector<AbstractGameObject*>* objectList, std::mutex* _mutex,
		std::condition_variable* _condition_variable, std::condition_variable* _next_variable, bool* running, GameTime* gameTimeline);
	// Provide implementation for abstract function
	void run() override;
};

