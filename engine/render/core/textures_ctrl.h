#pragma once
#define GLFW_INCLUDE_NONE
#include "stb_image.h"
#include <iostream>
#include <glfw3.h>
#include <vector>
#include <memory>
#include <utility>
#include <set>
#include <map>

class TexturesCtrl 
{
public:
	static TexturesCtrl& getInstance() 
	{
		static TexturesCtrl inst;
		return inst;
	}
	unsigned int loadImage(
		std::string imageName, std::string directory = "../../resources/textures");
	void bindAllTextures();

	unsigned int bindTexture(unsigned int textureID);

	unsigned int getTexturePos(unsigned int texId) const 
	{
		return m_boundTextures.at(texId);
	}

	void setWrapParams(unsigned int wrapS, unsigned int wrapT) 
	{
		m_wrapParams = { wrapS, wrapT };
	}

private:
	unsigned int m_currentTexIdx = 0;
	std::vector<unsigned int> m_textureIDs;
	std::pair<unsigned int, unsigned int> m_wrapParams;
	std::map<unsigned int, unsigned int> m_boundTextures;
	std::map<std::string, unsigned int, std::less<>> m_loadedTextures;
};