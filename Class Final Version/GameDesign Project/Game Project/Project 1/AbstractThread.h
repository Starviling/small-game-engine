/**
* @file AbstractThread.h
* @author Stephen Wang (sywang)
* Abstract class for handling thread creation
*/

#pragma once
#include <thread>
#include <chrono>
#include <mutex>
#include <condition_variable>

class AbstractThread
{
private:
	bool busy;
	std::mutex* _mutex;
	std::condition_variable* _condition_variable;

public:
	AbstractThread(std::mutex* _mutex, std::condition_variable* _condition_variable)
	{
		this->_mutex = _mutex;
		this->_condition_variable = _condition_variable;
		this->busy = false;
	}

	std::mutex* getMutex() {
		return _mutex;
	}

	std::condition_variable* getConditionVar() {
		return _condition_variable;
	}

	bool isBusy()
	{
		std::lock_guard<std::mutex> lock(*_mutex);
		return busy;
	}

	virtual void run() = 0;

};

