#pragma once

#include "open_gl_shader.h"

class GameObject;

class SolidObjectShader : public OpenGLShader
{
public:
	SolidObjectShader(std::string alias) : OpenGLShader(alias)
	{};
/*
	void setModelDataAndDraw(ModelBase* model, 
		GameObject* goPtr, GameObject* viewPointPtr) override;
*/
	void configure() override;

	void setMatrices(GameObject* viewPointPtr) override;

	~SolidObjectShader() = default;

protected:
	void setTextureData(std::vector<Mesh::Texture>& textures) override;
};