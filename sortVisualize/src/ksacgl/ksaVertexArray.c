#include "ksaVertexArray.h"

unsigned int ksaVBufferElementGetSize(unsigned int type)
{
    switch (type)
    {
    case GL_FLOAT:
        return 4;
    case GL_UNSIGNED_INT:
        return 4;
    case GL_UNSIGNED_BYTE:
        return 4;
    }
    return 0;
}

void ksaVBufferLayoutPush(ksaVBufferLayout *layout, unsigned int count)
{
    layout->elements[layout->index++] = (ksaVBufferElement){GL_FLOAT, count, GL_FALSE};
    layout->stride += count * sizeof(GL_FLOAT);
}

void ksaVArrayInit(ksaVArray *varray)
{
	glGenVertexArrays(1, &varray->rendererID);
	glBindVertexArray(varray->rendererID);
}

void ksaVArrayAddBuffer(ksaVArray *varray, ksaVBuffer *vbuffer, ksaVBufferLayout *layout)
{
    ksaVArrayBind(varray);
    ksaVBufferBind(vbuffer);
    unsigned int _offset = 0;
    for (unsigned int _i = 0; _i < layout->index; _i++)
    {
        glEnableVertexAttribArray(_i);
        glVertexAttribPointer(_i,
                              layout->elements[_i].count,
                              layout->elements[_i].type,
                              layout->elements[_i].normalized,
                              layout->stride,
                              (void *)_offset);
        _offset += layout->elements[_i].count * sizeof(GL_FLOAT);
    }
}

void ksaVArrayBind(ksaVArray *varray)
{
    glBindVertexArray(varray->rendererID);
}

void ksaVArrayBufferUnBind(ksaVArray *varray)
{
    glBindVertexArray(0);
}
