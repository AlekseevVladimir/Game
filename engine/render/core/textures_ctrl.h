#pragma once
#define GLFW_INCLUDE_NONE
#include "stb_image.h"
#include <iostream>
#include <glad/glad.h>
#include <glfw3.h>
#include <vector>
#include <memory>
#include <utility>
#include <set>
#include <map>

class TexturesCtrl {
public:
	static TexturesCtrl& getInstance() {
		static TexturesCtrl inst;
		return inst;
	}

	unsigned int loadImage(std::string imageName, std::string directory = "F:/cmake_the_game/resources/textures") {
		int width, height, nrChannels;
		const auto found = m_loadedTextures.find(imageName);
		if (found != m_loadedTextures.end()) {
			return found->second;
		}
		std::string path = directory + "/" + std::string(imageName);
		//stbi_set_flip_vertically_on_load(true);
		unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
		if (data) {
			GLenum format;
			switch (nrChannels) {
			case 1:
				format = GL_RED;
				break;
			case 3:
				format = GL_RGB;
				break;
			case 4:
				format = GL_RGBA;
				break;
			default:
				throw(std::logic_error("Unexpected number of channels"));
				break;
			}
			m_textureIDs.emplace_back();
			glGenTextures(1, &m_textureIDs.back());
			glBindTexture(GL_TEXTURE_2D, m_textureIDs.back());

			glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			stbi_image_free(data);
			m_loadedTextures[imageName] = m_textureIDs.back();
			glBindTexture(GL_TEXTURE_2D, 0);
			return m_textureIDs.back();
		}
		else {
			stbi_image_free(data);
			throw(std::logic_error("Failed to load texture" + imageName));
			
		}
	}

	void bindAllTextures() {
		for (size_t i = 0; i < m_textureIDs.size(); i++) {
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, m_textureIDs[i]);
			m_boundTextures[m_textureIDs[i]] = i;
		}
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	}

	unsigned int bindTexture(unsigned int textureID) {
		auto found = std::find(m_textureIDs.begin(), m_textureIDs.end(), textureID);
		if (found == m_textureIDs.end()) {
			throw std::logic_error("Attemp to bind not exisiting texture");
		}
		if (m_currentTexIdx > GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS) {
			throw std::logic_error("Max number of units exceeded");
		}
		if (m_boundTextures.count(*found)) {
			return m_boundTextures[*found];
		}
		glActiveTexture(GL_TEXTURE0 + m_currentTexIdx);
		glBindTexture(GL_TEXTURE_2D, *found);
		m_boundTextures[*found] = m_currentTexIdx;
		return m_currentTexIdx++;
	}

	unsigned int getTexturePos(unsigned int texId) const {
		return m_boundTextures.at(texId);
	}

	void setWrapParams(unsigned int wrapS, unsigned int wrapT) {
		m_wrapParams = { wrapS, wrapT };
	}

private:
	unsigned int m_currentTexIdx = 0;
	std::vector<unsigned int> m_textureIDs;
	std::pair<unsigned int, unsigned int> m_wrapParams;
	std::map<unsigned int, unsigned int> m_boundTextures;
	std::map<std::string, unsigned int, std::less<>> m_loadedTextures;

};