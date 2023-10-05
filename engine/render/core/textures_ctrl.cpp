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
	int width, height, nrChannels;
	const auto found = m_loadedTextures.find(imageName);
	if (found != m_loadedTextures.end()) 
	{
		return found->second;
	}
	std::string path = directory + "/" + std::string(imageName);
	//stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
	
	if (data) 
	{
//		if (imageName == "Troll_Normals.jpg")
//		{
//			const int res = width * height;
//			for (int i = 0; i < res; ++i)
//			{
//				const int n = i * nrChannels + 1;
//				data[n] = 255 - data[n];
//			}
//
//		}
		GLenum format;
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
	else 
	{
		stbi_image_free(data);
		throw(std::logic_error("Failed to load texture" + imageName));
	}
}

