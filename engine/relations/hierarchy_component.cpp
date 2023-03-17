#include <engine/core/game_object.h>
#include <engine/relations/hierarchy_component.h>

void HierarchyComponent::setParent(GameObject* go) {
	m_parentID = go->getAlias();
	HierarchyComponent* hierarchyComponentPtr = go->getComponent<HierarchyComponent>();
	if (!hierarchyComponentPtr) {
		hierarchyComponentPtr = go->createComponent<HierarchyComponent>();
	}
	if (!hierarchyComponentPtr->getChildren().count(m_goID)) {
		hierarchyComponentPtr->addChild(m_goID);
	}
}

void HierarchyComponent::setParent(std::string goID) {
	setParent(GameObjectHolder::getInstance().getObject(goID));
}

void HierarchyComponent::addChild(GameObject* go) {
	m_children.insert(go->getAlias());
	HierarchyComponent* hierarchyComponentPtr = go->getComponent<HierarchyComponent>();
	if (!hierarchyComponentPtr) {
		hierarchyComponentPtr = go->createComponent<HierarchyComponent>();
	}
	if (hierarchyComponentPtr->getParentID() != m_goID) {
		hierarchyComponentPtr->setParent(m_goID);
	}
}

void HierarchyComponent::addChild(std::string goID) {
	addChild(GameObjectHolder::getInstance().getObject(goID));
}
