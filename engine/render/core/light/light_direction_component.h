#pragma once
#include <core/component.h>
#include <Math/Vector3.h>

class LightDirectionComponent : public Component {
public:
	LightDirectionComponent(std::string goID) : Component(goID) {
		m_direction = { -0.2f, -1.0f, -0.3f };
	}

	Vector3<float> getDirection() {
		return m_direction;
	}

private:
	Vector3<float> m_direction;
};