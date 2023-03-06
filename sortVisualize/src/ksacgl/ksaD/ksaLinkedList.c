#include <stdlib.h>
#include <string.h>

 struct ksaNode
{
	void* data;
	struct ksaNode* next;
	size_t elemSize;
};

typedef struct ksaNode ksaNode;

ksaNode* ksaNodeInit(void* _data, size_t _size)
{
	ksaNode* node = (ksaNode*)malloc(sizeof(node));
	node->next = NULL;
	memcpy((char*)node->data, _data, node->elemSize);
	return node;
}

void ksaNodeInsertEnd(ksaNode* _first, void* _data)
{

	ksaNode* node = (ksaNode*)malloc(sizeof(node));
	node->next = NULL;
	memcpy((char*)node->data, _data, node->elemSize);

	size_t size = _first->elemSize;
	ksaNode* temp = _first;
	while (temp->next != NULL)
	{
		temp = temp->next;
	}
	temp->next = node;
}

ksaNode* ksaNodeInsertStart(ksaNode* _first, void* _data)
{
	ksaNode* node = (ksaNode*)malloc(sizeof(node));
	memcpy((char*)node->data, _data, node->elemSize);
	node->next = _first;
	return node;
}
