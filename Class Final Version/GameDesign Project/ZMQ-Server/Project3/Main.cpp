/**
* @file Main.cpp
* @author Stephen Wang (sywang)
* Program demonstrates client behavior.
*/

#include <zmq.hpp>
#include <iostream>
#include <sstream>

/**
* Main part handles client interactions
* @return the exit status of the program
*/
int main()
{
    zmq::context_t context(1);

    //  Socket to talk to server
    std::cout << "Collecting Client information...\n" << std::endl;
    zmq::socket_t subscriber(context, ZMQ_SUB);
    zmq::socket_t socket(context, ZMQ_REQ);
    subscriber.connect("tcp://localhost:5556");
    socket.connect("tcp://localhost:5555");


    zmq::message_t request(22);
    memcpy(request.data(), "Attempting Connection.", 22);
    socket.send(request);

    //  Get the reply.
    zmq::message_t reply;
    socket.recv(&reply);
    std::cout << (char *) reply.data() << std::endl;
    zmq_close(socket);

    // USE FILTER TO GET THE CLIENT NUMBER IF NEEDED
    //    const char* filter = "10001 ";
    //    subscriber.setsockopt(ZMQ_SUBSCRIBE, filter, strlen(filter));
    subscriber.setsockopt(ZMQ_SUBSCRIBE, 0, 0);

    while (true) {
        zmq::message_t update;

        subscriber.recv(&update);
        std::cout << static_cast<char*>(update.data()) << std::endl;
    }
    return 0;
}