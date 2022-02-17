/**
* @file Main.cpp
* @author Stephen Wang (sywang)
* Homework 1
*/
#include <SFML/Graphics.hpp>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <iostream>
#include <zmq.hpp>
#include <json.hpp>
#include "RealTime.h"
#include "GameTime.h"
#include "AbstractGameObject.h"
#include "PlatformObject.h"
#include "PlayerObject.h"
#include "GameObjectThread.h"
#include "GameObjectThreadUnsafe.h"
#include "DeathZoneObject.h"
#include "SpawnZoneObject.h"
#include "TranslateZoneObject.h"

#include "EventManager.h"
#include "AnimateEventHandler.h"
#include "InputEventHandler.h"
#include "CollisionEventHandler.h"
#include "ScriptEventHandler.h"

#include "dukglue/dukglue.h"
#include "ScriptManager.h"
using namespace nlohmann;

/**
* Print string function to demonstrate script capabilities
* @param message the message to print
*/
void printString(std::string message)
{
    std::cout << message << std::endl;
}


void run_wrapper(AbstractThread* fe)
{
    fe->run();
}

/**
* Main part of executes window
* @param argc the number of arguments provided
* @param argv the arguments provided to the program
* @return the exit status of the program
*/
int main()
{
    // Set up window
    sf::RenderWindow window;
    window.create(sf::VideoMode(800, 600), "Za Warudo", sf::Style::Default);
    window.setPosition(sf::Vector2i(10, 50));
    // To prevent screen tearing
    window.setVerticalSyncEnabled(true); // call it once, after creating the window

    // Another method to prevent screen tearing by limiting frame rate
    // Never use this in conjunction with setVerticalSyncEnabled(true)
    // This method is not super reliable with the SFML implementation
    // window.setFramerateLimit(60); // call it once, after creating the window

    // get the size of the window
    sf::Vector2u size = window.getSize();
    unsigned int width = size.x;
    unsigned int height = size.y;

    // Set up timelines
    RealTime* realTimeline = new RealTime();
    GameTime* gameTimeline = new GameTime(realTimeline);

    // Booleans to handle toggles
    bool spacePressed = false;
    bool speedPressed = false;
    bool slowPressed = false;
    bool rPressed = false;
    bool tPressed = false;
    bool cPressed = false;

    // Set up variables to point to in objects
    long double framePeriod = 0;
    long double gameTimeFrame = gameTimeline->getTime() * gameTimeline->getTic();
    bool focused = true;
    bool running = true;
    sf::Vector2f xyTranslation(0, 0);

    // Set up the pointer to list for objects to access
    std::vector<AbstractGameObject*> objectList;
    // Keep track of a vector that deletes entities at prior to each game loop
    std::vector<int> deleteList;



    // Set up event management
    AnimateEventHandler animationEventHandler = AnimateEventHandler();
    InputEventHandler inputEventHandler = InputEventHandler();
    CollisionEventHandler collisionEventHandler = CollisionEventHandler();
    // Register animation events
    EventManager::registerEvent(internString("next"), &animationEventHandler);
    EventManager::registerEvent(internString("update"), &animationEventHandler);
    EventManager::registerEvent(EventStringId::replayRecord, &animationEventHandler);
    EventManager::registerEvent(EventStringId::replayTerminate, &animationEventHandler);
    EventManager::registerEvent(internString("main_character"), &inputEventHandler);
    EventManager::registerEvent(EventStringId::gravityInfluence, &inputEventHandler);
    EventManager::registerEvent(internString("collision"), &collisionEventHandler);
    EventManager::registerEvent(EventStringId::deathCollision, &collisionEventHandler);
    EventManager::registerEvent(EventStringId::respawnSetCollision, &collisionEventHandler);
    EventManager::registerEvent(EventStringId::translateCollision, &collisionEventHandler);
    EventManager::registerEvent(internString("death"), &collisionEventHandler);
    EventManager::registerEvent(internString("translate"), &collisionEventHandler);
    EventManager::registerEvent(internString("respawn_set"), &collisionEventHandler);

    // Register input events
    // Set the gametimeline for event manager to follow
    EventManager::setGametimeline(gameTimeline);





    // Setup communication with server
    zmq::context_t context(1);
    //  Socket to talk to server
    std::cout << "Collecting Client information...\n" << std::endl;
    zmq::socket_t subscriber(context, ZMQ_SUB);
    zmq::socket_t socket(context, ZMQ_REQ);
    zmq::socket_t socketMovement(context, ZMQ_REQ);
    subscriber.setsockopt(ZMQ_SUBSCRIBE, "", 0);

    // Because apparently conflation has to be first
    subscriber.setsockopt(ZMQ_CONFLATE, 1);
    subscriber.connect("tcp://localhost:5556");
    socket.connect("tcp://localhost:5555");
    socketMovement.connect("tcp://localhost:5554");

    zmq::message_t request(22);
    memcpy(request.data(), "Attempting Connection.", 22);
    socket.send(request);

    //  Confirm connection.
    zmq::message_t reply;
    socket.recv(&reply);
    json jInfo;
    jInfo = json::parse((char*)reply.data());
    std::cout << jInfo << std::endl;

    int entitiesNumber = jInfo.at(0);
    StringId playerId = internString("player");
    StringId platformId = internString("platform");
    StringId deathId = internString("death");
    StringId spawnId = internString("spawn");
    StringId translateId = internString("translate");


    std::cout << playerId << std::endl;
    std::cout << platformId << std::endl;
    for (int i = 1; i < entitiesNumber + 1; i++) {
        std::cout << jInfo.at(i)["id"] << std::endl;
        if (jInfo.at(i)["id"] == playerId) {
            std::cout << "player added" << std::endl;
            PlayerObject* player = new PlayerObject(0, 0, &gameTimeFrame, &xyTranslation, &deleteList);
            player->serverUpdate(jInfo.at(i));
            objectList.push_back(player);
        }
        else if (jInfo.at(i)["id"] == platformId) {
            std::cout << "platform added" << std::endl;
            PlatformObject* platform = new PlatformObject(&gameTimeFrame, &xyTranslation, &deleteList);
            platform->serverUpdate(jInfo.at(i));
            objectList.push_back(platform);
        }
        else if (jInfo.at(i)["id"] == deathId) {
            std::cout << "platform added" << std::endl;
            DeathZoneObject* zone = new DeathZoneObject(&gameTimeFrame, &xyTranslation, &deleteList);
            zone->serverUpdate(jInfo.at(i));
            objectList.push_back(zone);
        }
        else if (jInfo.at(i)["id"] == spawnId) {
            std::cout << "platform added" << std::endl;
            SpawnZoneObject* zone = new SpawnZoneObject(&gameTimeFrame, &xyTranslation, &deleteList);
            zone->serverUpdate(jInfo.at(i));
            objectList.push_back(zone);
        }
        else if (jInfo.at(i)["id"] == translateId) {
            std::cout << "platform added" << std::endl;
            TranslateZoneObject* zone = new TranslateZoneObject(&gameTimeFrame, &xyTranslation, &deleteList);
            zone->serverUpdate(jInfo.at(i));
            objectList.push_back(zone);
        }
    }

    int playerIndex = entitiesNumber;
    // Generate player for client
    PlayerObject* newPlayer = new PlayerObject(400, 200, &framePeriod, &gameTimeFrame, &focused, &objectList, &xyTranslation, &deleteList);
    // Push the player onto the objectList for threads to update
    objectList.push_back(newPlayer);
    // Because the player has to be added to the list
    entitiesNumber++;

    // Send back basic info on character
    jInfo.clear();

    jInfo.push_back(newPlayer->getUpdateInfo());
    int jInfoSize = jInfo.dump().length() + 1;
    request.rebuild(jInfoSize);
    snprintf((char*)request.data(), jInfoSize, "%s", jInfo.dump().c_str());
    socket.send(request);
    socket.recv(&reply);
    //zmq_close(socket);



    // Create threads to handle animations
    // Mutex for accessing entities
    std::mutex _mutex;
    std::condition_variable _loop_variable;
    std::condition_variable _update_variable;
    
    GameObjectThread tObjects(&objectList, &_mutex, &_loop_variable, &_update_variable, &running, gameTimeline);
    GameObjectThreadUnsafe tObjectu(&objectList, &_mutex, &_update_variable, &_update_variable, &running, gameTimeline);
    std::thread threadAnimationOne(run_wrapper, &tObjects);
    std::thread threadAnimationTwo(run_wrapper, &tObjectu);



    // Script management
    ScriptManager eventScripts = ScriptManager();
    dukglue_register_constructor<Variant>(eventScripts.getContext(), "Variant");
    dukglue_register_function(eventScripts.getContext(), &internString, "internString");
    dukglue_register_function(eventScripts.getContext(), &printString, "printString");
    dukglue_register_method(eventScripts.getContext(), &Event::addBoolean, "addBool");
    dukglue_register_method(eventScripts.getContext(), &Event::getBool, "getBool");
    //    dukglue_register_constructor<Event, StringId>(eventScripts.getContext(), "Event");

    eventScripts.loadScript("scriptEvents.js");
    // This sets the function intended to be used
    duk_get_prop_string(eventScripts.getContext(), -1, "scriptEventId");
    // This actually executes the call to the script function
    if (duk_pcall(eventScripts.getContext(), 0) != 0)
    {
        std::cout << "Error: ";
        std::cout << duk_safe_to_string(eventScripts.getContext(), -1) << std::endl;
    }

    ScriptEventHandler scriptEventHandler = ScriptEventHandler();
    StringId scriptEventId = (StringId)duk_get_number(eventScripts.getContext(), -1);
    scriptEventHandler.setScriptManager(&eventScripts);
    scriptEventHandler.setEventIdCheck(scriptEventId);
    EventManager::registerEvent(scriptEventId, &scriptEventHandler);
    duk_pop(eventScripts.getContext());

    ScriptManager scripter = ScriptManager();
    dukglue_register_method(scripter.getContext(), &PlayerObject::setSpeed, "adjustSpeed");





    // TODO: Improve frame lock
    long double frameCheck = 40.0f / 30.0f;
    long double realTimeCheck = 0;
    long double tempTic = 0;
    int idxUpdate = 0;
    for (AbstractGameObject* o : objectList) {
        o->setIndex(idxUpdate++);
    }

    // Keep the program running as long as the window is open
    while (window.isOpen()) {
        scripter.loadScript("scriptPlayerFile.js");
        // This sets the function intended to be used
        duk_get_prop_string(scripter.getContext(), -1, "playerSpeedAdjust");
        // Dukglue's push function let's us push almost any value we want onto the stack
        dukglue_push(scripter.getContext(), newPlayer);
        // This actually executes the call to the script function
        if (duk_pcall(scripter.getContext(), 1) != 0)
        {
            std::cout << "Error: ";
            std::cout << duk_safe_to_string(scripter.getContext(), -1) << std::endl;
        }
        duk_pop(scripter.getContext());

        // Improve frame lock
        framePeriod = gameTimeline->getDeltaTime();
        tempTic = gameTimeline->getTic();
        gameTimeFrame = gameTimeline->getTime() * gameTimeline->getTic();
        if (framePeriod * tempTic > frameCheck) {
            framePeriod = frameCheck / tempTic;
        }

        /*
        {
            zmq::message_t update;
            subscriber.recv(&update);
            jInfo = json::parse((char*)update.data());

            // Retrieve the entities on server
            int newEntitiesNumber = jInfo.at(0);
            {
                std::unique_lock<std::mutex> lock(_mutex);
                deleteList = jInfo[newEntitiesNumber + 1].get<std::vector<int>>();
                // Handle deletion of objects
                for (int delIndex : deleteList) {
                    if (delIndex >= 0 && delIndex < objectList.size()) {
                        std::cout << delIndex << std::endl;
                        std::cout << objectList.at(delIndex)->getId();
                        delete objectList.at(delIndex);
                        objectList.at(delIndex) = nullptr;
                        objectList.erase(objectList.begin() + delIndex);
                        entitiesNumber--;
                        std::cout << ": Successful deletion" << std::endl;
                    }
                    else {
                        std::cout << "Failed deletion: " << delIndex << std::endl;
                    }
                }
                idxUpdate = 0;
                for (AbstractGameObject* o : objectList) {
                    o->setIndex(idxUpdate++);
                }
            }

            if (newEntitiesNumber > entitiesNumber) {
                for (int i = entitiesNumber; i < newEntitiesNumber; i++) {

                    if (jInfo.at(i)["id"] == playerId) {
                        PlayerObject* player = new PlayerObject(0, 0, &gameTimeFrame, &xyTranslation, &deleteList);
                        player->setIndex(objectList.size());
                        player->serverUpdate(jInfo.at(i));
                        objectList.push_back(player);
                    }
                    else if (jInfo.at(i)["id"] == platformId) {
                        PlatformObject* platform = new PlatformObject(&gameTimeFrame, &xyTranslation, &deleteList);
                        platform->setIndex(objectList.size());
                        platform->serverUpdate(jInfo.at(i));
                        objectList.push_back(platform);
                    }
                    else if (jInfo.at(i)["id"] == deathId) {
                        std::cout << "platform added" << std::endl;
                        DeathZoneObject* zone = new DeathZoneObject(&gameTimeFrame, &xyTranslation, &deleteList);
                        zone->setIndex(objectList.size());
                        zone->serverUpdate(jInfo.at(i));
                        objectList.push_back(zone);
                    }
                    else if (jInfo.at(i)["id"] == spawnId) {
                        std::cout << "platform added" << std::endl;
                        SpawnZoneObject* zone = new SpawnZoneObject(&gameTimeFrame, &xyTranslation, &deleteList);
                        zone->setIndex(objectList.size());
                        zone->serverUpdate(jInfo.at(i));
                        objectList.push_back(zone);
                    }
                    else if (jInfo.at(i)["id"] == translateId) {
                        std::cout << "platform added" << std::endl;
                        TranslateZoneObject* zone = new TranslateZoneObject(&gameTimeFrame, &xyTranslation, &deleteList);
                        zone->setIndex(objectList.size());
                        zone->serverUpdate(jInfo.at(i));
                        objectList.push_back(zone);
                    }
                }
                entitiesNumber = newEntitiesNumber;
            }

            for (int i = 1; i < newEntitiesNumber + 1; i++) {
                if (i - 1 != newPlayer->getIndex()) {
                    // i - 1 to adjust index to proper index

                    if (!EventManager::getReplaying()) {
                        Event e = Event(EventStringId::serverUpdateAnimate);
                        e.addGameObject(EventStringId::gameObjectPointer, objectList.at(i - 1));
                        e.addVector(EventStringId::nextAnimate, sf::Vector2f(jInfo.at(i)["position"][0], jInfo.at(i)["position"][1]));
                        e.timestamp = gameTimeFrame;
                        EventManager::raise(e);
                    }


                   // objectList.at(i - 1)->serverUpdate(jInfo.at(i));
                }
            }
        }
        */



        {
            zmq::message_t update;
            subscriber.recv(&update);
            jInfo = json::parse((char*)update.data());

            std::cout << playerId << " -> Player ID" << std::endl;
            for (int i = 1; i < jInfo.at(0); i++) {
                std::cout << jInfo.at(i)["id"] << std::endl;
            }
        }



        // Tell threads to update
        _loop_variable.notify_all();


        // Check all the window's events that were triggered since the last iteration
        sf::Event event;
        // .pollEvent will return true if an event was pending
        while (window.pollEvent(event)) {
            // "close requested" event: close window
            if (event.type == sf::Event::Closed) {
                running = false;
                // Tell threads to update
                _loop_variable.notify_all();
                window.close();
            }
            if (event.type == sf::Event::LostFocus)
                focused = false;
            if (event.type == sf::Event::GainedFocus)
                focused = true;
            if (focused) {
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && !spacePressed) {
                    spacePressed = true;
                    gameTimeline->pauseToggle();
                    std::cout << "paused" << std::endl;
                }
                else if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
                    spacePressed = false;
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Period) && !speedPressed) {
                    speedPressed = true;
                    gameTimeline->speedUp();
                    std::cout << "speed" << std::endl;
                }
                else if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Period)) {
                    speedPressed = false;
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Comma) && !slowPressed) {
                    slowPressed = true;
                    gameTimeline->slowDown();
                    std::cout << "slow" << std::endl;
                }
                else if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Comma)) {
                    slowPressed = false;
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::R) && !rPressed) {
                    rPressed = true;
                    // Event triggers record start if not yet recording & sends the game time frame
                    Event e = Event(EventStringId::replayRecord);
                    e.timestamp = gameTimeFrame;
                    EventManager::raise(e);
                    std::cout << "record" << std::endl;
                }
                else if (!sf::Keyboard::isKeyPressed(sf::Keyboard::R)) {
                    rPressed = false;
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::T) && !tPressed) {
                    tPressed = true;
                    // Event ends recording and starts the replay
                    Event e = Event(EventStringId::replayTerminate);
                    e.timestamp = gameTimeFrame;
                    EventManager::raise(e);
                    std::cout << "stop recording & start playing recording" << std::endl;
                }
                else if (!sf::Keyboard::isKeyPressed(sf::Keyboard::T)) {
                    tPressed = false;
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::C) && !cPressed) {
                    cPressed = true;
                    // Loads event and handler from script
                    eventScripts.loadScript("scriptEvents.js");
                    Event e = Event(scriptEventId);
                    // This sets the function intended to be used
                    duk_get_prop_string(eventScripts.getContext(), -1, "scriptEvent");
                    // Dukglue's push function let's us push almost any value we want onto the stack
                    dukglue_push(eventScripts.getContext(), &e);
                    // This actually executes the call to the script function
                    if (duk_pcall(eventScripts.getContext(), 1) != 0)
                    {
                        std::cout << "Error: ";
                        std::cout << duk_safe_to_string(eventScripts.getContext(), -1) << std::endl;
                    }
                    e.timestamp = gameTimeFrame;
                    EventManager::raise(e);
                    duk_pop(eventScripts.getContext());
                    std::cout << "load script event" << std::endl;
                }
                else if (!sf::Keyboard::isKeyPressed(sf::Keyboard::C)) {
                    cPressed = false;
                }
            }
        }

        // Clear the current frame of existing elements
        window.clear(sf::Color::Black);

        // Draw all entities here here
        for (AbstractGameObject* o : objectList) {
            window.draw(*o->getModel()->getShape());
        }

        if (!EventManager::getReplaying()) {
            {
                // Update server position
                jInfo.clear();
                jInfo.push_back(playerIndex);
                jInfo.push_back(newPlayer->getUpdateInfo());

                jInfoSize = jInfo.dump().length() + 1;
                request.rebuild(jInfoSize);
                snprintf((char*)request.data(), jInfoSize, "%s", jInfo.dump().c_str());
                if (socketMovement.send(request, ZMQ_DONTWAIT) != -1)
                    socketMovement.recv(&reply);
            }
        }

        // End the current frame
        window.display();
    }

    // Join threads
    threadAnimationOne.join();
    threadAnimationTwo.join();

    // Draw delete all entities
    for (AbstractGameObject* o : objectList) {
        delete o;
    }


    return 0;
}
