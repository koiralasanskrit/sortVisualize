#pragma once
#include <GL/glew.h>

typedef struct
{
	unsigned int rendererId;
} ksaVBuffer;

void ksaVBufferInit(ksaVBuffer *vbuffer, const void *data, unsigned int size, unsigned int type);
void ksaVBufferBind(ksaVBuffer *vbuffer);
void ksaVBufferUnBind(ksaVBuffer *vbuffer);
void ksaVBufferDestroy(ksaVBuffer *vbuffer);
