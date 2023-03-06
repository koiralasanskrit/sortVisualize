#include <stdlib.h>

typedef struct ksaStack
{
	size_t elemSize;
	unsigned int count;
	void* data;
} ksaStack;

void ksaStackInit(ksaStack* stack, size_t _size, unsigned int _count);
void ksaStackSet(ksaStack* stack, void* _data, int _index);
void ksaStackGet(ksaStack* stack, void* data, int _index);
void ksaStackDestroy(ksaStack* stack);