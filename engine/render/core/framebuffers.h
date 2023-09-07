#pragma once

#include <vector>
#include <glad/glad.h>

struct TextureParams
{
	TextureParams();
	unsigned int _internalFormat;
	unsigned int _width;
	unsigned int _height;
	unsigned int _format;
	unsigned int _wrapS;
	unsigned int _wrapT;
	unsigned int _minFilter;
	unsigned int _magFilter;
};

class Framebuffer
{
public:
	Framebuffer();
	virtual void _setupFramebuffer() = 0;
	unsigned int _createTexture(const TextureParams& textureParams);
	unsigned int _id;

};

class GFramebuffer : public Framebuffer
{
public:
	virtual void _setupFramebuffer() override;

	unsigned int _positions;
	unsigned int _normals;
	unsigned int _albedoSpecular;
};

class PostprocessFramebuffer : public Framebuffer
{
public:
	unsigned int _hdr;
	unsigned int _brightness;

	virtual void _setupFramebuffer() override;
};

class BlurFramebuffer : public Framebuffer
{
public:

};
