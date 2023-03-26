#pragma once
#include <engine/render/utils/render_utils.h>

#include <engine/core/system.h>
#include <engine/core/game_object.h>
#include <engine/render/core/shader_component.h>
#include <engine/render/open_gl/model/model_component.h>

class RenderSystem : public System {
	void process(float delta) override {
		for (GameObject* goPtr : GameObjectHolder::getInstance().getObjectsWithComponent<ShaderComponent>()) {
			std::shared_ptr<Shader> shaderPtr = goPtr->getComponent<ShaderComponent>()->m_shaderPtr;
			shaderPtr->use();
			goPtr->getComponent<ModelComponent>()->model->setModelData(shaderPtr);
		}
	}
};