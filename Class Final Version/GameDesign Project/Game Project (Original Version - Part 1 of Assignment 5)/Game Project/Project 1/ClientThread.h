/**
* @file ClientThread.h
* @author Stephen Wang (sywang)
* Class for handling other clients
*/
#pragma once
#include "AbstractThread.h"
#include "PlayerObject.h"
#include "AbstractGameObject.h"
#include "EventManager.h"
#include <zmq.hpp>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <iostream>
#include <json.hpp>
using namespace nlohmann;

class ClientThread : public AbstractThread
{
private:
	bool busy;
	zmq::context_t* context;
	
	// Other mutex to lock when updating objects
	std::mutex* _other_mutex;

	// Entity list and player to update incoming clients
	std::vector<AbstractGameObject*>* objectList;

	// Indicates whether program is still running
	bool* running;
	// Provides the framePeriod pointer
	long double* framePeriod;
	long double* gameTimeFrame;

	// Pointer to translation of coordinates
	sf::Vector2f* xyTranslation;

	StringId playerId = internString("player");
	StringId platformId = internString("platform");

public:
	// Constructor for join handler
	ClientThread(std::mutex* _mutex, std::mutex* _other_mutex, std::condition_variable* _condition_variable,
		zmq::context_t* context, std::vector<AbstractGameObject*>* objectList, bool* running, long double* framePeriod, long double* gameTimeFrame, sf::Vector2f* xyTranslation);

	virtual void run() override;
};

