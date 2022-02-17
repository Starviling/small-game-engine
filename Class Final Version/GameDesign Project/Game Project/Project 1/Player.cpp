/**
* @file Player.cpp
* @author Stephen Wang (sywang)
* Character model for player to control. Inherits and utilizes CircleShape
* to act as character
*/
#include "Player.h"
#define _USE_MATH_DEFINES
#define DEFAULTSPEED 2
#include <math.h>
#include <SFML/Graphics/Texture.hpp>
#include <SFML\Window\Keyboard.hpp>
#include "GameTime.h"

/**
* Main constructor handles speed, orientation, cycles to wait, starting xloc,
* starting yloc, and initiating other needed variables.
* @param speed the speed of the player to travel on the screen
* @param orientation the orientation of the player on the screen in degrees
* @param cycleWait the number of cycles of the ingame loop to wait before end of animation
* @param xloc the starting x location of the player
* @param yloc the starting y location of the player
*/
Player::Player(float speed, float orientation, int cycleWait, float xloc, float yloc, long double* framePeriod, bool* focused)
{
	// Set line size and color
	this->setRadius(pRadius);
	this->setSpeed(speed);
	this->setOrientation(orientation);
	this->setCycleWait(cycleWait);
	this->cyclesWaited = 0;
	this->setPosition(xloc, yloc);
	this->setFillColor(sf::Color(250, 100, 50));
	this->focused = focused;

	this->framePeriod = framePeriod;
	// Texture not currently working ::TODO

	//sf::Texture texture;
	//if (!texture.loadFromFile("./TempTexture.jpg"))
	//{
	//	this->setFillColor(sf::Color(250, 100, 50));
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
Player::Player(float xloc, float yloc, long double* framePeriod, bool* focused) :
	Player(DEFAULTSPEED, 0, 0, xloc, yloc, framePeriod, focused) {}

/**
* Overloaded constructor handles only orientation, x location, and y location.
* @param orientation the orientation of the platform in degrees
* @param xloc the starting x location of the platform
* @param yloc the starting y location of the platform
*/
Player::Player(float orientation, float xloc, float yloc, long double* framePeriod, bool* focused) :
	Player(DEFAULTSPEED, orientation, 0, xloc, yloc, framePeriod, focused) {}

/**
* Sets the speed of the player.
* @param newSpeed the new speed of the player
*/
void Player::setSpeed(float newSpeed)
{
	this->pSpeed = newSpeed;
}

/**
* Sets the orientation of the player. Recalculates the distance to travel
* in y and x directions.
* @param newOrientation the new orientation of the player
*/
void Player::setOrientation(float newOrientation)
{
	this->setRotation(newOrientation);
	this->pOrientation = newOrientation;
	//double yTemp = sin(getOrientation() * M_PI / 180) * (double)pRadius;
	//double xTemp = sqrt(pow((double)pRadius, 2) - pow(yTemp, 2));

	//yTravel = pSpeed * (yTemp / (sqrt(pow(xTemp, 2) + pow(yTemp, 2))));
	//xTravel = pSpeed * (xTemp / (sqrt(pow(xTemp, 2) + pow(yTemp, 2))));
}

/**
* Sets the new cycles to wait for the player
* @param newCycleWait the new cycles to wait for the player
*/
void Player::setCycleWait(int newCycleWait)
{
	this->pCycleWait = newCycleWait;
}

/**
* Updates the position of the player
* @param entityList the list of entities that can be collided with
* @param entities the number of entities in the list
*/
void Player::updatePosition(std::vector<sf::Shape*>* entityList)
{
	// check ahead in direction
	float timedSpeed = pSpeed * (float)*framePeriod;

	if (*focused) {
		int entities = entityList->size();
		// Get the bounding box of the entity
		sf::FloatRect boundingBox = this->getGlobalBounds();
		// Keeping this for future updates on collision
		sf::FloatRect* otherBox = new sf::FloatRect;


		boundingBox.left += -timedSpeed;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)
			&& !(collisionTest(entityList, entities, boundingBox, otherBox)))
		{
			// left key is pressed: move our character
			this->move(-timedSpeed, 0.f);
		}
		boundingBox.left += timedSpeed;
		// check ahead in direction
		boundingBox.width += timedSpeed;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)
			&& !(collisionTest(entityList, entities, boundingBox, otherBox)))
		{
			// left key is pressed: move our character
			this->move(timedSpeed, 0.f);
		}
		boundingBox.width += -timedSpeed;
		boundingBox.top += -timedSpeed;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)
			&& !(collisionTest(entityList, entities, boundingBox, otherBox)))
		{
			// left key is pressed: move our character
			this->move(0.f, -timedSpeed);
		}
		boundingBox.top += timedSpeed;
		boundingBox.height += timedSpeed;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)
			&& !(collisionTest(entityList, entities, boundingBox, otherBox)))
		{
			// left key is pressed: move our character
			this->move(0.f, timedSpeed);
		}
		boundingBox.height -= timedSpeed;

		delete otherBox;
	}

	if (cyclesWaited > getCycleWait()) {
		xTravel *= -1;
		yTravel *= -1;
		cyclesWaited = 0;
	}
	cyclesWaited += (int)(timedSpeed * (float)*framePeriod);
}

/**
* Determines whether or not there is a collision with another object
* @param entityList list of entities that can be collided with
* @param entities the number of entities in the list
* @param boundingBox the boundingBox of the player
* @param otherBox the boundingBox of the entity to be returned if collision is found
* @return true if collision, false otherwise
*/
bool Player::collisionTest(std::vector<sf::Shape*>* entityList, int entities, sf::FloatRect boundingBox, sf::FloatRect* otherBox) {
	for (int i = 0; i < entities; i++) {
		// get the bounding box of the entity
		sf::FloatRect entityBounds = (*entityList)[i]->getGlobalBounds();
		if (boundingBox.intersects(entityBounds)) {
			*otherBox = entityBounds;
			return true;
		}
	}
	return false;
}