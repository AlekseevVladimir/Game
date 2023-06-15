#pragma once

#include "open_gl_shader.h"
#include "engine/render/core/light/light_direction_component.h"

class ShadowMapShader : public OpenGLShader
{
public:
	ShadowMapShader(std::string alias) : OpenGLShader(alias) {};

	void setMatrices(GameObject* viewPointPtr) override;


protected:
	void setTextureData(std::vector<Mesh::Texture>& textures) {}
};
