#pragma once

#include <string>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <engine/engine_math/vector3.h>
#include <engine/core/component.h>

class RotationComponent : public Component {
public:
	RotationComponent(std::string goID,
		float yaw = 0.0f, float pitch = 0.0f, float roll = 0.0f)
		: Component(goID)
		, m_yaw(yaw)
		, m_pitch(pitch)
		, m_roll(roll) {
		updateDirection();
	}

	Vector3<float> getDirection() const {
		return m_direction;
	}

	void updateDirection() {
		m_direction.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
		m_direction.y = sin(glm::radians(m_pitch));
		m_direction.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
		m_direction.normalize();
	}

	void setRotation(float yaw, float pitch, float roll) {
		m_yaw = yaw;
		m_pitch = pitch;
		m_roll = roll;
		updateDirection();
	}

	float getYaw() const {
		return m_yaw;
	}

	float getPitch() const {
		return m_pitch;
	}

	float getRoll() const {
		return m_roll;
	}

	void setYaw(float yaw) {
		m_yaw = yaw;
		updateDirection();
	}
	void setPitch(float pitch) {
		m_pitch = pitch;
		updateDirection();
	}
	void setRoll(float roll) {
		m_roll = roll;
		updateDirection();
	}


private:
	float m_yaw = 0.0f;
	float m_pitch = 0.0f;
	float m_roll = 0.0f;
	Vector3<float> m_direction;
};
