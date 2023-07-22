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
	// TODO problem with hdr textures if this is zero 
	// hapens because the texture under hdr texture's id is not re-bounded because hdr texture is not bound using the textures_ctr singleton
	// needs fix
	unsigned int m_currentTexIdx = 1;
	std::vector<unsigned int> m_textureIDs;
	std::pair<unsigned int, unsigned int> m_wrapParams;
	std::map<unsigned int, unsigned int> m_boundTextures;
	std::map<std::string, unsigned int, std::less<>> m_loadedTextures;
};
