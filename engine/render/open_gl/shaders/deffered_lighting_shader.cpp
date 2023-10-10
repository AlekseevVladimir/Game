#include "deffered_lighting_shader.h"
#include "engine/transform/position_component.h"
#include "engine/transform/rotation_component.h"
#include "engine/transform/scale_component.h"
#include "engine/render/core/textures_ctrl.h"
#include "engine/render/core/light/light_direction_component.h"
#include "engine/render/core/light/light_emitter_component.h"
#include "engine/render/core/shadows/directional_shadow_map_component.h"
#include "engine/render/core/shadows/omnidir_shadow_map_component.h"
#include "engine/render/utils/render_utils.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glad/glad.h"


void SolidObjectShader::configure() {
	TexturesCtrl& texCtrl = TexturesCtrl::getInstance();

	auto ptrsVector = GameObjectHolder::getInstance().getObjectsOfType("pointLight");
	setInt1("numPointLights", ptrsVector.size());
	for (int i = 0; i < ptrsVector.size(); ++i) {
		const auto& goPtr = ptrsVector[i];
		//assert(i < MAX_POINT_LIGHTS);
		LightEmitterComponent* lightCompPtr = goPtr->getComponent<LightEmitterComponent>();
		std::string baseStr = "pointLights[" + std::to_string(i);
		setFloat3((baseStr + std::string("].ambient")).c_str(), 
			lightCompPtr->getLightData().ambient.data());
		setFloat3((baseStr + std::string("].diffuse")).c_str(), 
			lightCompPtr->getLightData().diffuse.data());
		setFloat3((baseStr + std::string("].specular")).c_str(), 
			lightCompPtr->getLightData().specular.data());

		setFloat3((baseStr + std::string("].position")).c_str(), 
			&goPtr->getComponent<PositionComponent>()->getPos()[0]);

		setFloat1((baseStr + std::string("].constant")).c_str(), 1.f);
		setFloat1((baseStr + std::string("].linear")).c_str(), 0.09f); 
		setFloat1((baseStr + std::string("].quadratic")).c_str(), 0.032f);

		OmnidirShadowMapComponent* shadowMap = goPtr->getComponent<OmnidirShadowMapComponent>();
		setFloat1("farPlane", shadowMap->_farPlane);
		setInt1("omnidirShadowMap", 
			texCtrl.bindTexture(shadowMap->_shadowMapID, GL_TEXTURE_CUBE_MAP));
		glActiveTexture(GL_TEXTURE0);
	}


	for (auto goPtr : GameObjectHolder::getInstance().getObjectsOfType("directionalLight")) {
		LightEmitterComponent* lightCompPtr = goPtr->getComponent<LightEmitterComponent>();
		setFloat3("directionalLight.ambient", lightCompPtr->getLightData().ambient.data());
		setFloat3("directionalLight.diffuse", lightCompPtr->getLightData().diffuse.data());
		setFloat3("directionalLight.specular", lightCompPtr->getLightData().specular.data());
		LightDirectionComponent* lightDirectionComponentPtr =
			goPtr->getComponent<LightDirectionComponent>();
		setFloat3("directionalLight.direction", &lightDirectionComponentPtr->getDirection()[0]);
		DirectionalShadowMapComponent* shadowMapCmp = goPtr->getComponent<DirectionalShadowMapComponent>();

		glm::mat lightView = glm::lookAt(
			lightDirectionComponentPtr->getFrom().getGlm(),
			lightDirectionComponentPtr->getTo().getGlm(),
			glm::vec3(0.f, 1.f, 0.f)
		);
		glm::mat4 lightSpaceMatrix = lightDirectionComponentPtr->_projectionMatrix * lightView;
		setMatrix4Float("lightSpaceMatrix", GL_FALSE, glm::value_ptr(lightSpaceMatrix));

		//glActiveTexture(GL_TEXTURE0 + 12);
		//glBindTexture(GL_TEXTURE_2D, shadowMapCmp->_shadowMapID);
//		TexturesCtrl::getInstance().bindTexture(shadowMapCmp->_shadowMapID);
		setInt1("shadowMap", TexturesCtrl::getInstance().bindTexture(
			shadowMapCmp->_shadowMapID, GL_TEXTURE_2D));
	//	glActiveTexture(GL_TEXTURE0);
	}
}

void SolidObjectShader::setMatrices(GameObject* viewPointPtr)
{
	setFloat3("viewPos", &viewPointPtr->getComponent<PositionComponent>()->getPos()[0]);
	setMatrix4Float("view", GL_FALSE, glm::value_ptr(getView(viewPointPtr)));
	setMatrix4Float("projection", GL_FALSE, glm::value_ptr(getProjection()));
}

void SolidObjectShader::setTextureData(std::vector<Mesh::Texture>& textures) 
{
	TexturesCtrl& texCtrl = TexturesCtrl::getInstance();
	bool hasNormalMap = false;
	bool hasHeightMap = false;
	for (const Mesh::Texture& texture : textures)
	{
		std::string name = texture.type;
		unsigned idx = texCtrl.bindTexture(texture.id, GL_TEXTURE_2D);
		setInt1(("material." + name).c_str(), idx);
		if (name == "texture_normal")
		{
			hasNormalMap = true;
		}
		if (name == "texture_height")
		{
			hasHeightMap = true;
		}
	}
	// TODO find better solution to determine if normal map or avg normal should be used in shader
	setInt1("texture_normal_set", hasNormalMap ? 1 : 0);
	setInt1("texture_height_set", hasHeightMap ? 1 : 0);
	setFloat1("material.shininess", 32.0f);
	glActiveTexture(GL_TEXTURE0);
}

