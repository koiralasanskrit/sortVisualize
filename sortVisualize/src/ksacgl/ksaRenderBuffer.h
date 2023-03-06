#pragma once
#include <GL/glew.h>

typedef struct
{
	unsigned int rendererId;
} ksaRBuffer;

void ksaRBufferInit(ksaRBuffer* fbuffer, unsigned int _storagex, unsigned int _storagey);
void ksaRBufferBind(ksaRBuffer *fbuffer);
void ksaRBufferUnBind(ksaRBuffer *fbuffer);
void ksaRBufferDestroy(ksaRBuffer *fbuffer);
