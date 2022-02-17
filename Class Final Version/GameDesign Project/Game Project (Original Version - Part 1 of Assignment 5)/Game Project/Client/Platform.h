/**
* @file platform.h
* @author Stephen Wang (sywang)
* Platforms for entities to sit upon. Inherits and utilizes RectangleShape
* to form lines as platform
*/
#include <SFML\Graphics\RectangleShape.hpp>
#include "GameTime.h"
#pragma once
class Platform : public sf::RectangleShape
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

    // Length of platform
    float pLength = 100.f;
    // Width of platform
    float pWidth = 5.f;

    // Keeps track of time
    long double* framePeriod;

public:
    // Constructor for platform with transformations
    Platform(float speed, float orientation, int cycleWait, float xloc, float yloc, long double* framePeriod);
    Platform(float orientation, float xloc, float yloc, long double* framePeriod);
    Platform(float xloc, float yloc, long double* framePeriod);

    // Updates the position of the platform
    void updatePosition();
    // Updates the position of the player based on the "speed" and the "cycles waited"
    void updatePlayerPosition(std::vector<sf::Shape*>* entityList);
    // Changes the speed of the platform
    void setSpeed(float newSpeed);
    // Changes the orientation of the platform
    void setOrientation(float newOrientation);
    // Changes the cycles waited for the platform
    void setCycleWait(int newCycleWait);
    // Changes the size of the platform
    void setNewSize(float length, float width);
    // Test collisions with entities that can be collided with
    bool collisionTest(std::vector<sf::Shape*>* entityList, bool players[10]);

    // Returns the speed of the platform
    float getSpeed() { return pSpeed; }
    // Returns the orientation of the platform
    float getOrientation() { return pOrientation; }
    // Returns the cycles to wait for the platform
    int getCycleWait() { return pCycleWait; }
};