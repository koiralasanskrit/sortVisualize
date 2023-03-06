#include "ksaIndexBuffer.h"
#pragma once

void ksaIBufferInit(ksaIBuffer *ibuffer, const void *data, unsigned int size, unsigned int type)
{
	glGenBuffers(1, &ibuffer->rendererId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibuffer->rendererId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, type);
}

void ksaIBufferBind(ksaIBuffer *ibuffer)
{
	glBindBuffer(GL_ARRAY_BUFFER, ibuffer->rendererId);
}

void ksaIBufferUnBind(ksaIBuffer *ibuffer)
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void ksaIBufferDestroy(ksaIBuffer *ibuffer)
{
	glDeleteBuffers(1, &ibuffer->rendererId);
}
