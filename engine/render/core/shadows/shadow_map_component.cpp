#include "shadow_map_component.h"
namespace {
	int SHADOW_WIDTH = 1024;
	int SHADOW_HEIGHT = 1024;
}


ShadowMapComponent::ShadowMapComponent(std::string goID) : Component(goID)
	{
		glGenTextures(1, &m_shadowMapID);
		glActiveTexture(GL_TEXTURE0 + 12);
		glBindTexture(GL_TEXTURE_2D, m_shadowMapID);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT,
			0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		float borderColor[] = { 1.0f, 1.0f, 1.0f };
		glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
		glActiveTexture(GL_TEXTURE0);


	}