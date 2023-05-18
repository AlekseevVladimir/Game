#pragma once
#include <memory>
#include <vector>

#include "system.h"

class SystemsHolder {
public:

	static SystemsHolder& getInstance() {
		static SystemsHolder inst;
		return inst;
	}

	void process(float delta);

	template<typename SystemT>
	SystemT* addSystem() {
		m_systems.push_back(std::make_unique<SystemT>());
		return dynamic_cast<SystemT*>(m_systems.back().get());
	}

	template<typename SystemT>
	SystemT* getSystem() {
		for (auto& system : m_systems) {
			SystemT* ptr = dynamic_cast<SystemT*>(system.get());
			if (ptr) {
				return ptr;
			}
		}
		return nullptr;

	}

private:
	std::vector<std::unique_ptr<System>> m_systems;
};