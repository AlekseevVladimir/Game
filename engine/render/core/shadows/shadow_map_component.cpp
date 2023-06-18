#include "shadow_map_component.h"
#include "glad/glad.h"

ShadowMapComponent::ShadowMapComponent(std::string alias, unsigned int shadowMapWidth, unsigned int shadowMapHeight) 
	: Component(alias)
	, _shadowMapWidth(shadowMapWidth)
	, _shadowMapHeight(shadowMapHeight)
{
	glGenTextures(1, &_shadowMapID);
}

ShadowMapComponent::~ShadowMapComponent()
{
	glDeleteTextures(1, &_shadowMapID);
}
