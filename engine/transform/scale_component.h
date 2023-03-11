#pragma once

#include <string>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <Math/Vector3.h>
#include <core/component.h>

class TransformComponent : public Component {
public:
	TransformComponent(std::string goID, Vector3<float> scale = Vector3<float>(1.0f, 1.0f, 1.0f))
		: Component(goID)
		, m_scale(std::move(scale)) {}

	void setScale(Vector3<float> newScale) {
		m_scale = std::move(newScale);
	}

	Vector3<float> getScale() const {
		return m_scale;
	}

private:
	Vector3<float> m_scale;
};
