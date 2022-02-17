/**
* @file platform.cpp
* @author Stephen Wang (sywang)
* Platforms for entities to sit upon. Inherits and utilizes RectangleShape
* to form lines as platform
*/
#include "platform.h"
#define _USE_MATH_DEFINES
#define PLAYERLIMIT 10
#include <math.h>
#include <SFML/Graphics/Texture.hpp>

/**
* Main constructor handles speed, orientation, cycles to wait, starting xloc,
* starting yloc, and initiating other needed variables.
* @param speed the speed of the platform to travel on the screen
* @param orientation the orientation of the platform on the screen in degrees
* @param cycleWait the number of cycles of the ingame loop to wait before end of animation
* @param xloc the starting x location of the platform
* @param yloc the starting y location of the platform
*/
Platform::Platform(float speed, float orientation, int cycleWait, float xloc, float yloc, long double* framePeriod)
{
	// Set line size and color
	this->setNewSize(pLength, pWidth);
	this->setSpeed(speed);
	this->setOrientation(orientation);
	this->setCycleWait(cycleWait);
	this->cyclesWaited = 0;
	this->setPosition(xloc, yloc);
	
	this->setFillColor(sf::Color(100, 250, 50));

	this->framePeriod = framePeriod;
	//sf::Texture texture;
	//if (!texture.loadFromFile("TempTexture.jpg"))
	//{
	//	this->setFillColor(sf::Color(100, 250, 50));
	//}
	//else {
	//	this->setTexture(&texture); // texture is a sf::Texture
	//	this->setTextureRect(sf::IntRect(10, 10, 100, 100));
	//}
}

/**
* Overloaded constructor handles only x location and y location.
* @param xloc the starting x location of the platform
* @param yloc the starting y location of the platform
*/
Platform::Platform(float xloc, float yloc, long double* framePeriod) :
	Platform(0, 0, 0, xloc, yloc, framePeriod) {}

/**
* Overloaded constructor handles only orientation, x location, and y location.
* @param orientation the orientation of the platform in degrees
* @param xloc the starting x location of the platform
* @param yloc the starting y location of the platform
*/
Platform::Platform(float orientation, float xloc, float yloc, long double* framePeriod) :
	Platform(0, orientation, 0, xloc, yloc, framePeriod) {}

/**
* Sets the speed of the platform.
* @param newSpeed the new speed of the platform
*/
void Platform::setSpeed(float newSpeed)
{
	this->pSpeed = newSpeed;
	double yTemp = sin(getOrientation() * M_PI / 180) * (double)pLength;
	double xTemp = sqrt(pow((double)pLength, 2) - pow(yTemp, 2));

	yTravel = pSpeed * (float)(yTemp / (sqrt(pow(xTemp, 2) + pow(yTemp, 2))));
	xTravel = pSpeed * (float)(xTemp / (sqrt(pow(xTemp, 2) + pow(yTemp, 2))));
}

/**
* Sets the orientation of the platform. Recalculates the distance to travel
* in y and x directions.
* @param newOrientation the new orientation of the platform
*/
void Platform::setOrientation(float newOrientation)
{
	this->setRotation(newOrientation);
	this->pOrientation = newOrientation;
	double yTemp = sin(getOrientation() * M_PI / 180) * (double) pLength;
	double xTemp = sqrt(pow((double) pLength, 2) - pow(yTemp, 2));

	yTravel = pSpeed * (float) (yTemp / (sqrt(pow(xTemp, 2) + pow(yTemp, 2))));
	xTravel = pSpeed * (float) (xTemp / (sqrt(pow(xTemp, 2) + pow(yTemp, 2))));
}

/**
* Sets the new cycles to wait for the platform
* @param newCycleWait the new cycles to wait for the platform
*/
void Platform::setCycleWait(int newCycleWait)
{
	this->pCycleWait = newCycleWait;
}

/**
* Sets the new size of the platform
* @param length the length of the platform
* @param width the width of the platform
*/
void Platform::setNewSize(float length, float width)
{
	pLength = length;
	pWidth = width;
	this->setSize(sf::Vector2f(pLength, pWidth));
}

/**
* Updates the position of the platform
* @param entityList the list of entities that can be collided with
* @param entities the number of entities in the list
*/
void Platform::updatePosition()
{
	if (cyclesWaited > getCycleWait()) {
		xTravel *= -1;
		yTravel *= -1;
		cyclesWaited = 0;
	}
	this->move(xTravel * (float)*framePeriod, yTravel * (float)*framePeriod);

	// Needs a new way of representing cycled animations
	// TODO:
	cyclesWaited += (int)(pSpeed * (float) *framePeriod);
}

/**
* Updates the position of the platform
* @param entityList the list of entities that can be collided with
* @param entities the number of entities in the list
*/
void Platform::updatePlayerPosition(std::vector<sf::Shape*>* entityList)
{
	int entities = entityList->size();

	bool *players = new bool[entities];
	for (int i = 0; i < entities; i++) {
		players[i] = false;    // Initialize all elements to zero.
	}

	if (collisionTest(entityList, players)) {
		for (int i = 0; i < entities; i++) {
			if (players[i])
				(*entityList)[i]->move(xTravel * (float)*framePeriod, yTravel * (float)*framePeriod);
		}
	}
	delete[] players;
	players = NULL;
}

/**
* Determines whether or not there is a collision with another object
* @param entityList list of entities that can be collided with
* @param entities the number of entities in the list
* @param boundingBox the boundingBox of the platform
* @param otherBox the boundingBox of the entity to be returned if collision is found
* @return true if collision, false otherwise
*/
bool Platform::collisionTest(std::vector<sf::Shape*>* entityList, bool players[10]) {
	int entities = entityList->size();
	// Get the bounding box of the entity
	sf::FloatRect boundingBox = this->getGlobalBounds();
	for (int i = 0; i < entities; i++) {
		// get the bounding box of the entity
		sf::FloatRect entityBounds = (*entityList)[i]->getGlobalBounds();
		if (boundingBox.intersects(entityBounds)) {
			players[i] = true;
			return true;
		}
	}
	return false;
}