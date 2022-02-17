/**
* The platform object. Contains Animation and cycle components.
* @author Stephen Wang (sywang)
*/
#include "PlatformObject.h"
#define DEFAULTLENGTH 100.f
#define DEFAULTWIDTH 5.f
#define DEFAULT_ORIENTATION 0.f
#define DEFAULT_XPOSITION 0.f
#define DEFAULT_YPOSITION 0.f

PlatformObject::PlatformObject(long double* gameTimeFrame, sf::Vector2f* xyTranslation)
	:AbstractGameObject(NULL, gameTimeFrame, NULL, false, xyTranslation, NULL)
{
	this->pModel = new PlatformModel(DEFAULT_XPOSITION, DEFAULT_YPOSITION, 0, 0, sf::Color(0, 0, 0));
	// These untranslatedXY have to be the same as the starting model position
	this->untranslatedXY.x = DEFAULT_XPOSITION;
	this->untranslatedXY.y = DEFAULT_YPOSITION;
	setSpeed(0);
	setXYSpeed(0, 0);
	setOrientation(0);

	this->objectId = internString("platform");
}

PlatformObject::PlatformObject(sf::Vector2f start, sf::Color color, long double* framePeriod, long double* gameTimeFrame, bool* focused, sf::Vector2f* xyTranslation,
	std::vector<AbstractGameObject*>* objectList)
	: PlatformObject(start, color, DEFAULT_ORIENTATION, framePeriod, gameTimeFrame, focused, xyTranslation, objectList) {}

PlatformObject::PlatformObject(sf::Vector2f start, sf::Color color, float orientation, long double* framePeriod, long double* gameTimeFrame, bool* focused, sf::Vector2f* xyTranslation,
	std::vector<AbstractGameObject*>* objectList)
	: PlatformObject(start, color, orientation, start, 0, framePeriod, gameTimeFrame, focused, xyTranslation, objectList) {}

PlatformObject::PlatformObject(sf::Vector2f start, sf::Color color, float orientation, sf::Vector2f end, float speed,
	long double* framePeriod, long double* gameTimeFrame, bool* focused, sf::Vector2f* xyTranslation, std::vector<AbstractGameObject*>* objectList)
	: PlatformObject(start, color, orientation, end, speed, DEFAULTLENGTH, DEFAULTWIDTH, framePeriod, gameTimeFrame, focused, xyTranslation, objectList) {}

/**
* Constructor for the platform object
* @param start the starting position of the platform
* @param orientation the orientation of the platform
* @param end the ending position of the platform
* @param newSpeed the new speed of the platform upon creation
* @param length the length of the platform being created
* @param width the width of the platform being created
* @param framePeriod the pointer to the frame period of the game
* @param focused the pointer to indicate whether the game is focused or not
*/
PlatformObject::PlatformObject(sf::Vector2f start, sf::Color color, float orientation, sf::Vector2f end, float newSpeed,
	float length, float width, long double* framePeriod, long double* gameTimeFrame, bool* focused, sf::Vector2f* xyTranslation, std::vector<AbstractGameObject*>* objectList)
	:AbstractGameObject(framePeriod, gameTimeFrame, focused, false, xyTranslation, objectList)
{
	this->pModel = new PlatformModel(start.x, start.y, length, width, color);
	this->untranslatedXY.x = start.x;
	this->untranslatedXY.y = start.y;
	setSpeed(newSpeed);
	setXYSpeed(0, 0);
	setOrientation(orientation);

	this->objectId = internString("platform");

	// Add a cycle component
	this->threadUnsafeList.push_back(new PlatformCycle(this,start,end));
	// Add an animation component to the platform
	this->threadUnsafeList.push_back(new Animation(this));

}

PlatformObject::~PlatformObject()
{
	delete pModel;
}

/**
* Sets the orientation of the platform
* @param orientation the new orientation of the platform
*/
void PlatformObject::setOrientation(float orientation)
{
	this->pModel->setRotation(orientation);
}

/**
* Sets the size of the platform
* @param length the new length of the platform being set
* @param width the new width of the platform being set
*/
void PlatformObject::setSize(float length, float width)
{
	this->pModel->setSize(sf::Vector2f(length, width));
}

/**
* Returns the json containing updated info on the object
* @return json containing updated info
*/
json PlatformObject::getUpdateInfo()
{
	sf::Vector2f position = pModel->getPosition();
	sf::Vector2f size = pModel->getSize();
	sf::Color color = pModel->getFillColor();

	json update = {
		{"id", objectId},
		{"position", {untranslatedXY.x, untranslatedXY.y}},
		{"xyTravel" , {xTravel, yTravel}},
		{"orientation", pModel->getRotation()},
		{"size", {size.x, size.y}},
		{"color", {color.r, color.g, color.b}}
	};
	return update;
}

/**
* Updates the object with the given json information
* @param updateInfo the json containing update information
*/
void PlatformObject::serverUpdate(json updateInfo)
{
	// Update the position
	untranslatedXY.x = updateInfo["position"][0];
	untranslatedXY.y = updateInfo["position"][1];
	pModel->setPosition(untranslatedXY.x + xyTranslation->x, untranslatedXY.y + xyTranslation->y);





	// Update the XY travel
	xTravel = updateInfo["xyTravel"][0];
	yTravel = updateInfo["xyTravel"][1];
	// Update the orientation of the object
	pModel->setRotation(updateInfo["orientation"]);
	// Update the size of the object
	pModel->setSize(sf::Vector2f(updateInfo["size"][0], updateInfo["size"][1]));
	// Update the color of the object
	pModel->setFillColor(sf::Color(updateInfo["color"][0], updateInfo["color"][1], updateInfo["color"][2]));
}
