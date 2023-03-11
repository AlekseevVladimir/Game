#pragma once
#include <core/component.h>

class MaterialComponent : public Component {
public:
	struct Material {
		unsigned int diffuse;
		unsigned int specular;
		float shininess;
	};

	MaterialComponent(std::string goID, Material materialData = {}) : Component(goID), m_materialData(materialData) {}

	Material getMaterialData() const {
		return m_materialData;
	}

	void setMaterialData(Material data) {
		m_materialData = data;
	}

private:
	Material m_materialData;
};

