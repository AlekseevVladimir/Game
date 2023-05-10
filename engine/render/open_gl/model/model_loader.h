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
		std::shared_ptr<Model> modelPtr = std::make_shared<Model>(
			loadModel(name));
		m_models[name] = modelPtr;
		return modelPtr;
	}
/*
	std::shared_ptr<Model<Mesh>> generateModel(std::string name, std::vector<Mesh> meshes) {
		auto found = m_models.find(name);
		if (found != m_models.end() && !found->second.expired()) {
			return found->second.lock();
		}
		Model model = Model<Mesh>();
		model.m_meshes = meshes;
		std::shared_ptr<Model<Mesh>> modelPtr = std::make_shared<Model<Mesh>>(model);
		m_models[name] = modelPtr;
		return modelPtr;
	}
*/
private:
	Model loadModel(std::string name);

	std::map<std::string, std::weak_ptr<Model>> m_models;
};