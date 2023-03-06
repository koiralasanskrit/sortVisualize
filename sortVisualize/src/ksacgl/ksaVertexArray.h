#pragma once
#include <gl/glew.h>
#include "ksaVertexBuffer.h"

typedef struct ksaVBufferElement
{
    unsigned int type;
    unsigned int count;
    unsigned int normalized;
} ksaVBufferElement;

typedef struct ksaVBufferLayout
{
    ksaVBufferElement elements[50];
    unsigned int stride;
    int index;
} ksaVBufferLayout;

void ksaVBufferLayoutPush(ksaVBufferLayout *layout, unsigned int count);

typedef struct ksaVArray
{
    unsigned int rendererID;
} ksaVArray;

void ksaVArrayInit(ksaVArray *varray);
void ksaVArrayAddBuffer(ksaVArray *varray, ksaVBuffer *vbuffer, ksaVBufferLayout *layout);
void ksaVArrayBind(ksaVArray *varray);
void ksaVArrayBufferUnBind(ksaVArray *varray);