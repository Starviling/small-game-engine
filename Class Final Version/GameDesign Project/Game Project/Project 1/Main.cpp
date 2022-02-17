/**
* @file Main.cpp
* @author Stephen Wang (sywang)
* Game server-client - Server acts as a client that provides information to other clients
*/
#include <SFML/Graphics.hpp>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <iostream>
#include <zmq.hpp>
#include <json.hpp>
#include "StringId.h"
#include "RealTime.h"
#include "GameTime.h"
#include "AbstractGameObject.h"
#include "PlatformObject.h"
#include "PlayerObject.h"
#include "ClientThread.h"
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

#define INVADERSQUARE 30
#define INVADERINROW 10

/**
* Print string function to demonstrate script capabilities
* @param message the message to print
*/
void printString(std::string message)
{
    std::cout << message << std::endl;
}

/**
* Run wrapper for threads
* @param fe the thread being wrapped for run
*/
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
    RealTime *realTimeline = new RealTime();
    GameTime *gameTimeline = new GameTime(realTimeline);
    GameTime *serverTimeline = new GameTime(realTimeline);

    // Booleans for whether buttons were pressed for the window
    bool spacePressed = false;
    bool speedPressed = false;
    bool slowPressed = false;
    bool rPressed = false;
    bool tPressed = false;
    bool cPressed = false;
    bool fPressed = false;

    // Set up variables to point to in objects
    long double framePeriod = 0;
    long double gameTimeFrame = gameTimeline->getTime() * gameTimeline->getTic();
    bool focused = true;
    bool running = true;
    sf::Vector2f xyTranslation(0, 0);

    // Create threads to handle animations
    // Mutex for accessing entities
    std::mutex _mutex;
    std::condition_variable _loop_variable;
    std::condition_variable _update_variable;
    std::condition_variable _main_variable;

    // Set up the pointer to list for objects to access
    std::vector<AbstractGameObject*> objectList;
    // Keep track of a vector that deletes entities at prior to each game loop
    std::vector<int> deleteList;
    // Player object
    PlayerObject* newPlayer = new PlayerObject(width / 2, height / 1.35, &framePeriod, &gameTimeFrame, &focused, &objectList, &xyTranslation, &deleteList);

    // Invaders
    for (int i = 1; i <= INVADERINROW; i++) {
        DeathZoneObject* newInvader = new DeathZoneObject(sf::Vector2f(width / INVADERINROW * i - 30, 0), sf::Color(0, 255, 0), 90,
            sf::Vector2f(width / INVADERINROW * i, 0), 1, INVADERSQUARE, INVADERSQUARE, &framePeriod, &gameTimeFrame, &focused, &xyTranslation, &objectList, &deleteList);
        DeathZoneObject* newInvader2 = new DeathZoneObject(sf::Vector2f(width / INVADERINROW * i - 30, 60), sf::Color(0, 255, 0), 90,
            sf::Vector2f(width / INVADERINROW * i, 60), 1, INVADERSQUARE, INVADERSQUARE, &framePeriod, &gameTimeFrame, &focused, &xyTranslation, &objectList, &deleteList);

        // Turn on collision to allow them to die to projectiles
        newInvader->toggleCollision();
        newInvader2->toggleCollision();
        // Push all my objects into a list
        objectList.push_back(newInvader);
        objectList.push_back(newInvader2);
    }



    // Set up death zone
    //DeathZoneObject* newDeathZone = new DeathZoneObject(sf::Vector2f(-1000, 525), sf::Color(0, 0, 0, 0), &framePeriod, &gameTimeFrame, &focused, &xyTranslation, &objectList);
    DeathZoneObject* newDeathZone = new DeathZoneObject(&gameTimeFrame, &xyTranslation, &deleteList);
    newDeathZone->setSize(4000, 200);
    newDeathZone->setPosition(sf::Vector2f(-1000, 525));
    newDeathZone->setDeathZoneId(internString("planet"));

    DeathZoneObject* newDeathZone2 = new DeathZoneObject(&gameTimeFrame, &xyTranslation, &deleteList);
    //newDeathZone2->setSize(4000, 200);
    //newDeathZone2->setPosition(sf::Vector2f(-1000, -210));

    newDeathZone2->setSize(4000, 20);
    newDeathZone2->setPosition(sf::Vector2f(-1000, -210));
    newDeathZone2->setDeathZoneId(internString("space"));

    PlatformObject* newWall = new PlatformObject(sf::Vector2f(10, 0), sf::Color(0, 0, 255, 255), 90, sf::Vector2f(10, 0), 0, height, 5, &framePeriod, &gameTimeFrame, &focused, &xyTranslation,
        &objectList, &deleteList);
    PlatformObject* newWall2 = new PlatformObject(sf::Vector2f(width - 10, 0), sf::Color(0, 0, 255, 255), 90, sf::Vector2f(width - 10, 0), 0, height, 5, &framePeriod, &gameTimeFrame, &focused, &xyTranslation,
        &objectList, &deleteList);

    // All zones should be defined after platforms or else you may encounter a scenario of clipping through walls when
    // inside a zone
    objectList.push_back(newDeathZone);
    objectList.push_back(newDeathZone2);
    objectList.push_back(newWall);
    objectList.push_back(newWall2);
    objectList.push_back(newPlayer);



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



    //  Prepare our context and publisher
    zmq::context_t context(1);
    zmq::socket_t publisher(context, ZMQ_PUB);
    publisher.bind("tcp://*:5556");

    std::mutex _mutexJoin;
    std::condition_variable _client_iter_variable;
    ClientThread joinThread(&_mutexJoin, &_mutex, &_client_iter_variable, &context, &objectList,
        &running, &framePeriod, &gameTimeFrame, &xyTranslation, &deleteList);
    std::thread joinHandler(run_wrapper, &joinThread);



    // The threads are run in a way such that safe components are run before unsafe components.
    // Perhaps can fix threads to run concurrently better by having the components better organized between thread
    // safe accesses. (Components that adjust speed rely on each other - so be careful)
    GameObjectThread tObjects(&objectList, &_mutex, &_loop_variable, &_update_variable, &running, gameTimeline);
    GameObjectThreadUnsafe tObjectu(&objectList, &_mutex, &_update_variable, &_main_variable, &running, gameTimeline);
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
    long double tempTic = 1;

    // Set the index of all the objects
    int idxUpdate = 0;
    for (AbstractGameObject* o : objectList) {
        o->setIndex(idxUpdate++);
    }
    StringId projectileId = internString("projectile");

    // Keep the program running as long as the window is open
    while (window.isOpen()) {
        {
            std::unique_lock<std::mutex> lock(_mutex);
            // Handle deletion of objects
            sort(deleteList.begin(), deleteList.end(), std::greater<int>());
            deleteList.erase(unique(deleteList.begin(), deleteList.end()), deleteList.end());
            for (int delIndex : deleteList) {
                if (delIndex >= 0 && delIndex < objectList.size()) {
                    std::cout << delIndex;
                    delete objectList.at(delIndex);
                    objectList.at(delIndex) = nullptr;
                    objectList.erase(objectList.begin() + delIndex);
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

        // Reload script
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

        // Check all the window's events that were triggered since the last iteration
        sf::Event event;
        // .pollEvent will return true if an event was pending
        while (window.pollEvent(event)) {

            // "close requested" event: close window
            if (event.type == sf::Event::Closed) {
                running = false;
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
                    EventManager::setTicChangeTime(gameTimeFrame);
                    std::cout << "speed" << std::endl;
                }
                else if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Period)) {
                    speedPressed = false;
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Comma) && !slowPressed) {
                    slowPressed = true;
                    gameTimeline->slowDown();
                    EventManager::setTicChangeTime(gameTimeFrame);
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
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::F) && !fPressed)
                {
                    std::unique_lock<std::mutex> lock(_mutex);
                    fPressed = true;
                    sf::Vector2f playerPos = sf::Vector2f(*(newPlayer->getUntranslated()));
                    playerPos.x += 22.5;
                    DeathZoneObject* projectile = new DeathZoneObject(playerPos, sf::Color(255, 0, 0, 255), &framePeriod, &gameTimeFrame,
                        &focused, &xyTranslation, &objectList, &deleteList);
                    projectile->setDeathZoneId(projectileId);
                    projectile->setSize(5, 15);
                    projectile->setIndex(objectList.size());
                    projectile->toggleCollision();
                    objectList.push_back(projectile);
                    std::cout << "FIRE" << std::endl;
                    std::cout << objectList.size() << std::endl;
                }
                else if (!sf::Keyboard::isKeyPressed(sf::Keyboard::F)) {
                    fPressed = false;
                }
            }
        }

        {
            std::unique_lock<std::mutex> lock(_mutexJoin);
            std::unique_lock<std::mutex> lock2(_mutex);


            json jInfo;
            jInfo.push_back(objectList.size());
            for (AbstractGameObject* o : objectList) {
                jInfo.push_back(o->getUpdateInfo());
            }
            // Send delete information
            jInfo.push_back(deleteList);

            int jInfoSize = jInfo.dump().length() + 1;
            zmq::message_t message(jInfoSize);
            snprintf((char*)message.data(), jInfoSize, "%s", jInfo.dump().c_str());
            publisher.send(message);
            //_client_iter_variable.notify_all();
        }
        // Clear the deleteList now that information has been sent
        deleteList.clear();

        // Tell threads to update
        _loop_variable.notify_all();

        // Clear the current frame of existing elements
        window.clear(sf::Color::Black);


        // Draw all entities here here
        for (AbstractGameObject* o : objectList) {
            window.draw(*o->getModel()->getShape());
        }

        // End the current frame
        window.display();
    }

    // Join threads
    threadAnimationOne.join();
    threadAnimationTwo.join();
    joinHandler.join();

    // Draw delete all entities
    for (AbstractGameObject* o : objectList) {
        delete o;
    }

    return 0;
}
