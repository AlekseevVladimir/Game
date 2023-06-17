#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <engine/core/systems_holder.h>


class GameCtrl 
{
public:
	GameCtrl();

	void tick(float delta) 
	{
		SystemsHolder::getInstance().process(delta);
		/*meObjectHolder::getInstance();
		for (auto& object : objectCtrl.getObjects()) {
			object.second->tick(delta);
		}
		*/
	}
private:
	std::vector<std::vector<int>> readLevelData() 
	{
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