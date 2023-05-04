#include "open_gl_solid_object_shader.h"
#include "engine/transform/position_component.h"
#include "engine/render/core/textures_ctrl.h"
#include "engine/render/core/light/light_direction_component.h"
#include "engine/render/core/light/light_emitter_component.h"

void SolidObjectShader::configure() {
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

