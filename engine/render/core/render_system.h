#pragma once
#include <engine/render/utils/render_utils.h>
#include <engine/core/system.h>
#include <engine/core/game_object.h>
#include "engine/render/open_gl/model/model.h"

class Shader;
class OpenGLShader;

class IRenderer
{
public:
	virtual void render(GameObject* goPtr, GameObject* viewPointPtr, Shader* shaderPtr) = 0;
	virtual void _setCullingType(unsigned int cullingType) = 0;
	virtual ~IRenderer() = default;
};

class RenderSystem : public System 
{
public:
	RenderSystem();
	void process(float delta) override;

	template<typename TShaderComponent>
	void render(GameObject* viewPointPtr, bool ignoreViewPoint=true);

	void setRenderer(std::unique_ptr<IRenderer>&& rendererPtr);
	
	inline void _setCullingType(unsigned int cullingType)
	{
		m_renderer->_setCullingType(cullingType);
	}
private:
	std::unique_ptr<IRenderer> m_renderer;
	unsigned int _HDRTexture;
	unsigned int _HDRFBOID;
	std::shared_ptr<OpenGLShader> _HDRShader;
	std::unique_ptr<Mesh> _HDRMesh;
};
