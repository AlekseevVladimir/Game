#pragma once

#include "engine/core/component.h"
#include <string>
#include "engine/engine_math/vector3.h"

// check if copy constructor called in set scale ewithout std move

class ScaleComponent : public Component {
public:
	ScaleComponent(std::string goID, Vector3<float> scale = Vector3<float>(1.0f, 1.0f, 1.0f))
		: Component(goID)
		, m_scale(scale) {}

	void setScale(Vector3<float> newScale) {
		m_scale = newScale;
	}

	Vector3<float> getScale() const {
		return m_scale;
	}

private:
	Vector3<float> m_scale;
};
