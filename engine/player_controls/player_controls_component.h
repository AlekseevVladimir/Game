#pragma once
#include <engine/core/component.h>

class PlayerControlsComponent : public Component {
public:
	PlayerControlsComponent(std::string goID) : Component(goID) {};
};