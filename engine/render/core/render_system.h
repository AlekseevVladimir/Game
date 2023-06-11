#pragma once
#include <engine/render/utils/render_utils.h>
#include <engine/core/system.h>
#include <engine/core/game_object.h>

class Shader;

class IRenderer
{
public:
	virtual void render(GameObject* goPtr, GameObject* viewPointPtr, Shader* shaderPtr) = 0;

	virtual ~IRenderer() = default;
};

class RenderSystem : public System {
public:
	void process(float delta) override;

	template<typename TShaderComponent>
	void render(GameObject* viewPointPtr, bool ignoreViewPoint=true);
	void setRenderer(std::unique_ptr<IRenderer>&& rendererPtr);
	
private:
	std::unique_ptr<IRenderer> m_renderer;
};