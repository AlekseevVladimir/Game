#pragma once
#include <memory>
#include <vector>

#include "system.h"

class SystemsHolder {
public:
	SystemsHolder();

	void process(float delta);
private:
	std::vector<std::unique_ptr<System>> m_systems;
};