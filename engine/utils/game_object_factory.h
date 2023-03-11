#pragma once
#include <GameObject.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

GameObject* createCube(
	glm::vec3 pos, glm::vec3 scale = glm::vec3(1.0f), GameObject::RenderSettings renderSettings = {});

GameObject* createBackPack(
	glm::vec3 pos, glm::vec3 scale = glm::vec3(1.0f), GameObject::RenderSettings renderSettings = {});

GameObject* createPointLight(
	glm::vec3 pos, glm::vec3 scale = glm::vec3(1.0f), GameObject::RenderSettings renderSettings = {});

GameObject* createDirectionalLight(GameObject::RenderSettings renderSettings = {});

GameObject* createSpotLight(GameObject::RenderSettings renderSettings = {});

GameObject* createCamera(
	glm::vec3 pos, float yaw = 0, float pitch = 0, float roll = 0, GameObject::RenderSettings renderSettings = {});

GameObject* createFloor(glm::vec3 pos, GameObject::RenderSettings renderSettings = {});

GameObject* createTroll(glm::vec3 pos, GameObject::RenderSettings renderSettings = {});
