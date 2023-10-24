#include "engine/render/open_gl/shaders/open_gl_shader.h"

class GameObject;

class PBRShader : public OpenGLShader
{

public:
	PBRShader(std::string alias) : OpenGLShader(alias) {}

	virtual void configure() override;
	virtual void setMatrices(GameObject* viewPoint) override;
	virtual void setTextureData(std::vector<Mesh::Texture>& textures) override;
	virtual ~PBRShader() = default;
};