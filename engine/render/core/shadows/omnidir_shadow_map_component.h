#include "engine/core/component.h"


class OmnidirShadowMapComponent : public Component
{
public:
	OmnidirShadowMapComponent(std::string goID);

	unsigned int m_shadowMapID;
};