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
	zmq::context_t *context, std::vector<int>* clientIterations, bool* running)
{
	this->_mutex = _mutex;
	this->_condition_variable = _condition_variable;
	this->context = context;
	this->clientIterations = clientIterations;
	this->running = running;
	this->busy = false;
}

/**
* Executes the thread
*/
void ClientJoinThread::run()
{
	try {
		zmq::socket_t socket(*context, ZMQ_REP);
		socket.bind("tcp://*:5555");

		while (*running) {
			zmq::message_t request;

			//  Wait for next request from client
			std::cout << "Awaiting new client..." << std::endl;
			socket.recv(&request);
			std::cout << "Received new client." << std::endl;

			{
				std::unique_lock<std::mutex> lock(*this->_mutex);
				this->_condition_variable->wait(lock);
				clientIterations->push_back(1);
			}
			//  Send reply back to client
			zmq::message_t reply(18);
			memcpy(reply.data(), "Client registered.", 18);
			socket.send(reply);
		}
	}
	catch (...) {
		std::cerr << "ClientJoinThread has caught an exception." << std::endl;
	}
}