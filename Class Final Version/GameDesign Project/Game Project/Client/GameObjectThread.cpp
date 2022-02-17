/**
* @file GameObjectThread.cpp
* @author Stephen Wang (sywang)
* Thread handles game object updating
*/
#include "GameObjectThread.h"

/**
* Constructor for thread
* @param objectList the objects to update
* @param _mutex the mutex related to thread
* @param _condition_variable the condition variable related to thread
* @param running the indicator as to whether the program is still running
*/
GameObjectThread::GameObjectThread(std::vector<AbstractGameObject*>* objectList, std::mutex* _mutex,
	std::condition_variable* _condition_variable, std::condition_variable* _next_variable, bool* running, GameTime* gameTimeline)
	: AbstractThread(_mutex, _condition_variable)
{
	this->objectList = objectList;
	this->_next_variable = _next_variable;
	this->running = running;
	this->gameTimeline = gameTimeline;
}

/**
* Thread runs position updates for objects
*/
void GameObjectThread::run() {
	try {
		while (*running) {
			{
				std::unique_lock<std::mutex> lock(*AbstractThread::getMutex());
				AbstractThread::getConditionVar()->wait(lock);
			}
			if (!gameTimeline->getPaused() && !EventManager::getReplaying()) {
				for (AbstractGameObject* o : *this->objectList) {
					o->runThreadSafeComp();
				}
			}
			_next_variable->notify_all();
		}
	}
	catch (...) {
		std::cerr << "PlatformThread caught exception." << std::endl;
	}
}