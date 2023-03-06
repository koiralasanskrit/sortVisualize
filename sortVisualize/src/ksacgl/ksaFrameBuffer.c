#include "ksaFrameBuffer.h"
#pragma once

void ksaFBufferInit(ksaFBuffer* fbuffer, const void* data, unsigned int size, unsigned int type)
{
	glGenFramebuffers(1, &fbuffer->rendererId);
	glBindFramebuffer(GL_FRAMEBUFFER, fbuffer->rendererId);
//	glBufferData(GL_FRAMEBUFFER, size, data, type);
}

void ksaFBufferBind(ksaFBuffer* fbuffer)
{
	glBindFramebuffer(GL_FRAMEBUFFER, fbuffer->rendererId);
}

void ksaFBufferUnBind(ksaFBuffer* fbuffer)
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void ksaFBufferDestroy(ksaFBuffer* fbuffer)
{
	glDeleteBuffers(1, &fbuffer->rendererId);
}
