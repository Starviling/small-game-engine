/**
* @file ClientJoinThread.h
* @author Stephen Wang (sywang)
* Class for handling thread creation
*/
#include "ClientJoinThread.h"


/**
* Constructor for the ClientJoinThread class
* @param _mutex the pointer to the mutex for the clientIterations vector
* @param _condition_variable the pointer to the condition variable for the clientIterations vector
* @param context the pointer to the context of the zmq
* @param clientIterations the pointer to the vector keeping track of client iterations
* @param running the pointer to the boolean determining whether the program is still running
*/
ClientJoinThread::ClientJoinThread(std::mutex* _mutex, std::condition_variable* _condition_variable,
	zmq::context_t* context, std::vector<sf::Shape*>* entityList, std::vector<sf::Shape*>* playerList, bool* running,
	long double *framePeriod, bool *otherFocus)
	: AbstractThread(_mutex, _condition_variable)
{
	this->context = context;
	this->running = running;
	this->entityList = entityList;
	this->playerList = playerList;
	this->busy = false;
	this->framePeriod = framePeriod;
	this->otherFocus = otherFocus;
}

/**
* Executes the thread
*/
void ClientJoinThread::run()
{
	try {
		zmq::socket_t socket(*context, ZMQ_REP);
		zmq::socket_t socketMovement(*context, ZMQ_REP);
		socket.bind("tcp://*:5555");
		socketMovement.bind("tcp://*:5554");

		while (*running) {
			zmq::message_t request;
			//  Keep checking for new client
			if (socket.recv(&request, ZMQ_DONTWAIT)) {
				std::cout << "Received new client." << std::endl;
				// Set up info transfer to clients
				json jInfo;
				{
					std::unique_lock<std::mutex> lock(*this->getMutex());
					//this->getConditionVar()->wait(lock);
					// Provide information on platforms to client
					jInfo.push_back(entityList->size());
					for (sf::Shape* s : *this->entityList) {
						Platform* p = dynamic_cast<Platform*> (s);
						sf::Vector2f tempPosition = p->getPosition();
						float tempPositionArray[4] = { tempPosition.x, tempPosition.y, p->getOrientation(), p->getSpeed() };
						jInfo.push_back(tempPositionArray);
					}
					// Provide information on players to client
					jInfo.push_back(playerList->size());
					for (sf::Shape* s : *this->playerList) {
						Player* p = dynamic_cast<Player*> (s);
						sf::Vector2f tempPosition = p->getPosition();
						float tempPositionArray[2] = { tempPosition.x, tempPosition.y };
						jInfo.push_back(tempPositionArray);
					}
				}
				//  Send reply back to client
				int jInfoSize = jInfo.dump().length() + 1;
				zmq::message_t reply(jInfoSize);
				snprintf((char*)reply.data(), jInfoSize, "%s", jInfo.dump().c_str());
				socket.send(reply);

				// Add player to the playerList
				socket.recv(&request);
				jInfo = json::parse((char*)request.data());
				Player* newPlayer = new Player(jInfo.at(0)[0], jInfo.at(0)[1], framePeriod, otherFocus);
				playerList->push_back(newPlayer);
				std::cout << "Player Generated." << std::endl;

				reply.rebuild(17);
				memcpy(reply.data(), "Player Generated.", 17);
				socket.send(reply);
			}
			// Manage client movement
			// Keep checking for new client
			if (socketMovement.recv(&request, ZMQ_DONTWAIT)) {
				json jInfo;
				{
					jInfo = json::parse((char*)request.data());
					std::unique_lock<std::mutex> lock(*this->getMutex());
					//this->getConditionVar()->wait(lock);
					std::cout << "Player ID: " + jInfo.at(0) << std::endl;
					playerList->at(jInfo.at(0))->setPosition(jInfo.at(1)[0], jInfo.at(1)[1]);
					zmq::message_t reply(15);
					memcpy(reply.data(), "Player Updated.", 15);
					socketMovement.send(reply);
					std::cout << "Player updated." << std::endl;
				}
			}
		}
	}
	catch (...) {
		std::cerr << "ClientJoinThread has caught an exception." << std::endl;
	}
}