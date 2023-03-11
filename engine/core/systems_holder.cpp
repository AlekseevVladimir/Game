#include <memory>

#include "systems_holder.h"
#include <player_controls/input_control_system.h>

SystemsHolder::SystemsHolder() {
	m_systems.push_back(std::make_unique<InputControlSystem>());
}

void SystemsHolder::process(float delta) {
	for (auto& system : m_systems) {
		system->process(delta);
	}
}