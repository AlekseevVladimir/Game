#include "engine/core/component.h"


class OmnidirShadowMapComponent : public Component
{
public:
	OmnidirShadowMapComponent(std::string goID);

	unsigned int m_shadowMapID;
	unsigned int _shadowMapWidth = 1024;
	unsigned int _shadowMapHeight = 1024;
};