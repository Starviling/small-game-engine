/**
* @file PlatformThread.cpp
* @author Stephen Wang (sywang)
* Thread handles entities updating
*/
#include "AbstractThread.h"
#include "Player.h"

#pragma once
class PlayerThread : public AbstractThread
{
private:
	// Keeps track of entities
	std::vector<sf::Shape*>* entityList;
	std::vector<sf::Shape*>* playerList;
	// Indicates whether program is still running
	bool* running;
public:
	// Constructor for player threads
	PlayerThread(std::vector<sf::Shape*>* playerList, std::vector<sf::Shape*>* entitiesList,
		std::mutex* _mutex, std::condition_variable* _condition_variable, bool* running);
	// Provide implementation for abstract function
	virtual void run() override;
};

