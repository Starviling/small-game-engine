/**
* Player model component - will be used by other components to update positions
* and characteristics.
* @author Stephen Wang (sywang)
*/
#pragma once
#include "AbstractModel.h"
#include <SFML\Graphics\CircleShape.hpp>
class PlayerModel : public AbstractModel, public sf::CircleShape
{
public:
	// Constructor for the player model
	PlayerModel(float xloc, float yloc);
	// Gets the bounds of the model
	sf::FloatRect getModelGlobalBounds();
	// Moves the model in a direction
	void moveModel(float x, float y);
	// Gets the model position
	sf::Vector2f getModelPosition();
	sf::Shape* getShape() { return this; };
};

