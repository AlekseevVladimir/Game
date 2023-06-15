#include "engine/render/core/render_system.h"
#include "engine/core/game_object.h"


namespace OpenGL
{
	class Renderer : public IRenderer
	{
	public:
		virtual void render(GameObject* goPtr, GameObject* viewPointPtr, Shader* shaderPtr) override;
		virtual ~Renderer() = default;
	};
}