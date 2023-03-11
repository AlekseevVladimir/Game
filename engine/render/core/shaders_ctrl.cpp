#include "ShadersCtrl.h"
#include "GameObject.h"
#include <utility>
#include <memory>
#include "TexturesCtrl.h"
#include <Constants.h>
#include "VerticesArray.h"
#include "Model.h"
#include <glad/glad.h>
//#include <Components/TransformComponent.h>
#include <Render/Components/LightComponents.h>
#include <Render/Components/ModelComponent.h>
#include <Render/Components/Generated2DVisualsComponent.h>
#include <Render/Components/Generated3DVisualsComponent.h>
#include <Render/Components/ViewPointComponent.h>


void setLightData(std::shared_ptr<Shader> shader) {
	TexturesCtrl& texCtrl = TexturesCtrl::getInstance();

	auto ptrsVector = GameObjectHolder::getInstance().getObjectsOfType("pointLight");
	shader->setInt1("numPointLights", ptrsVector.size());
	for (int i = 0; i < ptrsVector.size(); ++i) {
		const auto& goPtr = ptrsVector[i];
		assert(i < MAX_POINT_LIGHTS);
		LightEmitterComponent* lightCompPtr = goPtr->getComponent<LightEmitterComponent>();
		std::string baseStr = "pointLights[" + std::to_string(i);
		shader->setFloat3((baseStr + std::string("].ambient")).c_str(), lightCompPtr->getLightData().ambient.data());
		shader->setFloat3((baseStr + std::string("].diffuse")).c_str(), lightCompPtr->getLightData().diffuse.data());
		shader->setFloat3((baseStr + std::string("].specular")).c_str(), lightCompPtr->getLightData().specular.data());

		//shader->setFloat3((baseStr + std::string("].position")).c_str(), &goPtr->getComponent<TransformComponent>()->getPos()[0]);

		shader->setFloat1((baseStr + std::string("].constant")).c_str(), 1.0f);
		shader->setFloat1((baseStr + std::string("].linear")).c_str(), 0.09f);
		shader->setFloat1((baseStr + std::string("].quadratic")).c_str(), 0.032f);
	}


	for (auto goPtr : GameObjectHolder::getInstance().getObjectsOfType("directionalLight")) {
		DirectionalLightEmitterComponent* lightCompPtr = goPtr->getComponent<DirectionalLightEmitterComponent>();
		shader->setFloat3("directionalLight.ambient", lightCompPtr->getLightData().ambient.data());
		shader->setFloat3("directionalLight.diffuse", lightCompPtr->getLightData().diffuse.data());
		shader->setFloat3("directionalLight.specular", lightCompPtr->getLightData().specular.data());

		shader->setFloat3("directionalLight.direction", &lightCompPtr->getDirection()[0]);
	}

	//shader->setInt1("useSpotLight", 0);
	for (auto goPtr : GameObjectHolder::getInstance().getObjectsOfType("spotLight")) {
		//shader->setInt1("useSpotLight", 1);
		SpotLightComponent* lightCompPtr = goPtr->getComponent<SpotLightComponent>();
		shader->setFloat3("spotLight.ambient", lightCompPtr->getLightData().ambient.data());
		shader->setFloat3("spotLight.diffuse", lightCompPtr->getLightData().diffuse.data());
		shader->setFloat3("spotLight.specular", lightCompPtr->getLightData().specular.data());
		shader->setFloat3("spotLight.direction", &(lightCompPtr->getDirection()[0]));
		shader->setFloat3("spotLight.position", &lightCompPtr->getPos()[0]);
		shader->setFloat1("spotLight.cutOff", lightCompPtr->getCutOff());
		shader->setFloat1("spotLight.outerCutOff", lightCompPtr->getOuterCutOff());

		shader->setFloat1("spotLight.constant", 1.0f);
		shader->setFloat1("spotLight.linear", 0.09f);
		shader->setFloat1("spotLight.quadratic", 0.032f);
	}
}

