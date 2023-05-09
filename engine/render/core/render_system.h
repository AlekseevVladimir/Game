#pragma once
#include <engine/render/utils/render_utils.h>
#include <engine/core/system.h>
#include <engine/core/game_object.h>

class RenderSystem : public System {
public:
	void process(float delta) override;

	template<typename TShaderComponent>
	void render(GameObject* viewPointPtr);
	
};