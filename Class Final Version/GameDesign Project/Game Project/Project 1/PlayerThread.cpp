/**
* @file PlayerThread.cpp
* @author Stephen Wang (sywang)
* Thread handles players updating
*/
#include "PlayerThread.h"
#include <iostream>

/**
* Constructor for thread
* @param playerList the players to take into account while calculating
* @param entitiesList the platforms to take into account while calculating
* @param type the types of platforms being taken into account
* @param _mutex the mutex related to thread
* @param _condition_variable the condition variable related to thread
* @param running the indicator as to whether the program is still running
*/
PlayerThread::PlayerThread(std::vector<sf::Shape*>* playerList, std::vector<sf::Shape*>* entitiesList,
	std::mutex* _mutex, std::condition_variable* _condition_variable, bool* running)
	: AbstractThread(_mutex, _condition_variable)
{
	this->entityList = entitiesList;
	this->playerList = playerList;
	this->running = running;
}

/**
* Thread runs position updates for platforms
*/
void PlayerThread::run() {

	try {
		while (*running) {
			std::unique_lock<std::mutex> lock(*AbstractThread::getMutex());
			AbstractThread::getConditionVar()->wait(lock);
			for (sf::Shape* s : *this->playerList) {
				Player* p = dynamic_cast<Player*> (s);
				p->updatePosition(entityList);
			}
//			AbstractThread::getConditionVar()->notify_all();
		}
	}
	catch (...) {
		std::cerr << "PlatformThread caught exception." << std::endl;
	}
}