void setModelData(std::shared_ptr<Shader> shader, ModelComponent* modelComponent) {

	TexturesCtrl& texCtrl = TexturesCtrl::getInstance();
	int cnt = 0;
	for (auto mesh : modelComponent->model->m_meshes) {
		for (const Mesh::Texture& texture : mesh.getTextures())
		{
			// Получаем номер текстуры (значение N в diffuse_textureN)
			std::string name = texture.type;
			unsigned idx = texCtrl.bindTexture(texture.id);
			shader->setInt1(("material." + name).c_str(), idx);
		}
		shader->setFloat1("material.shininess", 32.0f);
		glActiveTexture(GL_TEXTURE0);
		glBindVertexArray(mesh.VAO);
		glDrawElements(GL_TRIANGLES, mesh.getIndices().size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}
}

void setMaterialData(std::shared_ptr<Shader> shader, Generated3DVisualsComponent* genVisuals) {
	Generated3DVisualsComponent::Material materialData = genVisuals->getMaterialData();
	TexturesCtrl& texCtrl = TexturesCtrl::getInstance();

	shader->setInt1("material.texture_diffuse", texCtrl.bindTexture(materialData.diffuse));
	shader->setInt1("material.texture_specular", texCtrl.bindTexture(materialData.specular));
	shader->setFloat1("material.shininess", 32.0f);

	//glUniform1i(glGetUniformLocation(m_programID, "material.emission"), 2);
	glBindVertexArray(genVisuals->getVAO());
	VerticesCtrl::getInstance().draw(genVisuals->getType());
	glBindVertexArray(0);

}

void setTextureData(std::shared_ptr<Shader> shader, Generated2DVisualsComponent* genVisuals) {
	unsigned int texture = genVisuals->getTexture();
	TexturesCtrl& texCtrl = TexturesCtrl::getInstance();
	shader->setInt1("image", texCtrl.bindTexture(texture));
	glm::vec3 color = genVisuals->getColor();
	shader->setFloat3("color", &color[0]);
	glBindVertexArray(genVisuals->getVAO());
	VerticesCtrl::getInstance().draw(genVisuals->getType());
}


void drawObject(std::shared_ptr<Shader> shader, GameObject* go, GameObject* viewPoint) {
	if (viewPoint) {

		//TransformComponent* transform = viewPoint->getComponent<TransformComponent>();

		//shader->setFloat3("viewPos", &transform->getPos()[0]);
		//shader->setMatrix4Float("view", GL_FALSE, glm::value_ptr(glm::lookAt(transform->getPos(), transform->getPos() + transform->getDirection().getGlm(), glm::vec3(0.0f, 1.0f, 0.0f))));
		shader->setMatrix4Float("projection", GL_FALSE, glm::value_ptr(glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f)));
	}

	Generated3DVisualsComponent* gen3DVisuals = go->getComponent<Generated3DVisualsComponent>();
	//shader->setMatrix4Float(
		//"model", GL_FALSE,
		//glm::value_ptr(go->getComponent<TransformComponent>()->getTransform()));
	if (gen3DVisuals && gen3DVisuals->hasMaterial) {
		setMaterialData(shader, gen3DVisuals);
		return;
	}
	else if (gen3DVisuals) {
		glBindVertexArray(gen3DVisuals->getVAO());
		VerticesCtrl::getInstance().draw(gen3DVisuals->getType());
		return;
	}

	Generated2DVisualsComponent* gen2DVisuals = go->getComponent<Generated2DVisualsComponent>();
	if (gen2DVisuals) {
		glm::mat4 matrix = glm::ortho(0.0f, RES_WIDTH, RES_HEIGHT, 0.0f, -1.0f, 1.0f);
		shader->setMatrix4Float("projection", GL_FALSE, glm::value_ptr(matrix));
		setTextureData(shader, gen2DVisuals);
		return;
	}

	ModelComponent* modelComponent = go->getComponent<ModelComponent>();
	if (modelComponent) {
		setModelData(shader, modelComponent);
		return;
	}
}

std::pair<bool, unsigned int> createShader(const std::string& shaderSrcFileName, int shaderType, unsigned int programID) {
	int success;
	unsigned int shaderID;
	char infoLog[512];
	const std::filesystem::path path{ "./shaders/" + shaderSrcFileName };
	std::ifstream shaderFstream(path);
	std::string shaderSrc;
	std::string buf;

	while (std::getline(shaderFstream, buf)) {
		shaderSrc += buf;
		shaderSrc.push_back('\n');
	}
	shaderFstream.close();
	shaderID = glCreateShader(shaderType);
	const char* cstr = shaderSrc.c_str();
	glShaderSource(shaderID, 1, &cstr, NULL);
	glCompileShader(shaderID);

	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);

	if (!success) {
		glGetShaderInfoLog(shaderID, 512, NULL, infoLog);
		std::cerr << "Vertex shader compilation failed\n" << infoLog << std::endl;
		return { false, 0 };
	}


	glAttachShader(programID, shaderID);
	return { true, shaderID };

}