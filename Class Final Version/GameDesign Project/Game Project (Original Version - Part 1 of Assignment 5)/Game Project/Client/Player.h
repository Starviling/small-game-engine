/**
* @file Player.h
* @author Stephen Wang (sywang)
* Platforms for entities to sit upon. Inherits and utilizes RectangleShape
* to form lines as platform
*/
#include <SFML\Graphics\CircleShape.hpp>
#include "GameTime.h"
#pragma once
class Player : public sf::CircleShape
{
private:
    // Stores properties useful for transformations
    // The speed of the platform moving
    float pSpeed;
    // The orientation of the platform
    float pOrientation;
    // The cycles of the platform in animation to loop through until it turns around
    int pCycleWait;
    // Keeps track of the cycles waited
    int cyclesWaited;

    // x Travel (unit vector value multiplied by speed)
    float xTravel;
    // y Travel (unit vector value multiplied by speed)
    float yTravel;

    // Radius of player
    float pRadius = 25.f;

    // Keeps track of delta time
    long double* framePeriod;
    // Keeps track if focused
    bool* focused;
public:
    // Constructor for player with transformations
    Player(float speed, float orientation, int cycleWait, float xloc, float yloc, long double* framePeriod, bool* focused);
    Player(float orientation, float xloc, float yloc, long double* framePeriod, bool* focused);
    Player(float xloc, float yloc, long double* framePeriod, bool *focused);

    // Updates the position of the player based on the "speed" and the "cycles waited"
    void updatePosition(std::vector<sf::Shape*>* entityList);
    // Changes the speed of the player
    void setSpeed(float newSpeed);
    // Changes the orientation of the player
    void setOrientation(float newOrientation);
    // Changes the cycles waited for the player
    void setCycleWait(int newCycleWait);
    // Handles collision tests for the object
    bool collisionTest(std::vector<sf::Shape*>* entityList, int entities, sf::FloatRect boundingBox, sf::FloatRect* otherBox);

    // Returns the speed of the player
    float getSpeed() { return pSpeed; }
    // Returns the orientation of the player
    float getOrientation() { return pOrientation; }
    // Returns the cycles to wait for the player
    int getCycleWait() { return pCycleWait; }

};