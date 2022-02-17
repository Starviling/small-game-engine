/**
* The model of the object
* @author Stephen Wang (sywang)
*/
#pragma once
#include <SFML\Graphics\Rect.hpp>
#include <SFML\Graphics\Shape.hpp>
class AbstractModel
{
protected:
public:
	// Constructor for the model
	AbstractModel();
	// For retrieval of the global bounds
	virtual sf::FloatRect getModelGlobalBounds() = 0;
	// For the movement of the model
	virtual void moveModel(float x, float y) = 0;
	// For the retrieval of the model position
	virtual sf::Vector2f getModelPosition() = 0;
	// For retrieving the shape to draw
	virtual sf::Shape* getShape() = 0;
};

