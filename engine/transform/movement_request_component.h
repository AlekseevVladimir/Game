#pragma once
#include <engine/core/component.h>
#include <engine/engine_math/vector3.h>

class MovementRequestComponent : public Component 
{
public:
	MovementRequestComponent(std::string goID, Vector3<float> movementVector) 
		: Component(goID), m_movementVector(movementVector) 
	{}

	Vector3<float> m_movementVector;
};