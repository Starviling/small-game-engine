/**
* Platform object holds model for platform
* @author Stephen Wang (sywang)
*/
#pragma once
#include "AbstractGameObject.h"
#include "Animation.h"
#include "PlatformModel.h"
#include "PlatformCycle.h"

class PlatformObject : public AbstractGameObject
{
protected:
	// The platform model
	PlatformModel* pModel;

public:
	// Spawns a platform with no details
	PlatformObject(long double* gameTimeFrame, sf::Vector2f* xyTranslation, std::vector<int>* deleteList);
	// The most simple constructor for the platform object
	PlatformObject(sf::Vector2f start, sf::Color color, long double* framePeriod, long double* gameTimeFrame, bool* focused, sf::Vector2f* xyTranslation,
		std::vector<AbstractGameObject*>* objectList, std::vector<int>* deleteList);
	// Simple constructor for the platform object
	PlatformObject(sf::Vector2f start, sf::Color color, float orientation, long double* framePeriod, long double* gameTimeFrame, bool* focused, sf::Vector2f* xyTranslation,
		std::vector<AbstractGameObject*>* objectList, std::vector<int>* deleteList);
	// Constructor for the platform object
	PlatformObject(sf::Vector2f start, sf::Color color, float orientation, sf::Vector2f end, float speed,
		long double* framePeriod, long double* gameTimeFrame, bool* focused, sf::Vector2f* xyTranslation, std::vector<AbstractGameObject*>* objectList,
		std::vector<int>* deleteList);
	// Complicated constructor for the platform object
	PlatformObject(sf::Vector2f start, sf::Color color, float orientation, sf::Vector2f end, float speed,
		float length, float width, long double* framePeriod, long double* gameTimeFrame, bool* focused, sf::Vector2f* xyTranslation,
		std::vector<AbstractGameObject*>* objectList, std::vector<int>* deleteList);

	virtual ~PlatformObject();

	// Provides the platform a behavior to execute when collided with


	// Return the model for the window to draw or to access position
	AbstractModel* getModel() { return pModel; }
	// Setters
	void setOrientation(float orientation);
	void setSize(float length, float width);

	// Generates information about game object for server to pass to clients
	virtual json getUpdateInfo();
	// Retrieves information from server update and updates object
	virtual void serverUpdate(json updateInfo);
};

