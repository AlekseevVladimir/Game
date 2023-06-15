#pragma once

#include "open_gl_shader.h"

class GameObject;

class SolidObjectShader : public OpenGLShader
{
public:
	SolidObjectShader(std::string alias) : OpenGLShader(alias) {};

	void configure() override;

	void setMatrices(GameObject* viewPointPtr) override;

	~SolidObjectShader() = default;

protected:
	void setTextureData(std::vector<Mesh::Texture>& textures) override;
};
