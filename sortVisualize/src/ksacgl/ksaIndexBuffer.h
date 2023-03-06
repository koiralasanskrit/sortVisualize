#pragma once
#include <GL/glew.h>

typedef struct {
	unsigned int rendererId;
} ksaIBuffer;

void ksaIBufferInit(ksaIBuffer* ibuffer, const void* data, unsigned int size, unsigned int type);
void ksaIBufferBind(ksaIBuffer* ibuffer);
void ksaIBufferUnBind(ksaIBuffer* ibuffer);
void ksaIBufferDestroy(ksaIBuffer* ibuffer);
