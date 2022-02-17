#include "TranslateZoneObject.h"
#define DEFAULTLENGTH 100.f
#define DEFAULTWIDTH 5.f
#define DEFAULT_ORIENTATION 0.f

TranslateZoneObject::TranslateZoneObject(long double* gameTimeFrame, sf::Vector2f* xyTranslation)
	: TranslateZoneObject(gameTimeFrame, xyTranslation, 0, 0) {}

TranslateZoneObject::TranslateZoneObject(long double* gameTimeFrame, sf::Vector2f* xyTranslation, int translateX, int translateY)
	: AbstractZoneObject(gameTimeFrame, xyTranslation)
{
	this->getModel()->getShape()->setFillColor(sf::Color(0, 0, 0, 0));
	this->targetPlayerId = internString("player");
	this->objectId = internString("translate");
	this->translateX = translateX;
	this->translateY = translateY;
}

TranslateZoneObject::TranslateZoneObject(sf::Vector2f start, sf::Color color, long double* framePeriod, long double* gameTimeFrame, bool* focused,
	sf::Vector2f* xyTranslation, std::vector<AbstractGameObject*>* objectList, int translateX, int translateY)
	: AbstractZoneObject(start, color, DEFAULT_ORIENTATION, framePeriod, gameTimeFrame, focused, xyTranslation, objectList)
{
	this->targetPlayerId = internString("player");
	this->objectId = internString("translate");
	this->translateX = translateX;
	this->translateY = translateY;
}

void TranslateZoneObject::trigger(AbstractGameObject* target)
{
	translateObjects(translateX, translateY);
}

void TranslateZoneObject::translateObjects(int x, int y)
{
	this->xyTranslation->x = (float)x;
	this->xyTranslation->y = (float)y;
}

json TranslateZoneObject::getUpdateInfo()
{
	sf::Vector2f position = pModel->getPosition();
	sf::Vector2f size = pModel->getSize();
	sf::Color color = pModel->getFillColor();

	json update = {
		{"id", objectId},
		{"position", {untranslatedXY.x, untranslatedXY.y}},
		{"xyTravel" , {xTravel, yTravel}},
		{"xyTranslate", {translateX, translateY}},
		{"orientation", pModel->getRotation()},
		{"size", {size.x, size.y}},
		{"color", {color.r, color.g, color.b}}
	};
	return update;
}

void TranslateZoneObject::serverUpdate(json updateInfo)
{
	// Update the position
	untranslatedXY.x = updateInfo["position"][0];
	untranslatedXY.y = updateInfo["position"][1];
	pModel->setPosition(untranslatedXY.x + xyTranslation->x, untranslatedXY.y + xyTranslation->y);

	// Update the XY travel
	xTravel = updateInfo["xyTravel"][0];
	yTravel = updateInfo["xyTravel"][1];

	translateX = updateInfo["xyTranslate"][0];
	translateY = updateInfo["xyTranslate"][1];

	// Update the orientation of the object
	pModel->setRotation(updateInfo["orientation"]);
	// Update the size of the object
	pModel->setSize(sf::Vector2f(updateInfo["size"][0], updateInfo["size"][1]));
	// Update the color of the object
	pModel->setFillColor(sf::Color(updateInfo["color"][0], updateInfo["color"][1], updateInfo["color"][2]));
}
