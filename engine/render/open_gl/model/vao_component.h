#pragma once

#include <core/component.h>

class VAOComponent : public Component {
public:
	VAOComponent(std::string goID, unsigned int VAOID = 0) : Component(goID), m_VAOID(VAOID) {}

	unsigned int getVAOID() const {
		return m_VAOID;
	}

	void setVAOID(unsigned int VAOID) {
		m_VAOID = VAOID;
	}

private:
	unsigned int m_VAOID;
};
