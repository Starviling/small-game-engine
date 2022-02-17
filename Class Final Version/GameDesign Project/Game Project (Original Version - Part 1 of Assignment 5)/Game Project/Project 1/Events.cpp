/**
* The event representation
* @author Stephen Wang (sywang)
*/
#include "Events.h"

/**
* Constructor for the event object
* @param eventType the type of event being created
*/
Event::Event(StringId eventType)
{
	this->eventType = eventType;
	this->timestamp = 0;
	this->priority = 0;
}

/**
* Add a game object as an argument
* @param argument the hashed string version of the argument
*/
void Event::addGameObject(StringId argument, AbstractGameObject* gameObject)
{
	Variant argumentData = Variant(Variant::Type::TYPE_GAMEOBJECT_PTR, gameObject);
	this->arguments[argument] = argumentData;
}

void Event::addComponent(StringId argument, AbstractComponent* component)
{
	Variant argumentData = Variant(Variant::Type::TYPE_COMPONENT_PTR, component);
	this->arguments[argument] = argumentData;
}

void Event::addInteger(StringId argument, int integerData)
{
	Variant argumentData = Variant(Variant::Type::TYPE_INT, integerData);
	this->arguments[argument] = argumentData;
}

void Event::addFloat(StringId argument, float floatData)
{
	Variant argumentData = Variant(Variant::Type::TYPE_FLOAT, floatData);
	this->arguments[argument] = argumentData;
}

void Event::addVector(StringId argument, sf::Vector2f vector)
{
	Variant argumentData = Variant(Variant::Type::TYPE_VECTOR2F, vector);
	this->arguments[argument] = argumentData;
}

void Event::addLongDouble(StringId argument, long double longDouble)
{
	Variant argumentData = Variant(Variant::Type::TYPE_LONGDOUBLE, longDouble);
	this->arguments[argument] = argumentData;
}

void Event::addBoolean(StringId argument, bool boolean)
{
	Variant argumentData = Variant(Variant::Type::TYPE_BOOLEAN, boolean);
	this->arguments[argument] = argumentData;
}

AbstractGameObject* Event::getGameObj(StringId argument)
{
	return dynamic_cast<AbstractGameObject*> (arguments[argument].m_asGameObjectPtr);
}

AbstractComponent* Event::getComponentObj(StringId argument)
{
	return dynamic_cast<AbstractComponent*> (arguments[argument].m_asComponentPtr);
}

int Event::getInteger(StringId argument)
{
	return (int)arguments[argument].m_asInteger;
}

float Event::getFloat(StringId argument)
{
	return (float)arguments[argument].m_asFloat;
}

sf::Vector2f Event::getVector(StringId argument)
{
	return (sf::Vector2f) arguments[argument].m_asVector;
}

long double Event::getLongDouble(StringId argument)
{
	return (long double)arguments[argument].m_asLongDouble;
}

bool Event::getBool(StringId argument)
{
	return (bool)arguments[argument].m_asBoolean;
}

/**
* Default constructor for the variant arguments
*/
Variant::Variant()
{
	m_type = Variant::Type::TYPE_VECTOR2F;
	this->m_asVector = sf::Vector2f(0, 0);
}

/**
* Basic constructor for the variant
* @param m_type the type of variant being created
*/
Variant::Variant(Type m_type, AbstractGameObject* m_asGameObjectPtr)
{
	this->m_type = m_type;
	this->m_asLongDouble = 0;
	this->m_asGameObjectPtr = m_asGameObjectPtr;
}

Variant::Variant(Type m_type, AbstractComponent* m_asComponentPtr)
{
	this->m_type = m_type;
	this->m_asLongDouble = 0;
	this->m_asComponentPtr = m_asComponentPtr;
}

Variant::Variant(Type m_type, int m_asInteger)
{
	this->m_type = m_type;
	this->m_asLongDouble = 0;
	this->m_asInteger = m_asInteger;
}

Variant::Variant(Type m_type, float m_asFloat)
{
	this->m_type = m_type;
	this->m_asLongDouble = 0;
	this->m_asFloat = m_asFloat;
}

Variant::Variant(Type m_type, sf::Vector2f m_asVector)
{
	this->m_type = m_type;
	this->m_asVector = m_asVector;
}

Variant::Variant(Type m_type, long double m_asLongDouble)
{
	this->m_type = m_type;
	this->m_asLongDouble = m_asLongDouble;
}

Variant::Variant(Type m_type, bool m_asBoolean)
{
	this->m_type = m_type;
	this->m_asLongDouble = 0;
	this->m_asBoolean = m_asBoolean;
}