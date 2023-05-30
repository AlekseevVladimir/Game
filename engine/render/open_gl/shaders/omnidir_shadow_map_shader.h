#include "engine/render/open_gl/shaders/open_gl_shader.h"

class OmnidirShadowMapShader : public OpenGLShader
{
public:
	OmnidirShadowMapShader(std::string alias) : OpenGLShader(alias)
	{

	}

	virtual void setMatrices(GameObject* viewPointPtr) override
	{

	}

	virtual void configure() override
	{

	}
};