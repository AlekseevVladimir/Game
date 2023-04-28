#pragma once
#include <string>
#include <typeinfo>

#define quote(x) #x
class Component {
public:
	Component(std::string goID) : m_goID(goID) {};
	std::string m_goID{ "" };
	std::string getComponentName() {
		return typeid(*this).name();
	}
	virtual ~Component() = default;
private:

};

class UniqueComponent : public Component {
	UniqueComponent(std::string goID) : Component(goID) {}
};