#include <iostream> 
#include "movement_system.h" 
#include "position_component.h"
#include "movement_request_component.h"
#include "engine/core/game_object.h"


void MovementSystem::process(float delta) {
	for (GameObject* goPtr :
			GameObjectHolder::getInstance().getObjectsWithComponent<MovementRequestComponent>()) {
		PositionComponent* positionComponent = goPtr->getComponent<PositionComponent>();
		Vector3<float> posDiff;
		for (Component* request : goPtr->getComponentsByClass<MovementRequestComponent>()) {
			posDiff += dynamic_cast<MovementRequestComponent*>(request)->m_movementVector;
		}
			
			//= goPtr->getComponent<MovementRequestComponent>()->m_movementVector;
		Vector3<float> position = positionComponent->getPos();
		//std::cout << "Pos diff" << "x " << posDiff.x << " y " << posDiff.y << " z " << posDiff.z <<
		//	std::endl;
		position += posDiff * 0.001f;
		positionComponent->setPos(position);
		goPtr->removeComponents<MovementRequestComponent>();
	}
}