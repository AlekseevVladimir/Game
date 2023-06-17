#pragma once

#include <string>
#include <engine/engine_math/vector3.h>
#include <engine/core/component.h>

class PositionComponent : public Component 
{
public:
	PositionComponent(std::string goID,
		Vector3<float> pos = Vector3<float>(0.0f, 0.0f, 0.0f))
		: Component(goID)
		, m_pos(std::move(pos)) 
	{}

	void setPos(Vector3<float> newPos) 
	{
		m_pos = std::move(newPos);
	}

	Vector3<float> getPos() const 
	{
		return m_pos;
	}

	Vector3<float> getWorldPos() const;
private:
	Vector3<float> m_pos;
};
