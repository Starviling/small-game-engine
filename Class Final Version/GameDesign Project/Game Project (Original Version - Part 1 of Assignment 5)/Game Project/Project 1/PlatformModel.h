/**
* Platform model component - will be used by other components to update positions
* and characteristics.
* @author Stephen Wang (sywang)
*/
#pragma once
#include <SFML\Graphics\RectangleShape.hpp>
#include "AbstractModel.h"

class PlatformModel : public AbstractModel, public sf::RectangleShape
{
public:
	// Constructor for the player model
	PlatformModel(float xloc, float yloc, float length, float width, sf::Color color);
	// Gets the bounds of the model
	sf::FloatRect getModelGlobalBounds();
	// Moves the model in a direction
	void moveModel(float x, float y);
	// Gets the model position
	sf::Vector2f getModelPosition();
	sf::Shape* getShape() { return this; };
};

