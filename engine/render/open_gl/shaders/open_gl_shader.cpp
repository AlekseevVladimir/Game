#include "glad/glad.h"
#include "open_gl_shader.h"
#include "engine/render/core/textures_ctrl.h"
//#include "engine/render/core/constants.h"
#include "engine/render/core/light/light_emitter_component.h"
#include "engine/render/core/light/light_direction_component.h"
#include "engine/render/core/light/spot_light_component.h"
#include "engine/transform/position_component.h"

void OpenGLShader::configure() {
	TexturesCtrl& texCtrl = TexturesCtrl::getInstance();

	auto ptrsVector = GameObjectHolder::getInstance().getObjectsOfType("pointLight");
	setInt1("numPointLights", ptrsVector.size());
	for (int i = 0; i < ptrsVector.size(); ++i) {
		const auto& goPtr = ptrsVector[i];
		//assert(i < MAX_POINT_LIGHTS);
		LightEmitterComponent* lightCompPtr = goPtr->getComponent<LightEmitterComponent>();
		std::string baseStr = "pointLights[" + std::to_string(i);
		setFloat3((baseStr + std::string("].ambient")).c_str(), lightCompPtr->getLightData().ambient.data());
		setFloat3((baseStr + std::string("].diffuse")).c_str(), lightCompPtr->getLightData().diffuse.data());
		setFloat3((baseStr + std::string("].specular")).c_str(), lightCompPtr->getLightData().specular.data());

		setFloat3((baseStr + std::string("].position")).c_str(), &goPtr->getComponent<PositionComponent>()->getPos()[0]);

		setFloat1((baseStr + std::string("].constant")).c_str(), 1.0f);
		setFloat1((baseStr + std::string("].linear")).c_str(), 0.09f);
		setFloat1((baseStr + std::string("].quadratic")).c_str(), 0.032f);
	}


	for (auto goPtr : GameObjectHolder::getInstance().getObjectsOfType("directionalLight")) {
		LightEmitterComponent* lightCompPtr = goPtr->getComponent<LightEmitterComponent>();
		setFloat3("directionalLight.ambient", lightCompPtr->getLightData().ambient.data());
		setFloat3("directionalLight.diffuse", lightCompPtr->getLightData().diffuse.data());
		setFloat3("directionalLight.specular", lightCompPtr->getLightData().specular.data());
		LightDirectionComponent* lightDirectionComponentPtr =
			goPtr->getComponent<LightDirectionComponent>();
		setFloat3("directionalLight.direction", &lightDirectionComponentPtr->getDirection()[0]);
	}

}

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


