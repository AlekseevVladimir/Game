#include "game_object_factory.h"

#include <engine/render/core/light/light_direction_component.h>
#include <engine/render/core/light/light_emitter_component.h>
#include <engine/render/core/light/spot_light_component.h>

#include "engine/player_controls/player_controls_component.h"
#include <engine/relations/hierarchy_component.h>
#include <engine/transform/movement_request_component.h>
#include <engine/transform/position_component.h>
#include <engine/transform/rotation_component.h>
#include "engine/transform/scale_component.h"
#include <engine/render/core/model/material_component.h>
#include <engine/render/core/textures_ctrl.h>
#include <engine/render/core/shadows/shadow_map_component.h>
#include <engine/render/core/shadows/omnidir_shadow_map_shader_component.h>
#include <engine/render/open_gl/model/model_loader.h>
#include <engine/render/open_gl/model/model.h>
#include <engine/render/core/constants.h>
#include <engine/render/open_gl/model/model_component.h>
#include <engine/render/open_gl/shaders/open_gl_shader.h>
#include "engine/render/open_gl/shaders/open_gl_solid_object_shader.h"
#include "engine/render/open_gl/shaders/open_gl_shadow_map_shader.h"
#include <engine/render/core/shader_component.h>
#include <engine/render/core/shadows/shadow_map_shader_component.h>
#include "engine/render/core/shadows/omnidir_shadow_map_component.h"
#include <engine/render/core/view_point_component.h>

//controls are inverted

std::shared_ptr<Model> generateModel(std::string, std::string);

GameObject* createCube(
		glm::vec3 pos, glm::vec3 scale, GameObject::RenderSettings renderSettings) {
	GameObject* goPtr = GameObjectHolder::getInstance().createGO("cube", renderSettings);
	PositionComponent* position = goPtr->createComponent<PositionComponent>(Vector3<float>(pos));

	RotationComponent* rotation = goPtr->createComponent<RotationComponent>(10.0f, 10.0f, 10.0f);
	
/*
	TexturesCtrl& texturesCtrl = TexturesCtrl::getInstance();
	texturesCtrl.setWrapParams(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);
	unsigned int texture = texturesCtrl.loadImage("box.png");
	texturesCtrl.setWrapParams(GL_REPEAT, GL_REPEAT);
	unsigned int specularMap = texturesCtrl.loadImage("box_specular_map.png");
	std::vector<Mesh::Vertex> vertices = cubeVertices;
	std::vector<Mesh::Texture> textures{ {texture, "texture_diffuse", ""}, {specularMap, "texture_specular", ""} };
	Mesh mesh{ vertices, textures };
	ModelComponent* modelComponent = goPtr->createComponent<ModelComponent>();
	modelComponent->model = std::make_shared<Model>();
	modelComponent->model->m_meshes = {mesh};
	modelComponent->model->setupModel();
	*/
	ModelComponent* modelComponent = goPtr->createComponent<ModelComponent>();
	modelComponent->model = 
		generateModel("box.png", "box_specular_map.png");
	std::shared_ptr<Shader> shader = ShadersManager::getInstance().
		createProgram<SolidObjectShader>("solidObject");
	goPtr->createComponent<ShaderComponent>(shader);
	std::shared_ptr<Shader> shadowMapShader = ShadersManager::getInstance().
		createProgram<ShadowMapShader>("shadow_map");
	goPtr->createComponent<ShadowMapShaderComponent>(shadowMapShader);
	//goPtr->createComponent<TransformComponent>(pos, scale);
	//goPtr->createComponent<Generated3DVisualsComponent>("cube", "cube", "box.png", "box_specular_map.png");
	return goPtr;
}

GameObject* createBackPack(
		glm::vec3 pos, glm::vec3 scale, GameObject::RenderSettings renderSettings) {
	GameObject* goPtr = GameObjectHolder::getInstance().createGO("backpack", renderSettings);
	//goPtr->createComponent<TransformComponent>(pos, scale);
	//goPtr->createComponent<ModelComponent>("models/backpack/backpack.obj");
	return goPtr;
}

GameObject* createPointLight(
		glm::vec3 pos, glm::vec3 scale, GameObject::RenderSettings renderSettings) {
	GameObject* goPtr = GameObjectHolder::getInstance().createGO("pointLight", renderSettings);
	RotationComponent* rotation = goPtr->createComponent<RotationComponent>(10.0f, 10.0f, 10.0f);
	goPtr->createComponent<PositionComponent>(pos);
	//goPtr->createComponent<Generated3DVisualsComponent>("light", "cube");
	goPtr->createComponent<LightEmitterComponent>();
	ModelComponent* modelComponent = goPtr->createComponent<ModelComponent>();
	modelComponent->model = 
		generateModel("box.png", "box_specular_map.png");
	std::shared_ptr<Shader> shader = ShadersManager::getInstance().
		createProgram<SolidObjectShader>("solidObject");
	goPtr->createComponent<ShaderComponent>(shader);
	std::shared_ptr<Shader> shadowMapShader = ShadersManager::getInstance().
		createProgram<ShadowMapShader>("shadow_map");
	//goPtr->createComponent<OmnidirShadowMapShaderComponent>(shadowMapShader);
	//goPtr->createComponent<OmnidirShadowMapComponent>();

	return goPtr;
}

