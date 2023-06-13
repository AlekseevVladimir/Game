#pragma once

#include <engine/core/component.h>
#include <memory>
#include "model.h"

class ModelComponent : public Component 
{
public:
	ModelComponent(std::string goID) : Component(goID) {};
	std::shared_ptr<Model> model;
};
