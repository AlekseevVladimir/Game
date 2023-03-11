#pragma once
#define GLFW_INCLUDE_NONE
#include <iostream>
#include <glad/glad.h>
#include <glfw3.h>
#include <set>
#include <memory>
#include <utility>
#include <vector>
#include <mutex>
#include <map>
#include <Constants.h>

class VerticesCtrl {
public:
	static VerticesCtrl& getInstance() {
		static VerticesCtrl inst;
		return inst;
	}

	unsigned int getVAO(std::string_view alias, const std::string_view typeName) {
		const auto it = m_VAOs.find(alias);
		if (it != m_VAOs.end()) {
			return it->second;
		}
		return createVAO(std::string(alias), std::string(typeName));

	}

	void draw(const std::string& typeName) const {
		glDrawArrays(drawConfig.at(typeName).first, 0, drawConfig.at(typeName).second);
	}

private:
	unsigned int createVAO(const std::string& alias, const std::string& typeName) {
		glGenVertexArrays(1, &m_VAOs[alias]);
		const auto it = m_VBOs.find(typeName);
		if (it == m_VBOs.end()) {
			glGenBuffers(1, &m_VBOs[typeName]);
		}

		glBindVertexArray(m_VAOs[alias]);
		glBindBuffer(GL_ARRAY_BUFFER, m_VBOs[typeName]);

		glBufferData(GL_ARRAY_BUFFER, verticesConfig.at(typeName).size() * sizeof(std::remove_reference<decltype(verticesConfig.at(typeName))>::type::value_type), verticesConfig.at(typeName).data(), GL_STATIC_DRAW);

		for (int i = 0, offset = 0; i < verticesLayout.at(alias).first.size(); offset += verticesLayout.at(alias).first[i], ++i) {
			glVertexAttribPointer(i, verticesLayout.at(alias).first[i], GL_FLOAT, GL_FALSE, verticesLayout.at(alias).second * sizeof(float), (void*)(offset * sizeof(float)));
			glEnableVertexAttribArray(i);
		}
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
		return m_VAOs[alias];
	}

	VerticesCtrl() {};
	VerticesCtrl(VerticesCtrl& other) = delete;
	void operator=(const VerticesCtrl& other) = delete;
	std::map<std::string, unsigned int, std::less<>> m_VBOs;
	std::map<std::string, unsigned int, std::less<>> m_VAOs;
};