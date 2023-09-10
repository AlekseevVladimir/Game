#include "framebuffers.h"
#include "glad/glad.h"
#include "engine/render/core/constants.h"
#include "engine/render/core/textures_ctrl.h"

TextureParams::TextureParams()
	: _internalFormat(GL_RGBA16F)
	, _width(RES_WIDTH)
	, _height(RES_HEIGHT)
	, _format(GL_RGBA)
	, _wrapS(GL_REPEAT)
	, _wrapT(GL_REPEAT)
	, _minFilter(GL_LINEAR)
	, _magFilter(GL_LINEAR)
{}


Framebuffer::Framebuffer()
{
	glGenFramebuffers(1, &_id);

}

unsigned int Framebuffer::_createTexture(const TextureParams& textureParams)
{
	unsigned int textureId;
	glGenTextures(1, &textureId);
	TexturesCtrl::getInstance().bindTexture(textureId);
	glTexImage2D(GL_TEXTURE_2D, 0, textureParams._internalFormat, textureParams._width,
		textureParams._height, 0, textureParams._format, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, textureParams._wrapS);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, textureParams._wrapT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, textureParams._minFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, textureParams._magFilter);
	return textureId;
}

void GFramebuffer::_setupFramebuffer()
{
	TextureParams texParams;
	glBindFramebuffer(GL_FRAMEBUFFER, _id);
	_positions = _createTexture(texParams);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _positions, 0);

	_normals = _createTexture(texParams);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, _normals, 0);
	texParams._internalFormat = GL_RGBA;
	_albedoSpecular = _createTexture(texParams);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, _albedoSpecular, 0);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void PostprocessFramebuffer::_setupFramebuffer()
{
	TextureParams texParams;
	glBindFramebuffer(GL_FRAMEBUFFER, _id);
	_hdr = _createTexture(texParams);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _hdr, 0);
	_brightness = _createTexture(texParams);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, _brightness, 0);

	unsigned int tmpDepthBuffer;
	glGenRenderbuffers(1, &tmpDepthBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, tmpDepthBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, RES_WIDTH, RES_HEIGHT);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, tmpDepthBuffer);

	unsigned int attachmentTypes[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
	glDrawBuffers(2, attachmentTypes);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
