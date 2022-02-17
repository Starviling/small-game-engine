/**
* Animation component of a game object. (Moves model according to xTravel and yTravel)
* @author Stephen Wang (sywang)
*/
#include "Animation.h"

/**
* Animation constructor sets up pointers given from object connected
* @param object the object the component is connected to
*/
Animation::Animation(AbstractGameObject* object)
	:AbstractComponent(object)
{
	this->model = object->getModel();
	this->xTravel = object->getXSpeed();
	this->yTravel = object->getYSpeed();
	this->untranslatedXY = object->getUntranslated();
	this->xyTranslation = object->getXYTranslation();
	this->gameTimeFrame = object->getGameTimeFrame();
	this->objectId = object->getId();

}

void Animation::run()
{
	EventManager::handleEvents();
	//model->getShape()->setPosition(untranslatedXY->x + xyTranslation->x + *xTravel,
	//	untranslatedXY->y + xyTranslation->y + *yTravel);
	//untranslatedXY->x += *xTravel;
	//untranslatedXY->y += *yTravel;
	Event e = Event(EventStringId::nextAnimate);
	e.addComponent(EventStringId::componentPointer, this);
	e.addVector(EventStringId::newPosition, sf::Vector2f(untranslatedXY->x + xyTranslation->x + *xTravel,
		untranslatedXY->y + xyTranslation->y + *yTravel));
	e.addVector(EventStringId::oldXYTravel, sf::Vector2f(untranslatedXY->x + *xTravel, untranslatedXY->y + *yTravel));
	e.timestamp = *gameTimeFrame;
	EventManager::raise(e);
}

void Animation::runEvent(sf::Vector2f newPosition, sf::Vector2f untranslated)
{
	//if (objectId == testing) {
	//	std::cout << "Animation:" << std::endl;
	//	std::cout << "old xtravel ytravel" << std::endl;
	//	std::cout << *xTravel << " : " << *yTravel << std::endl;
	//	std::cout << "new xtravel ytravel" << std::endl;
	//	std::cout << untranslated.x << " : " << untranslated.y << std::endl;
	//}
	model->getShape()->setPosition(newPosition.x, newPosition.y);
	untranslatedXY->x = untranslated.x;
	untranslatedXY->y = untranslated.y;

	//model->getShape()->setPosition(sf::Vector2f(untranslatedXY->x + xyTranslation->x + *xTravel,
	//	untranslatedXY->y + xyTranslation->y + *yTravel));
	//untranslatedXY->x += *xTravel;
	//untranslatedXY->y += *yTravel;
}