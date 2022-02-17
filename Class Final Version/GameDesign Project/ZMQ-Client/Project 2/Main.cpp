/**
* @file Main.cpp
* @author Stephen Wang (sywang)
* Program demonstrates server behavior with an unrestricted number of clients.
*/
#include <zmq.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <iterator>
#include <map>
#ifndef _WIN32
#include <unistd.h>
#else
#include <windows.h>

#define sleep(n)    Sleep(n)
#endif
#include "ClientJoinThread.h"

/**
* Run wrapper for threads
*/
void run_wrapper(ClientJoinThread* fe)
{
    fe->run();
}

/**
* Main part handles client server interactions
* @return the exit status of the program
*/
int main() {

    // empty vector container 
    std::vector<int> clientIterations;

    //  Prepare our context and publisher
    zmq::context_t context(1);
    zmq::socket_t publisher(context, ZMQ_PUB);
    publisher.bind("tcp://*:5556");

    std::string clientStr("Client: ");
    std::string iterationStr(" Iteration: ");

    bool running = true;
    std::mutex _mutex;
    std::condition_variable _client_iter_variable;
    ClientJoinThread joinThread(&_mutex, &_client_iter_variable, &context, &clientIterations, &running);
    std::thread joinHandler(run_wrapper, &joinThread);

    while (running) {
        std::unique_lock<std::mutex> lock(_mutex);
        for (int i = 0; i < clientIterations.size(); i++) {
            // Set up information on client and iteration to be sent
            std::string clientFullStr = clientStr + std::to_string(i + 1);
            int iterationNumber = clientIterations.at(i);
            std::string iterationFullStr = iterationStr + std::to_string(iterationNumber);
            // Update iteration number
            clientIterations.at(i) = iterationNumber + 1;

            //  Send message to all 
            int stringFullSize = sizeof(clientFullStr) + sizeof(iterationFullStr) + 1;
            zmq::message_t message(stringFullSize);

            snprintf((char*)message.data(), stringFullSize,
                "%s %s", clientFullStr.c_str(), iterationFullStr.c_str());
            publisher.send(message);
        }
        _client_iter_variable.notify_all();

        // To demonstrate the ability to handle stuff properly
        sleep(500);
    }

    joinHandler.join();
    return 0;
}