#include <utils/game_object_factory.h>

#include "Render/Components/LightComponents.h"
#include "Render/Components/Generated3DVisualsComponent.h"
#include "Render/Components/Generated2DVisualsComponent.h"
#include "Render/Components/ModelComponent.h"
#include "Components/ColliderComponent.h"
#include "Components/PlayerControlsComponent.h"
//#include "Components/TransformComponent.h"
//#include "Components/MoverComponent.h"
#include "Components/BoundingSphereComponent.h"
#include "relations/hierarchy_component.h"
#include "Render/Components/ViewPointComponent.h"

//controls are inverted

GameObject* createCube(
		glm::vec3 pos, glm::vec3 scale, GameObject::RenderSettings renderSettings) {
	GameObject* goPtr = GameObjectHolder::getInstance().createGO("cube", renderSettings);
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