#include "ksacgl/ksaD/ksaStack.h"
#include <stdio.h>
#include "sortVis/sortbase.h"
#include <intrin.h>


int JustATest()
{
	ksaStack stack;
	ksaStackInit(&stack, sizeof(float), 12);

	float x = 5;
	ksaStackSet(&stack, &x, 0);
	x = 6;
	ksaStackSet(&stack, &x, 1);
	x = 7;
	ksaStackSet(&stack, &x, 2);
	x = 0.5;
	ksaStackSet(&stack, &x, 3);
	x = 0.7;
	ksaStackSet(&stack, &x, 4);
	
	for (int i = 0; i < 5; i++)
	{
		float data;
		ksaStackGet(&stack, &data, i);
		printf("%f\n", data);
	}
	return 0;
}

int AnotherTest()
{
	sortbase_randomize();
	sortbase_bubblesort(sortbase_frame_stuff.array, sortbase_frame_stuff.array_size);
	sortbase_print_everything();
	sortbase_destroy();
}