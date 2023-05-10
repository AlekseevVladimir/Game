#include "engine/render/core/render_system.h"
#include "engine/core/game_object.h"


class Renderer : public IRenderer
{
public:
	virtual void render(GameObject* viewPointPtr) override;
	virtual ~Renderer() = default;
};