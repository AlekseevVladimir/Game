#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <engine/core/game_object.h>
#include <engine/utils/game_object_factory.h>
#include <engine/engine_math/Vector3.h>
#include <engine/core/systems_holder.h>

class GameCtrl {
public:
	GameCtrl() {
		createDirectionalLight({ false, false, "" });
		createCube({ -3.0f, 0.0f, -3.0f }, { 1.0f, 1.0f, 1.0f }, {true, false, "solidObject"});
		createCamera({ 0.0f, 0.0f, 0.0f }, 0.0f, 0.0f, 0.0f, {false, false, ""});
		createPointLight({ 3.0f, 3.0f, 3.0f }, { 1.0f, 1.0f , 1.0f }, {true, false, "light"});
		createFloor({ 0.0f, -1.0f, 0.0f }, { true, false, "solidObject" });
		createTroll({ 0.0f, 0.0f, 0.0f }, { true, false, "solidObject" });

		//createSpotLight({ false, false, "" });
		//createBackPack({ 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f }, { true, false, "solidObject" });
		/*
		std::vector<std::vector<int>> levelData = readLevelData();
		float brickWidth = RES_WIDTH / levelData[0].size();
		auto& objectCtrl = GameObjectHolder::getInstance();
		for (int i = 0; i < levelData.size(); i++) {
			for (int j = 0; j < levelData[0].size(); j++) {
				if (levelData[i][j] == 1) {
					objectCtrl.createBrick(glm::vec3(j * brickWidth, i * 100.0f, 0.0f), { brickWidth, 100.0f, 1.0f }, { true, false });
				}
			}
		}
		objectCtrl.createPaddle(glm::vec3(200.0f, 550.0f, 1.0f), { 200.0f, 50.0f, 1.0f }, { true, false });
		objectCtrl.createBall(glm::vec3(100.0f, -50.0f, 1.0f), { 50.0f, 50.0f, 1.0f }, { true, false });
		*/
	}

	void tick(float delta) {
		m_systemsHolder.process(delta);
		/*meObjectHolder::getInstance();
		for (auto& object : objectCtrl.getObjects()) {
			object.second->tick(delta);
		}
		*/
	}
private:
	SystemsHolder m_systemsHolder;
	std::vector<std::vector<int>> readLevelData() {
		std::fstream file;
		file.open("level_data/level1.txt", std::ios::in);
		std::string buf;
		std::stringstream stream;
		std::vector<std::vector<int>> res;
		std::vector<int> tmp;
		int tmpInt;
		while (std::getline(file, buf)) {
			stream << buf;
			while (stream >> tmpInt) {
				tmp.push_back(tmpInt);
			}
			stream.clear();
			res.push_back(std::move(tmp));
			tmp = {};
		}
		return res;
	}
};