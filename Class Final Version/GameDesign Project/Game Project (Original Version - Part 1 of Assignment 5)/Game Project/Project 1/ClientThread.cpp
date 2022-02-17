/**
* @file ClientThread.cpp
* @author Stephen Wang (sywang)
* Class for handling other clients
*/
#include "ClientThread.h"

/**
* Constructor for the ClientJoinThread class
* @param _mutex the pointer to the mutex for the clientIterations vector
* @param _condition_variable the pointer to the condition variable for the clientIterations vector
* @param context the pointer to the context of the zmq
* @param objectList the pointer to the objects to pass to clients
* @param player the pointer to the player controlled by server
* @param running the pointer to indicate whether the program is still running
* @param framePeriod the pointer to the frame period
*/
ClientThread::ClientThread(std::mutex* _mutex, std::mutex* _other_mutex, std::condition_variable* _condition_variable, zmq::context_t* context,
	std::vector<AbstractGameObject*>* objectList, bool* running, long double* framePeriod, long double* gameTimeFrame, sf::Vector2f* xyTranslation)
	: AbstractThread(_mutex, _condition_variable)
{
	this->_other_mutex = _other_mutex;
	this->context = context;
	this->running = running;
	this->objectList = objectList;
	this->busy = false;
	this->framePeriod = framePeriod;
	this->gameTimeFrame = gameTimeFrame;
	this->xyTranslation = xyTranslation;
}

/**
* Executes the thread
*/
void ClientThread::run()
{
	try {
		zmq::socket_t socket(*context, ZMQ_REP);
		zmq::socket_t socketMovement(*context, ZMQ_REP);
		socket.bind("tcp://*:5555");
		socketMovement.bind("tcp://*:5554");

		// While the program is still running
		while (*running) {
			if (!EventManager::getReplaying()) {
				// Process request
				zmq::message_t request;

				//  Keep checking for new client
				if (socket.recv(&request, ZMQ_DONTWAIT)) {
					std::cout << "Received new client." << std::endl;
					// Set up info transfer to clients
					json jInfo;
					{
						std::unique_lock<std::mutex> lock(*this->getMutex());
						std::unique_lock<std::mutex> lock2(*_other_mutex);
						//this->getConditionVar()->wait(lock);
						// Provide information on objects to client
						jInfo.push_back(objectList->size());
						for (AbstractGameObject* o : *this->objectList) {
							jInfo.push_back(o->getUpdateInfo());
						}
						//  Send reply back to client
						int jInfoSize = jInfo.dump().length() + 1;
						zmq::message_t reply(jInfoSize);
						snprintf((char*)reply.data(), jInfoSize, "%s", jInfo.dump().c_str());
						socket.send(reply);

						// Add player to the objectList
						socket.recv(&request);
						jInfo = json::parse((char*)request.data());
						AbstractGameObject* newPlayer = new PlayerObject(0, 0, gameTimeFrame, xyTranslation);
						objectList->push_back(newPlayer);
						std::cout << "Player Generated." << std::endl;

						reply.rebuild(17);
						memcpy(reply.data(), "Player Generated.", 17);
						socket.send(reply);
					}

				}

				// Update client information
				if (socketMovement.recv(&request, ZMQ_DONTWAIT)) {
					json jInfo;
					{
						jInfo = json::parse((char*)request.data());
						std::unique_lock<std::mutex> lock(*this->getMutex());
						std::unique_lock<std::mutex> lock2(*_other_mutex);
						//this->getConditionVar()->wait(lock);
						// Receive a json that contains (the index of the player in objectList
						// and a json object for server updating)
	//					std::cout << "Player ID: " + jInfo.at(0) << std::endl;
						//objectList->at(jInfo.at(0))->serverUpdate(jInfo.at(1));

						// Send raise an event for server update based on client animation event
						if (!EventManager::getReplaying()) {
							Event e = Event(EventStringId::serverUpdateAnimate);
							e.addGameObject(EventStringId::gameObjectPointer, objectList->at(jInfo.at(0)));
							e.addVector(EventStringId::nextAnimate, sf::Vector2f(jInfo.at(1)["position"][0], jInfo.at(1)["position"][1]));
							e.timestamp = *gameTimeFrame;
							EventManager::raise(e);
						}

						zmq::message_t reply(15);
						memcpy(reply.data(), "Player Updated.", 15);
						socketMovement.send(reply);
						//					std::cout << "Player updated." << std::endl;
					}
				}
			}
		}
	}
	catch (...) {
		std::cerr << "ClientJoinThread has caught an exception." << std::endl;
	}
}
