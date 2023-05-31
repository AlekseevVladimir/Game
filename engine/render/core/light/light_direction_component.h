#pragma once
#include <engine/core/component.h>
#include <engine/engine_math/vector3.h>

class LightDirectionComponent : public Component {
public:
	LightDirectionComponent(std::string goID) : Component(goID) {
		m_direction = { -0.2f, -1.0f, -0.3f };
		_to = { 0.f, 0.f, 0.f };
		_from = { -10.f, 10.f, -10.f };
	}

	Vector3<float> getDirection() {
		return (_to - _from).normalised();
		//return m_direction;
	}

	Vector3<float> getTo() {
		return _to;
		//return m_direction;
	}

	Vector3<float> getFrom() {
		return _from;
		//return m_direction;
	}

	// TODO replace with core math to get rid of opengl mentions
	glm::mat4 _projectionMatrix = glm::ortho(-30.f, 30.f, -30.f, 30.f, 1.f, 30.f); 

private:
	Vector3<float> m_direction;
	Vector3<float> _from;
	Vector3<float> _to;
};
