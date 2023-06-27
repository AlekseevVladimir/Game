#include "GameCtrl.h"
#include "engine/render/open_gl/renderer.h"
#include <engine/utils/game_object_factory.h>
#include <engine/core/systems_holder.h>
#include <engine/player_controls/input_control_system.h>
#include <engine/render/core/render_system.h>
#include "engine/transform/Movement_system.h"
#include "engine/transform/rotation_system.h"
#include "engine/render/core/shadows/shadow_map_generation_system.h"

GameCtrl::GameCtrl() 
{
	createDirectionalLight({ false, false, "" });
	//createCube({ -3.0f, -2.0f, -3.0f }, { 1.0f, 1.0f, 1.0f }, { true, false, "solidObject" });
	//createCube({ -3.0f, -9.0f, -3.0f }, { 1.0f, 1.0f, 1.0f }, { true, false, "solidObject" });
	//createCube({ -3.0f, 9.0f, -3.0f }, { 1.0f, 1.0f, 1.0f }, { true, false, "solidObject" });
	//TODO cube casts no shadow from dir light
	createCube({ 1.0f, 1.0f, 3.0f }, { 1.0f, 1.0f, 1.0f }, { true, false, "solidObject" });
	createCamera({ 0.0f, 0.0f, 0.0f }, 0.0f, 0.0f, 0.0f, { false, false, "" });

	//createPointLight({ 2.0f, 1.0f, 5.0f }, { 1.0f, 1.0f , 1.0f }, { true, false, "light" });

	createFloor({0.0f, -1.0f, 0.0f}, {true, false, "solidObject"});
	//createTroll({true, false, "solidObject"});
	//createFloor({ 0.0f, -1.0f, 0.0f }, { true, false, "solidObject" });
	createTroll({ 0.0f, 0.0f, 0.0f }, { true, false, "solidObject" });
	SystemsHolder::getInstance().addSystem<InputControlSystem>();
	SystemsHolder::getInstance().addSystem<MovementSystem>();
	SystemsHolder::getInstance().addSystem<RotationSystem>();
	SystemsHolder::getInstance().addSystem<ShadowMapGenerationSystem>();
	RenderSystem* renderSystemPtr = SystemsHolder::getInstance().addSystem<RenderSystem>();
	renderSystemPtr->setRenderer(std::make_unique<OpenGL::Renderer>());


	//createSpotLight({ false, false, "" });
	//createBackPack({ 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f }, { true, false, "solidObject" });
}

