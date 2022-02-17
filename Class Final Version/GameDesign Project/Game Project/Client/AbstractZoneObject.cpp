#include "AbstractZoneObject.h"
#define DEFAULTLENGTH 100.f
#define DEFAULTWIDTH 5.f
#define DEFAULT_ORIENTATION 0.f

AbstractZoneObject::AbstractZoneObject(long double* gameTimeFrame, sf::Vector2f* xyTranslation, std::vector<int>* deleteList)
	: PlatformObject(gameTimeFrame, xyTranslation, deleteList) {}

AbstractZoneObject::AbstractZoneObject(sf::Vector2f start, sf::Color color, long double* framePeriod, long double* gameTimeFrame, bool* focused,
	sf::Vector2f* xyTranslation, std::vector<AbstractGameObject*>* objectList, std::vector<int>* deleteList)
	: AbstractZoneObject(start, color, DEFAULT_ORIENTATION, framePeriod, gameTimeFrame, focused, xyTranslation, objectList, deleteList) {}

AbstractZoneObject::AbstractZoneObject(sf::Vector2f start, sf::Color color, float orientation, long double* framePeriod, long double* gameTimeFrame,
	bool* focused, sf::Vector2f* xyTranslation, std::vector<AbstractGameObject*>* objectList, std::vector<int>* deleteList)
	: AbstractZoneObject(start, color, orientation, start, 0, framePeriod, gameTimeFrame, focused, xyTranslation, objectList, deleteList) {}

AbstractZoneObject::AbstractZoneObject(sf::Vector2f start, sf::Color color, float orientation, sf::Vector2f end, float speed,
	long double* framePeriod, long double* gameTimeFrame, bool* focused, sf::Vector2f* xyTranslation, std::vector<AbstractGameObject*>* objectList, std::vector<int>* deleteList)
	: AbstractZoneObject(start, color, orientation, end, speed, DEFAULTLENGTH, DEFAULTWIDTH, framePeriod, gameTimeFrame, focused, xyTranslation, objectList, deleteList) {}

AbstractZoneObject::AbstractZoneObject(sf::Vector2f start, sf::Color color, float orientation, sf::Vector2f end, float speed,
	float length, float width, long double* framePeriod, long double* gameTimeFrame, bool* focused, sf::Vector2f* xyTranslation, std::vector<AbstractGameObject*>* objectList,
	std::vector<int>* deleteList)
	: PlatformObject(start, color, orientation, end, speed, length, width, framePeriod, gameTimeFrame, focused, xyTranslation, objectList, deleteList) {}
