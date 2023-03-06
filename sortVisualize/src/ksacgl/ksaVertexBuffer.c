#include "ksaVertexBuffer.h"
#pragma once

void ksaVBufferInit(ksaVBuffer* vbuffer, const void* data, unsigned int size, unsigned int type)
{
	glGenBuffers(1, &vbuffer->rendererId);
	glBindBuffer(GL_ARRAY_BUFFER, vbuffer->rendererId);
	glBufferData(GL_ARRAY_BUFFER, size, data, type);
}

void ksaVBufferBind(ksaVBuffer* vbuffer)
{
	glBindBuffer(GL_ARRAY_BUFFER, vbuffer->rendererId);
}

void ksaVBufferUnBind(ksaVBuffer* vbuffer)
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void ksaVBufferDestroy(ksaVBuffer* vbuffer)
{
	glDeleteBuffers(1, &vbuffer->rendererId);
}
