#include "open_gl_shader.h"

class ShadowMapShader : public OpenGLShader
{
public:
	ShadowMapShader(std::string alias) : OpenGLShader(alias)
	{};

	void configure() override;
};