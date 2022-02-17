/**
* Representation of events for engine
* @author Stephen Wang (sywang)
*/
#pragma once
#include "StringId.h"
#include "AbstractGameObject.h"

// An argument within the event
struct Variant
{
	// All the types of information the variant can be
	enum class Type
	{
		TYPE_GAMEOBJECT_PTR,
		TYPE_COMPONENT_PTR,
		TYPE_INT,
		TYPE_FLOAT,
		TYPE_VECTOR2F,
		TYPE_LONGDOUBLE,
		TYPE_BOOLEAN
	};
	Type m_type;

	// The data stored within the variant (argument within event)
	union
	{
		AbstractGameObject* m_asGameObjectPtr;
		AbstractComponent* m_asComponentPtr;
		int m_asInteger;
		float m_asFloat;
		sf::Vector2f m_asVector;
		long double m_asLongDouble;
		bool m_asBoolean;
		// Add a JSON for updateinfo?
	};
	// Constructors for the variant argument
	Variant();
	Variant(Type m_type, AbstractGameObject* m_asGameObjectPtr);
	Variant(Type m_type, AbstractComponent* m_asComponentPtr);
	Variant(Type m_type, int m_asInteger);
	Variant(Type m_type, float m_asFloat);
	Variant(Type m_type, sf::Vector2f m_asVector);
	Variant(Type m_type, long double m_asLongDouble);
	Variant(Type m_type, bool m_asBoolean);
};

// The representation of an event within the engine
class Event
{
private:
	// Stores the event type
	StringId eventType;
	// Stores the variant arguments of the event
	std::unordered_map<StringId, Variant> arguments;
public:
	// Stores the priority rank of the event
	int priority;
	// Timestamp (stores the realtime) - eventmanager will use a gametime pointer to determine
	// Event manager sets the timestamp based on when the event gets raised
	long double timestamp;

	// Constructor for the event of a particular type
	Event(StringId eventType);
	// Adds the argument given to the event (variants)
	void addGameObject(StringId argument, AbstractGameObject* gameObject);
	void addComponent(StringId argument, AbstractComponent* component);
	void addInteger(StringId argument, int integerData);
	void addFloat(StringId argument, float floatData);
	void addVector(StringId argument, sf::Vector2f vector);
	void addLongDouble(StringId argument, long double longDouble);
	void addBoolean(StringId argument, bool boolean);
	// Returns the type of the event
	StringId getType() { return eventType; }
	// Returns the variant argument of given paramter
	Variant getArgument(StringId argument) { return arguments[argument]; }

	// For Scripting specific return functions
	AbstractGameObject* getGameObj(StringId argument);
	AbstractComponent* getComponentObj(StringId argument);
	int getInteger(StringId argument);
	float getFloat(StringId argument);
	sf::Vector2f getVector(StringId argument);
	long double getLongDouble(StringId argument);
	bool getBool(StringId argument);
};