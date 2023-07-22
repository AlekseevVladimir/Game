#include "engine/render/core/render_system.h"
#include "engine/core/game_object.h"


namespace OpenGL
{
	class Renderer : public IRenderer
	{
	public:
		Renderer();
		virtual void render(GameObject* goPtr, GameObject* viewPointPtr, Shader* shaderPtr) override;
		virtual void _setCullingType(unsigned int) override;
		virtual ~Renderer() = default;
	private:
		unsigned int _cullingType = 0;
	};
}