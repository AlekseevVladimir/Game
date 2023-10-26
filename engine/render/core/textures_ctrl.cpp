#include <string>
#include "textures_ctrl.h"
#include "glad/glad.h"


unsigned int TexturesCtrl::bindTexture(unsigned int textureID, unsigned int textureType) 
{
	// TODO consider making texture a class
	
	if (m_currentTexIdx > GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS) 
	{
		throw std::logic_error("Max number of units exceeded");
	}

	auto found = std::find(m_textureIDs.begin(), m_textureIDs.end(), textureID);
	if (found == m_textureIDs.end()) 
	{
		m_textureIDs.push_back(textureID);
	}

	if (m_boundTextures.count(textureID)) 
	{
		glActiveTexture(GL_TEXTURE0 + m_boundTextures[textureID]);
		glBindTexture(textureType, textureID);
		return m_boundTextures[textureID];
	}
	glActiveTexture(GL_TEXTURE0 + m_currentTexIdx);
	glBindTexture(textureType, textureID);
	m_boundTextures[textureID] = m_currentTexIdx;
	return m_currentTexIdx++;
}


void TexturesCtrl::bindAllTextures() 
{
	// DEPRECATED
	for (size_t i = 0; i < m_textureIDs.size(); i++) 
	{
		//glActiveTexture(GL_TEXTURE0 + i);
		//glBindTexture(GL_TEXTURE_2D, m_textureIDs[i]);
		//m_boundTextures[m_textureIDs[i]] = i;
		bindTexture(m_textureIDs[i], GL_TEXTURE_2D);
	}
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
}


unsigned int TexturesCtrl::loadImage(std::string imageName, std::string directory) 
{
	const auto found = m_loadedTextures.find(imageName);
	if (found != m_loadedTextures.end()) 
	{
		return found->second;
	}
	std::string path = directory + "/" + std::string(imageName);
	//stbi_set_flip_vertically_on_load(true);
	std::string extension = imageName.substr(imageName.find("."), imageName.size());

	m_textureIDs.emplace_back();
	glGenTextures(1, &m_textureIDs.back());
	glBindTexture(GL_TEXTURE_2D, m_textureIDs.back());

	if (extension == ".hdr")
	{
		loadEquirectMap(path);
	}
	else 
	{
		loadTexture(path);
	}

	glGenerateMipmap(GL_TEXTURE_2D);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, m_wrapParams.first);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, m_wrapParams.second);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	m_loadedTextures[imageName] = m_textureIDs.back();
	glBindTexture(GL_TEXTURE_2D, 0);
	return m_textureIDs.back();
	/*
	{
		stbi_image_free(data);
		throw(std::logic_error("Failed to load texture" + imageName));
	}
	*/
}

void TexturesCtrl::loadTexture(std::string path)
{
	int width, height, nrChannels;
	unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
		
	GLenum format;
	GLenum internalFormat;
	switch (nrChannels) 
	{
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
	internalFormat = format;

	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, data);
}

void TexturesCtrl::loadEquirectMap(std::string path)
{
	int width, height, nrChannels;
	float* data = stbi_loadf(path.c_str(), &width, &height, &nrChannels, 0);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, data);
	stbi_image_free(data);
}

