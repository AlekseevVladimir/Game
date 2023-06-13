#pragma once
#include <string>
#include <typeinfo>

class Component 
{
public:
	Component(std::string goID) : m_goID(goID) {};
	std::string m_goID{ "" };

	std::string getComponentName() 
	{
		return typeid(*this).name();
	}

	virtual ~Component() = default;
};

//TODO use this
class UniqueComponent : public Component 
{
public:
	UniqueComponent(std::string goID) : Component(goID) {}

	virtual ~UniqueComponent() = default;
};
