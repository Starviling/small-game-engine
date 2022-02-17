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

#pragma once
class ClientJoinThread
{
private:
	bool busy;
	// Vector container for client iterations
	std::vector<int>* clientIterations;
	std::mutex* _mutex;
	std::condition_variable* _condition_variable;
	zmq::context_t* context;
	// Indicates whether program is still running
	bool* running;

public:
	ClientJoinThread(std::mutex* _mutex, std::condition_variable* _condition_variable,
		zmq::context_t *context, std::vector<int>* clientIterations, bool* running);

	// Return the mutex
	std::mutex* getMutex() {
		return _mutex;
	}

	// Return the condition variable
	std::condition_variable* getConditionVar() {
		return _condition_variable;
	}

	bool isBusy()
	{
		std::lock_guard<std::mutex> lock(*_mutex);
		return busy;
	}

	void run();
};

