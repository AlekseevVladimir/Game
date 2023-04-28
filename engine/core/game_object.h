#pragma once
#include <iterator>
#include <algorithm>
#include <typeindex>
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

	template<typename TComponent>
	TComponent* getComponent() {
		return dynamic_cast<TComponent*>(getComponentByTypeId(typeid(TComponent)));
	}

	Component* getComponentByTypeId(std::type_index targetType) {
		auto found = m_components.find(targetType);
		if (found != m_components.end()) {
			return found->second.front().get();
		}
		return nullptr;
	}
		
	std::vector<Component*> getComponentsByTypeId(std::type_index targetType) {
		auto found = m_components.find(targetType);
		std::vector<Component*> res;
		if (found != m_components.end()) {
			std::transform(found->second.begin(), found->second.end(),
				std::back_inserter(res),
				[](std::unique_ptr<Component>& inPtr) { return inPtr.get(); });
		}
		return res;
	}


	template<typename TComponent>
	std::vector<Component*> getComponentsByClass() {
		return getComponentsByTypeId(typeid(TComponent));
	}

	template<typename TComponent>
	void removeComponents() {
		m_components.erase(typeid(TComponent));
	}

	template<typename TComponent, typename... Args>
	TComponent* createComponent(Args&&... args) {
		m_components[typeid(TComponent)].
			push_back(std::make_unique<TComponent>(getAlias(), std::forward<Args>(args)...));
//		std::cout << "num components " << m_components.size() << std::endl;
		TComponent* componentPtr = dynamic_cast<TComponent*>(
			m_components[typeid(TComponent)].back().get());
		if (dynamic_cast<UniqueComponent*>(componentPtr) && m_components[typeid(TComponent)].size() != 1) {
			throw std::logic_error("Attempt to create multiple instances of a unique component");
		}
		return componentPtr;
	}

protected:
	std::string m_alias;
	unsigned int m_id = 0;

private:
	std::map<std::type_index, std::vector<std::unique_ptr<Component>>> m_components;
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

	template<typename TComponent>
	std::vector<GameObject*> getObjectsWithComponent() {
		std::vector<GameObject*> res;
		for (auto& go : m_gameObjects) {
			if (go.second->getComponent<TComponent>()) {
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