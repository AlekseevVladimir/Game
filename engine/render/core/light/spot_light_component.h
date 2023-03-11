#pragma once
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include <core/component.h>

class SpotLightComponent : public Component {
public:
	SpotLightComponent(std::string goID) : Component(goID)
		, m_cutOff(glm::cos(glm::radians(12.5f)))
		, m_outerCutOff(glm::cos(glm::radians(17.5f)))
	{

	}

	float getCutOff() {
		return m_cutOff;
	}
	float getOuterCutOff() {
		return m_outerCutOff;
	}

private:
	float m_cutOff;
	float m_outerCutOff;
};