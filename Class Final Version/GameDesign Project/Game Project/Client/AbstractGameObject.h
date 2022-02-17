/**
* An abstract game object for other objects to inherit from.
* Following a hybrid model between generic and strict components.
* @author Stephen Wang (sywang)
*/
#pragma once
#include "AbstractComponent.h"
#include "StringId.h"
#include "AbstractModel.h"
#include <vector>
#include <json.hpp>
using namespace nlohmann;

class AbstractComponent;

class AbstractGameObject
{
protected:
	// List of all the components that don't require order for the game object
	std::vector<AbstractComponent*> threadSafeList;
	// List of all the components that require order for the game object
	std::vector<AbstractComponent*> threadUnsafeList;
	// List of all other objects that can interact
	std::vector<AbstractGameObject*>* objectList;

	// Pointer to the list of object indices to delete at the beginning of a loop
	std::vector<int>* deleteList;

	// Object GUID
	StringId objectId;
	// The default speed of the player moving (used by the animation component)
	float speed;
	// x Travel (unit vector value multiplied by speed)
	float xTravel;
	// y Travel (unit vector value multiplied by speed)
	float yTravel;

	// Untranslated x, y coordinates
	sf::Vector2f untranslatedXY;
	sf::Vector2f* xyTranslation;

	// Keep track of pointer to the frame period
	long double* framePeriod;
	// Keep track of pointer to game time frame
	long double* gameTimeFrame;
	// Keep track of if object is focused
	bool* focused;
	// Indicator whether the object can jump
	bool canJump;
	// Keeps track of if the object can be collided with (can it be pushed?)
	bool collisionToggle;

	// Updated every gameloop to handle deletion and creation of new objects
	int indexPosition;
public:
	AbstractGameObject(long double* framePeriod, long double* gameTimeFrame, bool* focused, bool collision, sf::Vector2f* xyTranslation,
		std::vector<AbstractGameObject*>* objectList, std::vector<int>* deleteList);
	// Have all child classes call upon this destructor too
	virtual ~AbstractGameObject();

	// Runs all the components within the list
	virtual void runThreadSafeComp();
	// Runs all the components within the list
	virtual void runThreadUnsafeComp();

	// Returns the model of the game object
	virtual AbstractModel* getModel() = 0;

	// Update the speed
	virtual void setSpeed(float newSpeed);
	// Update the position
	void setXYSpeed(float xSpeed, float ySpeed);
	// Toggle collision of object
	void toggleCollision();
	// Sets the new position of the object
	void setPosition(sf::Vector2f position);

	// Sets the index position of the object
	void setIndex(int index);
	int getIndex() { return indexPosition; }

	// Getters for components to modify
	// Returns the speed of the object pointer
	float* getSpeed() { return &speed; }
	// Returns the speed of the object pointer
	float* getXSpeed() { return &xTravel; }
	// Returns the speed of the object pointer
	float* getYSpeed() { return &yTravel; }
	// Returns the untranslated coordinates pointer
	sf::Vector2f* getUntranslated() { return &untranslatedXY; }
	// Returns the translation coordinates pointer
	sf::Vector2f* getXYTranslation() { return xyTranslation; }
	// Keep track of pointer to the frame period
	long double* getFramePeriod() { return framePeriod; }
	long double* getGameTimeFrame() { return gameTimeFrame; }
	// Keep track of if object is focused
	bool* getFocus() { return focused; }
	// Return the collision toggle
	bool* getCollisionBool() { return &collisionToggle; }
	// Return the bool pointer for canJump
	bool* getCanJump() { return &canJump; }
	// Return the pointer to the interactable game objects
	std::vector<AbstractGameObject*>* getOtherInteractable() { return objectList; }

	// Set up pointer to indices to delete
	void setDeleteList(std::vector<int>* deleteList);
	std::vector<int>* getDeleteList() { return deleteList; }

	// Returns the ID of the object
	StringId getId() { return objectId; }

	// Generates information about game object for server to pass to clients
	virtual json getUpdateInfo() = 0;
	// Retrieves information from server update and updates object
	virtual void serverUpdate(json updateInfo) = 0;
};

