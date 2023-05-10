#include "glad/glad.h"
#include "open_gl_shader.h"
#include "engine/render/core/textures_ctrl.h"
//#include "engine/render/core/constants.h"
#include "engine/render/core/light/light_emitter_component.h"
#include "engine/render/core/light/light_direction_component.h"
#include "engine/render/core/light/spot_light_component.h"
#include "engine/transform/position_component.h"

#include "engine/transform/transform_utils.h"
#include <engine/render/core/shaders_manager.h>
#include "engine/render/open_gl/model/model.h"
#include "engine/render/utils/render_utils.h"



OpenGLShader::OpenGLShader(std::string alias) {
	unsigned int programID = glCreateProgram();
	auto vertexShaderCreationRes = createShader(alias + ".vs", GL_VERTEX_SHADER, programID);
	if (!vertexShaderCreationRes.first) {
		//return;
	}
	auto fragmentShaderCreationRes = createShader(alias + ".fs", GL_FRAGMENT_SHADER, programID);
	if (!fragmentShaderCreationRes.first) {
		//return;
	}

	glLinkProgram(programID);
	int success;
	char infoLog[512];
	glGetProgramiv(programID, GL_LINK_STATUS, &success);

	if (!success) {
		glGetProgramInfoLog(programID, 512, NULL, infoLog);
		std::cerr << "Shader program linking failed \n" << infoLog << std::endl;
		//return;

	}
	glDeleteShader(vertexShaderCreationRes.second);
	glDeleteShader(fragmentShaderCreationRes.second);
	m_programID = programID;
}


	void OpenGLShader::use() {
		glUseProgram(m_programID);
	}

	void OpenGLShader::setInt1(const char* name, int value) {
		glUniform1i(glGetUniformLocation(m_programID, name), value);
	}

	void OpenGLShader::setFloat1(const char* name, float value) {
		glUniform1f(glGetUniformLocation(m_programID, name), value);
	}

	void OpenGLShader::setFloat3(const char* name, const float* values) {
		glUniform3fv(glGetUniformLocation(m_programID, name), 1, values);
	}

	void OpenGLShader::setFloat3(const char* name, float val1, float val2, float val3) {
		glUniform3f(
			glGetUniformLocation(m_programID, name),
			val1, val2, val3);
	}

	void OpenGLShader::setMatrix4Float(const char* name, int transpose, const float* values) {
		glUniformMatrix4fv(
			glGetUniformLocation(m_programID, name),
			1, transpose, values);
	}

void OpenGLShader::setModelDataAndDraw(Model* model, GameObject* goPtr,
	GameObject* viewPointPtr)
{
//	GameObject* cube = GameObjectHolder::getInstance().getObject("cube2");
	
	setMatrix4Float(
		"model", GL_FALSE,
		glm::value_ptr(TransformUtils::getTransform(goPtr->getComponent<PositionComponent>(),
			goPtr->getComponent<RotationComponent>(), 
			goPtr->getComponent<ScaleComponent>())));
			

	int cnt = 0;
	for (std::variant<Mesh, ElementsMesh>& mesh : model->m_meshes) 
	{
		auto texturesGetter = [](MeshConcept auto& inMesh) -> std::vector<Mesh::Texture> {return inMesh.getTextures(); };
		//texturesGetter(std::get<1>(mesh));
		std::vector<Mesh::Texture> textures = std::visit(texturesGetter, mesh);
		setTextureData(textures);
		glBindVertexArray(mesh.VAO);
		//glDrawElements(GL_TRIANGLES, mesh.getIndices().size(), GL_UNSIGNED_INT, 0);
	//.	glDrawArrays(GL_TRIANGLES, 0, mesh.m_vertices.size());
		mesh.drawMesh();
		//glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
	}
}

