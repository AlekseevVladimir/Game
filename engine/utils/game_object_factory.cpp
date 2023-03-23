#include "game_object_factory.h"

#include <engine/render/core/light/light_direction_component.h>
#include <engine/render/core/light/light_emitter_component.h>
#include <engine/render/core/light/spot_light_component.h>

#include "engine/player_controls/player_controls_component.h"
#include <engine/relations/hierarchy_component.h>
#include <engine/transform/movement_request_component.h>
#include <engine/transform/position_component.h>
#include <engine/transform/rotation_component.h>
#include <engine/render/core/model/material_component.h>
#include <engine/render/core/textures_ctrl.h>
#include <engine/render/open_gl/model/model_loader.h>
#include <engine/render/open_gl/model/model.h>
#include <engine/render/core/constants.h>
#include <engine/render/open_gl/model/model_component.h>

//controls are inverted



GameObject* createCube(
		glm::vec3 pos, glm::vec3 scale, GameObject::RenderSettings renderSettings) {
	GameObject* goPtr = GameObjectHolder::getInstance().createGO("cube", renderSettings);
	PositionComponent* position = goPtr->createComponent<PositionComponent>(pos);

	RotationComponent* rotation = goPtr->createComponent<RotationComponent>();
	

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
	//goPtr->createComponent<TransformComponent>(pos, scale);
	//goPtr->createComponent<Generated3DVisualsComponent>("light", "cube");
	//goPtr->createComponent<LightEmitterComponent>();
	return goPtr;
}

GameObject* createDirectionalLight(GameObject::RenderSettings renderSettings) {
	GameObject* goPtr = GameObjectHolder::getInstance().createGO("directionalLight", renderSettings);
	//goPtr->createComponent<DirectionalLightEmitterComponent>();
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
	//TransformComponent* transform = goPtr->createComponent<TransformComponent>();
	//goPtr->createComponent<DirectionalMoverComponent>(transform);
	//goPtr->createComponent<ViewPointComponent>(goPtr);
	//goPtr->createComponent<PlayerControlsComponent>(goPtr);
	return goPtr;
}

GameObject* createFloor(glm::vec3 pos, GameObject::RenderSettings renderSettings) {
	GameObject* goPtr = GameObjectHolder::getInstance().createGO("floor", renderSettings);
	//TransformComponent* transform = goPtr->createComponent<TransformComponent>();
	//transform->setPos(pos);
	//transform->setScale({10.0f, 1.0f, 10.0f});
	//goPtr->createComponent<Generated3DVisualsComponent>("plate", "plate", "floor.png", "box_specular_map.png");
	return goPtr;

}

GameObject* createTroll(glm::vec3 pos, GameObject::RenderSettings renderSettings) {
	GameObject* goPtr = GameObjectHolder::getInstance().createGO("troll", renderSettings);
	//TransformComponent* transform = goPtr->createComponent<TransformComponent>();
	//transform->setPos(pos);
	//goPtr->createComponent<ModelComponent>("models/troll/troll.obj");
	return goPtr;

}