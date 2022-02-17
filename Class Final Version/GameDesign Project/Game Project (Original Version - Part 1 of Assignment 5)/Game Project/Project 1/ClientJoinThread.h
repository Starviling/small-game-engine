/**
* @file ClientJoinThread.h
* @author Stephen Wang (sywang)
* Class for handling thread creation
*/
#include <zmq.hpp>
#include <thread>
#include <chrono>
#include <mutex>
#include <condition_variable>
#include <iostream>
#include "AbstractThread.h"
#include "Platform.h"
#include "Player.h"
#include <SFML\Graphics\Shape.hpp>
#include <json.hpp>
using namespace nlohmann;

#pragma once
class ClientJoinThread : public AbstractThread
{
private:
	bool busy;
	zmq::context_t* context;

	// Entity list and player list to update incoming clients
	std::vector<sf::Shape*>* entityList;
	std::vector<sf::Shape*>* playerList;

	// Indicates whether program is still running
	bool* running;
	// Provides the framePeriod pointer
	long double* framePeriod;
	// Provides the focus bool for other players
	bool* otherFocus;
public:
	// Constructor for join handler
	ClientJoinThread(std::mutex* _mutex, std::condition_variable* _condition_variable,
		zmq::context_t* context, std::vector<sf::Shape*>* entityList, std::vector<sf::Shape*>* playerList, bool* running,
		long double* framePeriod, bool* otherFocus);

	virtual void run() override;
};

