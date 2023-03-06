#include "ksaRenderBuffer.h"
#pragma once

void ksaRBufferInit(ksaRBuffer* fbuffer, unsigned int _storagex, unsigned int _storagey)
{
	glGenRenderbuffers(1, &fbuffer->rendererId);
	glBindRenderbuffer(GL_RENDERBUFFER, fbuffer->rendererId);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, _storagex, _storagey);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
//	glBufferData(GL_RENDERBUFFER, size, data, type);
}

void ksaRBufferBind(ksaRBuffer* fbuffer)
{
	glBindRenderbuffer(GL_RENDERBUFFER, fbuffer->rendererId);
}

void ksaRBufferUnBind(ksaRBuffer* fbuffer)
{
	glBindBuffer(GL_RENDERBUFFER, 0);
}

void ksaRBufferDestroy(ksaRBuffer* fbuffer)
{
	glDeleteBuffers(1, &fbuffer->rendererId);
}
