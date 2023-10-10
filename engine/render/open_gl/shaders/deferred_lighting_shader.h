#pragma once

#include "open_gl_shader.h"

class GameObject;

class DeferredLightingShader : public OpenGLShader
{
public:
	DeferredLightingShader(std::string alias) : OpenGLShader(alias) {};

	void configure() override;

	void setMatrices(GameObject* viewPointPtr) override;

	~DeferredLightingShader() = default;

protected:
	void setTextureData(std::vector<Mesh::Texture>& textures) override;
};
