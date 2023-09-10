#pragma once

#include <vector>
#include <glad/glad.h>

struct TextureParams
{
	TextureParams();
	unsigned int m_internalFormat;
	unsigned int m_width;
	unsigned int m_height;
	unsigned int m_format;
	unsigned int m_wrapS;
	unsigned int m_wrapT;
	unsigned int m_minFilter;
	unsigned int m_magFilter;
};

class Framebuffer
{
public:
	Framebuffer();
	virtual void m_setupFramebuffer() = 0;
	unsigned int m_createTexture(const TextureParams& textureParams);
	unsigned int m_id;

};

class GFramebuffer : public Framebuffer
{
public:
	virtual void m_setupFramebuffer() override;

	unsigned int m_positions;
	unsigned int m_normals;
	unsigned int m_albedoSpecular;
};

class PostprocessFramebuffer : public Framebuffer
{
public:
	unsigned int m_hdr;
	unsigned int m_brightness;

	virtual void m_setupFramebuffer() override;
};

class BlurFramebuffer : public Framebuffer
{
public:
	BlurFramebuffer(bool horizontal);

	virtual void m_setupFramebuffer() override;

	unsigned int m_buffer;
	bool m_horizontal;
};
