#pragma once
#include <engine/render/utils/render_utils.h>

#include "engine/render/open_gl/model/model.h"
#include <engine/core/system.h>
#include <engine/core/game_object.h>
#include <engine/render/core/shader_component.h>
#include <engine/render/open_gl/model/model_component.h>

class RenderSystem : public System {
public:
	void process(float delta) override;

	template<typename TShaderComponent>
	void render(GameObject* viewPointPtr);
	
};