GameObject* createDirectionalLight(GameObject::RenderSettings renderSettings) {
	GameObject* goPtr = GameObjectHolder::getInstance().createGO("directionalLight", renderSettings);
	goPtr->createComponent<LightEmitterComponent>();
	goPtr->createComponent<LightDirectionComponent>();
	goPtr->createComponent<ShadowMapComponent>();
	return goPtr;
}

GameObject* createSpotLight(GameObject::RenderSettings renderSettings) {
	GameObject* goPtr = GameObjectHolder::getInstance().createGO("spotLight", renderSettings);
	//goPtr->createComponent<SpotLightComponent>();
	return goPtr;
}

GameObject* createCamera(
		glm::vec3 pos, float yaw, float pitch, float roll, GameObject::RenderSettings renderSettings) {
	GameObject* goPtr = GameObjectHolder::getInstance().createGO("camera", renderSettings);
	PositionComponent* position = goPtr->createComponent<PositionComponent>();
	RotationComponent* rotation = goPtr->createComponent<RotationComponent>();
	//goPtr->createComponent<DirectionalMoverComponent>(transform);
	goPtr->createComponent<ViewPointComponent>();
	goPtr->createComponent<PlayerControlsComponent>();
	return goPtr;
}

GameObject* createFloor(glm::vec3 pos, GameObject::RenderSettings renderSettings) {
	GameObject* goPtr = GameObjectHolder::getInstance().createGO("floor", renderSettings);
	PositionComponent* posComponent = goPtr->createComponent<PositionComponent>();
	posComponent->setPos(pos);
	RotationComponent* rotComponent = goPtr->createComponent<RotationComponent>();
	rotComponent->setYaw(90.0f);
	ModelComponent* modelComponent = goPtr->createComponent<ModelComponent>();
	modelComponent->model = generateModel("floor.png", "box_specular_map.png");
	std::shared_ptr<Shader> shader = ShadersManager::getInstance().
		createProgram<SolidObjectShader>("solidObject");;
	goPtr->createComponent<ShaderComponent>(shader);
	std::shared_ptr<Shader> shadowMapShader = ShadersManager::getInstance().
		createProgram<ShadowMapShader>("shadow_map");
	goPtr->createComponent<ShadowMapShaderComponent>(shadowMapShader);
	goPtr->createComponent<ScaleComponent>(Vector3<float>(100.f, 1.f, 100.f));
	//transform->setScale({10.0f, 1.0f, 10.0f});
	//goPtr->createComponent<Generated3DVisualsComponent>("plate", "plate", "floor.png", "box_specular_map.png");
	return goPtr;

}

GameObject* createTroll(glm::vec3 pos, GameObject::RenderSettings renderSettings) {
	GameObject* goPtr = GameObjectHolder::getInstance().createGO("troll", renderSettings);
	ModelComponent* modelCompPtr = goPtr->createComponent<ModelComponent>();
	std::shared_ptr<Model> modelPtr = ModelLoader::getInstance().getModel("test");
	modelCompPtr->model = modelPtr;
	//modelPtr->setupModel();
	std::shared_ptr<Shader> shader = ShadersManager::getInstance().
		createProgram<SolidObjectShader>("solidObject");;
	std::shared_ptr<Shader> shadowMapShader = ShadersManager::getInstance().
		createProgram<ShadowMapShader>("shadow_map");
	goPtr->createComponent<ShadowMapShaderComponent>(shadowMapShader);
	goPtr->createComponent<ShaderComponent>(shader);
	goPtr->createComponent<PositionComponent>(pos);
	goPtr->createComponent<RotationComponent>();
	//TransformComponent* transform = goPtr->createComponent<TransformComponent>();
	//transform->setPos(pos);
	//goPtr->createComponent<ModelComponent>("models/troll/troll.obj");
	return goPtr;

}

std::shared_ptr<Model> generateModel(
		std::string diffuseName, std::string specularName) {
	TexturesCtrl& texturesCtrl = TexturesCtrl::getInstance();
	texturesCtrl.setWrapParams(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);
	unsigned int texture = texturesCtrl.loadImage(diffuseName);
	texturesCtrl.setWrapParams(GL_REPEAT, GL_REPEAT);
	unsigned int specularMap = texturesCtrl.loadImage(specularName);
	std::vector<Mesh::Vertex> vertices = cubeVertices;
	std::vector<Mesh::Texture> textures{ {texture, "texture_diffuse", ""}, {specularMap, "texture_specular", ""} };
	Mesh mesh{ vertices, textures };
	std::shared_ptr<Model> model = std::make_shared<Model>();
	// TODO FIX THIS
	model->m_meshes = {mesh};
	//concreteModel->setupModel();
	return model;
}