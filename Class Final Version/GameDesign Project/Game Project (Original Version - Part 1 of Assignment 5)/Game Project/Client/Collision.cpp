/**
* Collision component of the game object updates xTravel and yTravel accordingly
* @author Stephen Wang (sywang)
*/
#include "Collision.h"
#define OBJECTINERTIA 0.25

/**
* Constructor for collision component. Adjusts the xTravel and yTravel according to next location to be after collisions
* @param mainObject the object the component is connected to
*/
Collision::Collision(AbstractGameObject* mainObject)
	:AbstractComponent(mainObject)
{
	this->model = mainObject->getModel();
	this->objectList = mainObject->getOtherInteractable();
	this->xTravel = mainObject->getXSpeed();
	this->yTravel = mainObject->getYSpeed();
	this->focused = mainObject->getFocus();
	this->collision = mainObject->getCollisionBool();
	this->canJump = mainObject->getCanJump();
	this->gameTimeFrame = mainObject->getGameTimeFrame();
	this->playerId = internString("player");
	this->deathId = internString("death");
	this->spawnId = internString("spawn");
	this->translateId = internString("translate");
}

/**
* Runs the collision component
*/
void Collision::run()
{
	//EventManager::handleEvents();
	if (*collision) {
		// Retrieve the bounds of the model
		sf::FloatRect modelBounds;

		// Calculate the locations of bottom and right of the bounds
		float modelBottom = 0;
		float modelRight = 0;
		float otherBottom = 0;
		float otherRight = 0;

		// Declare outside for loop for efficiency
		float tempOne = 0;
		float tempTwo = 0;
		sf::Vector2f tempEventArg = sf::Vector2f(0, 0);

		// Loop through each object in the list
		for (AbstractGameObject* o : *this->objectList) {

			modelBounds = model->getModelGlobalBounds();
			// Set the model bounds to future locations
			modelBounds.top += *(this->yTravel);
			modelBounds.left += *(this->xTravel);
			modelBottom = modelBounds.top + modelBounds.height;
			modelRight = modelBounds.left + modelBounds.width;

			AbstractModel* otherModel = o->getModel();
			// Make sure we aren't colliding with "self"
			if (otherModel != model) {
				sf::FloatRect otherBounds = otherModel->getModelGlobalBounds();

				// Check collision
				if (modelBounds.intersects(otherBounds))
				{
					// For now...
					if (o->getId() == deathId)
					{
						Event e = Event(EventStringId::deathCollision);
						e.addGameObject(EventStringId::gameObjectPointer, object);
						e.addGameObject(EventStringId::otherGameObjectPointer, o);
						e.timestamp = *gameTimeFrame;
						EventManager::raise(e);
						break;
					}
					else if (o->getId() == spawnId)
					{
						Event e = Event(EventStringId::respawnSetCollision);
						e.addGameObject(EventStringId::gameObjectPointer, object);
						e.addGameObject(EventStringId::otherGameObjectPointer, o);
						e.timestamp = *gameTimeFrame;
						EventManager::raise(e);
						break;
					}
					else if (o->getId() == translateId) {
						Event e = Event(EventStringId::translateCollision);
						e.addGameObject(EventStringId::gameObjectPointer, object);
						e.addGameObject(EventStringId::otherGameObjectPointer, o);
						e.timestamp = *gameTimeFrame;
						EventManager::raise(e);
						//AbstractZoneObject* z = dynamic_cast<AbstractZoneObject*> (o);
						//z->trigger(object);
						break;
					}
					else {
						otherBottom = otherBounds.top + otherBounds.height;
						otherRight = otherBounds.left + otherBounds.width;

						tempEventArg.x = 0;
						tempEventArg.y = 0;
						bool eventJump = *canJump;
						// Check for up down collision
						if ((modelBottom > otherBounds.top && modelBounds.top < otherBounds.top) ||
							(modelBounds.top < otherBottom && modelBottom > otherBottom))
						{
							tempOne = otherBounds.top - modelBottom;
							tempTwo = otherBottom - modelBounds.top;

							// Create a new event for a up down collision
							if (std::abs(tempOne) < std::abs(tempTwo)) {
								eventJump = true;
								//*(this->yTravel) += tempOne;
								tempEventArg.y += tempOne;
							}
							else {
								//*(this->yTravel) += tempTwo;
								tempEventArg.y += tempTwo;
							}

						}
						// If the model is clipping into other from right or left
						else if ((modelRight > otherBounds.left && modelBounds.left < otherBounds.left) ||
							(modelBounds.left < otherRight && modelRight > otherRight))
						{
							tempOne = otherBounds.left - modelRight;
							tempTwo = otherRight - modelBounds.left;
							// Create a new event for a right left collision
							if (std::abs(tempOne) < std::abs(tempTwo)) {
								//*(this->xTravel) += tempOne;
								tempEventArg.x += tempOne;
							}
							else {
								//*(this->xTravel) += tempTwo;
								tempEventArg.x += tempTwo;
							}
						}

						// The other object pushes this object in the direction it's travelling
						// Consult the value used for inertia in player control
						//*(this->xTravel) += *o->getXSpeed() * OBJECTINERTIA;
						tempEventArg.x += *o->getXSpeed() * OBJECTINERTIA;
						//*(this->yTravel) += o->getYSpeed();
						Event e = Event(EventStringId::collisionType);
						e.addComponent(EventStringId::componentPointer, this);
						e.addVector(EventStringId::collisionVector, tempEventArg);
						e.addVector(EventStringId::oldXYTravel, sf::Vector2f(*xTravel, *yTravel));
						e.addBoolean(EventStringId::jumpBoolean, eventJump);
						e.timestamp = *gameTimeFrame;
						EventManager::raise(e);
						//(dynamic_cast<Collision*> (e.getArgument(EventStringId::componentPointer).m_asComponentPtr))
						//	->runEvent(e.getArgument(EventStringId::collisionVector).m_asVector, e.getArgument(EventStringId::oldXYTravel).m_asVector);
					}
				}
			}
		}
	}
}

void Collision::runEvent(sf::Vector2f xyEvent, sf::Vector2f oldTravel, bool jumpBoolean)
{
	//std::cout << "Collision:" << std::endl;
	//std::cout << "current xtravel ytravel" << std::endl;
	//std::cout << *xTravel << " : " << *yTravel << std::endl;
	//std::cout << "old xtravel ytravel" << std::endl;
	//std::cout << oldTravel.x << " : " << oldTravel.y << std::endl;
	//std::cout << "new xtravel ytravel" << std::endl;
	//std::cout << xyEvent.x << " : " << xyEvent.y << std::endl;
	*(this->xTravel) += xyEvent.x;
	*(this->yTravel) += xyEvent.y;
	*canJump = jumpBoolean;
}
