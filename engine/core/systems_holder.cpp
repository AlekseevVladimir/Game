#include <memory>

#include "systems_holder.h"

void SystemsHolder::process(float delta) 
{
	for (auto& system : m_systems) 
	{
		system->process(delta);
	}
}