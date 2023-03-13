#include <core/component.h>

#include <vector>

class LightEmitterComponent : public Component {
public:
	struct Light {
		std::vector<float> ambient;
		std::vector<float> diffuse;
		std::vector<float> specular;
	};

	LightEmitterComponent(std::string goID) : Component(goID) {
		m_light = { {0.1f, 0.1f, 0.1f}, {1.0f, 1.0f, 1.0f}, {1.0, 1.0f, 1.0f} };
	}

	const Light getLightData() {
		return m_light;
	}

	~LightEmitterComponent() {};

private:
	Light m_light;
};