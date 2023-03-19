#pragma once

#include <map>
#include <string>
#include <memory>

#include "model.h"

class ModelLoader {
public:
	static ModelLoader& getInstance() {
		static ModelLoader inst;
		return inst;
	}

	std::shared_ptr<Model> getModel(std::string name) {
		auto found = m_models.find(name);
		if (found != m_models.end() && !found->second.expired()) {
			return found->second.lock();
		}

	}

private:
	Model loadModel(std::string name);

	std::map<std::string, std::weak_ptr<Model>> m_models;
};