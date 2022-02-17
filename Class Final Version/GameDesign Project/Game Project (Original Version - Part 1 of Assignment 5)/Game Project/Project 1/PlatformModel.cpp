/**
* Platform model default settings - provides more customizable features.
* @author Stephen Wang (sywang)
*/
#include "PlatformModel.h"

PlatformModel::PlatformModel(float xloc, float yloc, float length, float width, sf::Color color)
	:AbstractModel()
{
	// Set platform size and color
	this->setSize(sf::Vector2f(length, width));
	this->setFillColor(color);
	// Set the spawn location
	this->setPosition(xloc, yloc);
}

sf::FloatRect PlatformModel::getModelGlobalBounds()
{
	return this->getGlobalBounds();
}

void PlatformModel::moveModel(float x, float y)
{
	this->move(x, y);
}

sf::Vector2f PlatformModel::getModelPosition()
{
	return this->getPosition();
}
