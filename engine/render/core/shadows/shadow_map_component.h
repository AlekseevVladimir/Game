#include "engine/core/component.h"
#include "glad/glad.h"

int SHADOW_WIDTH = 1024;
int SHADOW_HEIGHT = 1024;


class ShadowMapComponent : public Component
{
public:
	ShadowMapComponent(std::string goID) : Component(goID)
	{
		glGenTextures(1, &m_shadowMapID);
		glBindTexture(GL_TEXTURE_2D, m_shadowMapID);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT,
			0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);


	}

	unsigned int m_shadowMapID;
};
