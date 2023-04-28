#include "rotation_system.h"
#include <iostream>
#include "engine/core/game_object.h"
#include "engine/transform/rotation_request_component.h"
#include "engine/transform/rotation_component.h"

void RotationSystem::process(float delta) {
	for (auto goPtr : GameObjectHolder::getInstance().getObjectsWithComponent<RotationRequestComponent>()) {
		RotationComponent* rotationPtr = goPtr->getComponent<RotationComponent>();
		float yaw = rotationPtr->getYaw();
		float pitch = rotationPtr->getPitch();
		for (auto componentPtr : goPtr->getComponentsByClass<RotationRequestComponent>()) {
			RotationRequestComponent* rotationRequestPtr = dynamic_cast<RotationRequestComponent*>
				(componentPtr);
			yaw += rotationRequestPtr->m_yaw;
			pitch += rotationRequestPtr->m_pitch;
		}
		rotationPtr->setYaw(yaw);
		rotationPtr->setPitch(pitch);
		goPtr->removeComponents<RotationRequestComponent>();
	}
}