#pragma once
#include <memory>

#include <glm/glm.hpp>
#include <engine/core/game_object.h>

glm::mat4 getProjection();

glm::mat4 getView(GameObject* goPtr);