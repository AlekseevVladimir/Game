#pragma once
#include <string>

class Component {
public:
	Component(std::string goID) : m_goID(goID) {};
	std::string m_goID{ "" };

	virtual ~Component() = default;

};