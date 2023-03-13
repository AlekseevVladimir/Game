#pragma once
#include <memory>
#include <vector>
#include <utility>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <map>
#include <engine/engine_math/vector3.h>
#include <engine/core/component.h>


class GameObject {
public:
	struct RenderSettings {
		bool isVisible = true;
		bool isHighlighted = false;
		std::string shader = "";
	};

	GameObject(unsigned int id, std::string alias, RenderSettings renderSettings) :
		m_id(id)
		, m_alias(alias + std::to_string(id)) 
		, m_renderSettings(renderSettings)
	{};

	virtual ~GameObject() {};

	void tick(float delta) {
		for (auto& component : m_components) {
			//component->tick(delta);
		}
	};

	const RenderSettings getRenderSettings() {
		return m_renderSettings;
	}

	const std::string getAlias() {
		return m_alias;
	}

	template<typename T>
	T* getComponent() {
		for (auto& componentPtr : m_components) {
			T* ptr = dynamic_cast<T*>(componentPtr.get());
			if (ptr) {
				return ptr;
			}
		}
		return nullptr;
	}

	template<typename ComponentT, typename... Args>
	ComponentT* createComponent(Args&&... args) {
		m_components.push_back(std::make_unique<ComponentT>(getAlias(), std::forward<Args>(args)...));
		return dynamic_cast<ComponentT*>(m_components.back().get());
	}

protected:
	std::string m_alias;
	unsigned int m_id = 0;

private:
	std::vector<std::unique_ptr<Component>> m_components;
	RenderSettings m_renderSettings;
};

class GameObjectHolder {
public:
	static GameObjectHolder& getInstance() {
		static GameObjectHolder inst;
		return inst;
	}

	GameObject* getObject(std::string alias) const {
		return m_gameObjects.at(alias).get();
	}

	const std::map<std::string, std::unique_ptr<GameObject>>& getObjects() const {
		return m_gameObjects;
	}

	template<typename... ComponentsT>
	std::vector<GameObject*> getObjectsWithComponents() {
		std::vector<GameObject*> res;
		for (auto& go : m_gameObjects) {
			if ((...&& go.second->getComponent<ComponentsT>())) {
				res.push_back(go.second.get());
			}
		}
		return res;
	}

	template<typename ComponentT>
	std::vector<GameObject*> getObjectsWithComponent() {
		std::vector<GameObject*> res;
		for (auto& go : m_gameObjects) {
			if (go.second->getComponent<ComponentT>()) {
				res.push_back(go.second.get());
			}
		}
		return res;
	}

	std::vector<GameObject*> getObjectsOfType(const std::string& alias) {
		auto found = m_gameObjectsByType.find(alias);
		if (found != m_gameObjectsByType.end()) {
			return found->second;
		}
		return {};
	}


	GameObject* createGO(std::string alias, GameObject::RenderSettings renderSettings) {
		std::unique_ptr<GameObject> ptr = std::make_unique<GameObject>(m_currentID++, alias, renderSettings);
		GameObject* rawPtr = ptr.get();
		if (!renderSettings.shader.empty()) {
			m_objectsByShader[renderSettings.shader].push_back(rawPtr);
		}
		m_gameObjectsByType[alias].push_back(rawPtr);
		m_gameObjects[ptr->getAlias()] = std::move(ptr);
		return rawPtr;
	}


	const std::map<std::string, std::vector<GameObject*>> getObjectsByShader() {
		return m_objectsByShader;
	}

private:

	std::map<std::string, std::unique_ptr<GameObject>> m_gameObjects;
	std::map<std::string, std::vector<GameObject*>> m_gameObjectsByType;
	std::map<std::string, std::vector<GameObject*>> m_objectsByShader;
	unsigned int m_currentID = 1;
};