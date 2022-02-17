/**
* Player model default settings - provides more customizable.
* @author Stephen Wang (sywang)
*/
#include "PlayerModel.h"
#define PRADIUS 25.f

PlayerModel::PlayerModel(float xloc, float yloc)
	:AbstractModel()
{
	// Set player size and color
	this->setRadius(PRADIUS);
	this->setFillColor(sf::Color(250, 100, 50));
	// Set the spawn location
	this->setPosition(xloc, yloc);
}

sf::FloatRect PlayerModel::getModelGlobalBounds()
{
	return this->getGlobalBounds();
}

void PlayerModel::moveModel(float x, float y)
{
	this->move(x, y);
}

sf::Vector2f PlayerModel::getModelPosition()
{
	return this->getPosition();
}
