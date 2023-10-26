#pragma once
#include <engine/render/utils/render_utils.h>
#include <engine/core/system.h>
#include <engine/core/game_object.h>
#include "engine/render/open_gl/model/model.h"
#include "engine/render/core/framebuffers.h"

class Shader;
class OpenGLShader;

namespace
{
	constexpr unsigned int COLOR_BUFFERS_NUM = 2;
	constexpr unsigned int G_BUFFER_ITEMS_NUM = 3;
}

enum ColorBuffers : uint8_t
{
	HDR = 0,
	Brightness = 1,
};

enum GBufferItems : uint8_t
{
	Positions = 0,
	Normals = 1,
	AlbedoSpecular = 2,
};

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

	std::shared_ptr<OpenGLShader> _GShader;
	std::unique_ptr<Mesh> _GMesh;

	std::shared_ptr<OpenGLShader> _HDRShader;
	std::unique_ptr<Mesh> _HDRMesh;

	std::shared_ptr<OpenGLShader> _bloomShader;
	std::unique_ptr<Mesh> _bloomMesh;

	std::shared_ptr<OpenGLShader> _EquirectToCubeShader;
	std::unique_ptr<Mesh> _EquirectToCubeMesh;

	std::shared_ptr<OpenGLShader> _BackgroundShader;
	std::unique_ptr<Mesh> _BackgroundMesh;


	unsigned int envCubemap;

	unsigned int _colorBuffers[COLOR_BUFFERS_NUM];

	//one for horizontal blurring, one for vertical
//	unsigned int _bloomGenerationBuffers[2];
//	unsigned int _bloomGenerationTextures[2];

	GFramebuffer _gBuffer;

	PostprocessFramebuffer _postprocessBuffer;

	std::vector<BlurFramebuffer> m_blurBuffers = { {false}, {true} };



	//unsigned int _gBuffer;
	unsigned int _gBufferItems[G_BUFFER_ITEMS_NUM];
	std::shared_ptr<OpenGLShader> _gBufferShader;

};
