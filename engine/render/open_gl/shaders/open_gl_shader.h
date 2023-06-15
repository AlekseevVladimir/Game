#pragma once
#include <string>
#include <iostream>

#include <engine/render/core/shaders_manager.h>
#include "engine/render/open_gl/model/model.h"

#include "shader_utils.h"


class OpenGLShader : public Shader 
{
public:
	OpenGLShader(std::string alias);

	void setModelDataAndDraw(Model* model, GameObject* goPtr,
		GameObject* viewPointPtr);

	void use() override;
// TODO rename setMatrices to better represent method actions
	// for seteMatrices to set world data, persistent data
	// configure to set object data or set relative data, set local data
	virtual void setMatrices(GameObject* viewPointPtr) {};

	void setInt1(const char* name, int value);

	void setFloat1(const char* name, float value);
	void setFloat3(const char* name, const float* values);

	void setFloat3(const char* name, float val1, float val2, float val3);

	void setMatrix4Float(const char* name, int transpose, const float* values);

	~OpenGLShader() = default;

protected:
	virtual void setTextureData(std::vector<Mesh::Texture>& textures) {};
private:
	unsigned int m_programID = 0;
};
