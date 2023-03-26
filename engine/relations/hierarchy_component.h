#pragma once

#include <memory>
#include <engine/core/component.h>
#include <unordered_set>
#include <string>

class GameObject;

class HierarchyComponent : public Component {
public:
	HierarchyComponent(std::string goID) : Component(goID) {}

	void setParent(GameObject* go);

	void setParent(std::string goID);

	void addChild(GameObject* go);

	void addChild(std::string goID);

	std::string getParentID() {
		return m_parentID;
	}

	std::unordered_set<std::string> getChildren() {
		return m_children;
	}

private:
	std::string m_parentID;
	std::unordered_set<std::string> m_children;
};