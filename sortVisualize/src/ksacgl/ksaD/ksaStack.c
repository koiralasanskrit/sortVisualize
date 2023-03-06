#include "ksaStack.h"
#include <string.h>
#include <memory.h>

void ksaStackInit(ksaStack* stack, size_t _size, unsigned int _count)
{
	stack->data = (void*)malloc(_size * _count);
	stack->elemSize = _size;
	stack->count = _count;
}

void ksaStackSet(ksaStack* stack, void* _data, int _index)
{
	memcpy((char*)stack->data + stack->elemSize * _index, _data, stack->elemSize);
}

void ksaStackGet(ksaStack* stack, void* data, int _index)
{
	memcpy(data, (char*)stack->data + stack->elemSize * _index, stack->elemSize);
}

void ksaStackDestroy(ksaStack* stack)
{
	if (!stack->data)
		return;
	free(stack->data);
	stack->data = NULL;
	stack->count = NULL;
	stack->elemSize = NULL;
}
