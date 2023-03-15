#include "position_component.h"

#include <engine/core/game_object.h>
#include <engine/relations/hierarchy_component.h>

Vector3<float> PositionComponent::getWorldPos() const {
	GameObjectHolder& goHolder = GameObjectHolder::getInstance();
	HierarchyComponent* hierarchyCompPtr =
		goHolder.getObject(m_goID)->getComponent<HierarchyComponent>();
	Vector3<float> pos = m_pos;
	if (hierarchyCompPtr && !hierarchyCompPtr->getParentID().empty()) {
		GameObject* parent = hierarchyCompPtr ? goHolder.getObject(hierarchyCompPtr->getParentID()) : nullptr;
		if (parent) {
			PositionComponent* parentPosition = parent->getComponent<PositionComponent>();
			Vector3<float> parentPos = parentPosition->getWorldPos();
			pos.x += parentPos.x;
			pos.y += parentPos.y;
		}
	}
	return pos;
}
