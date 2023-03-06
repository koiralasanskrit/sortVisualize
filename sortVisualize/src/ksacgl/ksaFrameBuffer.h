#pragma once
#include <GL/glew.h>

typedef struct
{
	unsigned int rendererId;
} ksaFBuffer;

void ksaFBufferInit(ksaFBuffer *fbuffer, const void *data, unsigned int size, unsigned int type);
void ksaFBufferBind(ksaFBuffer *fbuffer);
void ksaFBufferUnBind(ksaFBuffer *fbuffer);
void ksaFBufferDestroy(ksaFBuffer *fbuffer);
