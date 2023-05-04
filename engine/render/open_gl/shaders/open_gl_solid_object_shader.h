#include "open_gl_shader.h"

class SolidObjectShader : public OpenGLShader
{
	SolidObjectShader(std::string alias) : OpenGLShader(alias)
	{};

	void setModelDataAndDraw() override;

	void configure() override;
};