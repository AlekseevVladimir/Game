#pragma once
#include <engine/core/component.h>
#include <string>

class RotationRequestComponent : public Component {
public:
	RotationRequestComponent(std::string goID, float yaw, float pitch, float roll) 
		: Component(goID), m_yaw(yaw), m_pitch(pitch), m_roll(roll) 
	{}

	float m_yaw;
	float m_pitch;
	float m_roll;
};