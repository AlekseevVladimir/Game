#include "framebuffers.h"
#include "glad/glad.h"
#include "engine/render/core/constants.h"
#include "engine/render/core/textures_ctrl.h"

TextureParams::TextureParams()
	: m_internalFormat(GL_RGBA16F)
	, m_width(RES_WIDTH)
	, m_height(RES_HEIGHT)
	, m_format(GL_RGBA)
	, m_wrapS(GL_REPEAT)
	, m_wrapT(GL_REPEAT)
	, m_minFilter(GL_LINEAR)
	, m_magFilter(GL_LINEAR)
{}


Framebuffer::Framebuffer()
{
	glGenFramebuffers(1, &m_id);

}

unsigned int Framebuffer::m_createTexture(const TextureParams& textureParams)
{
	unsigned int textureId;
	glGenTextures(1, &textureId);
	TexturesCtrl::getInstance().bindTexture(textureId, GL_TEXTURE_2D);
	glTexImage2D(GL_TEXTURE_2D, 0, textureParams.m_internalFormat, textureParams.m_width,
		textureParams.m_height, 0, textureParams.m_format, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, textureParams.m_wrapS);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, textureParams.m_wrapT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, textureParams.m_minFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, textureParams.m_magFilter);
	return textureId;
}

void GFramebuffer::m_setupFramebuffer()
{
	TextureParams texParams;
	glBindFramebuffer(GL_FRAMEBUFFER, m_id);
	//m_CreateTexture breaks something CONFLICTING BINDINGS ACCORDING TO RENDERDOC
	m_positions = m_createTexture(texParams);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_positions, 0);

	m_normals = m_createTexture(texParams);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, m_normals, 0);
	texParams.m_internalFormat = GL_RGBA;
	m_albedoSpecular = m_createTexture(texParams);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, m_albedoSpecular, 0);
	unsigned int attachmentTypes[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2};
	glDrawBuffers(3, attachmentTypes);


	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void PostprocessFramebuffer::m_setupFramebuffer()
{
	TextureParams texParams;
	glBindFramebuffer(GL_FRAMEBUFFER, m_id);
	m_hdr = m_createTexture(texParams);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_hdr, 0);
	m_brightness = m_createTexture(texParams);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, m_brightness, 0);

	unsigned int tmpDepthBuffer;
	glGenRenderbuffers(1, &tmpDepthBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, tmpDepthBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, RES_WIDTH, RES_HEIGHT);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, tmpDepthBuffer);

	unsigned int attachmentTypes[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
	glDrawBuffers(2, attachmentTypes);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

BlurFramebuffer::BlurFramebuffer(bool horizontal)
	: m_horizontal(horizontal), m_buffer(0), Framebuffer()
{

}

void BlurFramebuffer::m_setupFramebuffer()
{
	TextureParams texParams;
	m_buffer = m_createTexture(texParams);
	
	glBindFramebuffer(GL_FRAMEBUFFER, m_id);
	TexturesCtrl::getInstance().bindTexture(m_buffer, GL_TEXTURE_2D);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, RES_WIDTH, RES_HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL);
	// TODO read about filters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// TODO read about wrap params again
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_buffer, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

