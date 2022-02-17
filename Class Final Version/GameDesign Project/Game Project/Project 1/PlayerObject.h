/**
* Player game object - Contains the components and makeup of a player game object.
* @author Stephen Wang (sywang)
*/
#pragma once
#include "AbstractGameObject.h"
#include "Animation.h"
#include "PlayerModel.h"
#include "Collision.h"
#include "PlayerControl.h"
#include "Gravity.h"

class PlayerObject : public AbstractGameObject
{
private:
	// The player model
	PlayerModel *pModel;
	// Spawn point
	sf::Vector2f spawnLocation;
	// Spawn translation
	sf::Vector2f spawnTranslation;

public:
	// Constructor for a player object not controlled by client
	PlayerObject(float xloc, float yloc, long double* gameTimeFrame, sf::Vector2f* xyTranslation, std::vector<int>* deleteList);

	// Constructor for the player object
	PlayerObject(float xloc, float yloc, long double* framePeriod, long double* gameTimeFrame, bool* focused,
		std::vector<AbstractGameObject*>* objectList, sf::Vector2f* xyTranslation, std::vector<int>* deleteList);

	// Have all child classes call upon this destructor too
	virtual ~PlayerObject();

	// Sets the spawn point of the player object
	void setSpawn(sf::Vector2f spawn, sf::Vector2f spawnTranslation);
	// "Kills" the player (return the player back to the previous spawn point)
	void killPlayer();
	// Return the model for the window to draw or to access position
	AbstractModel* getModel() { return pModel; }

	// Set the speed of the player
	virtual void setSpeed(float newSpeed);
	// Generates information about game object for server to pass to clients
	virtual json getUpdateInfo();
	// Retrieves information from server update and updates object
	virtual void serverUpdate(json updateInfo);
};

