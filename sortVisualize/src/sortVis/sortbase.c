#include <stdlib.h>
#include <string.h>
#include "sortbase.h"

unsigned int sortbase_no_of_items = 10;
unsigned int sortbase_no_of_frames = 100;


sortbase_frame sortbase_frame_stuff;


void sortbase_randomize() 
{
    sortbase_no_of_frames = 10 * sortbase_no_of_items;
    srand(45);
    sortbase_frame_stuff.frame_index = 0;

    /*Allocate Memory*/
    sortbase_frame_stuff.array_size = sortbase_no_of_items;
    sortbase_frame_stuff.frame_array_size = sortbase_no_of_frames;

    sortbase_frame_stuff.array = (int*)malloc(sortbase_no_of_items * sizeof(int));
    sortbase_frame_stuff.array_of_selection = (sortbase_pair*)malloc(sortbase_no_of_frames* sizeof(sortbase_pair));
    sortbase_frame_stuff.array_of_arrays = (int**)malloc(sortbase_no_of_items * sortbase_no_of_frames * sizeof(int));

    for (int i = 0; i < sortbase_no_of_frames; ++i)
    {
        sortbase_frame_stuff.array_of_arrays[i] = (int*)malloc(sortbase_no_of_items * sizeof(int));
        memset(sortbase_frame_stuff.array_of_arrays[i], 0, sortbase_frame_stuff.array_size);
    }
    
    for (int i = 0; i < sortbase_no_of_items; ++i) 
    {
        int _random = rand() % 10 + 1;
        sortbase_frame_stuff.array[i] = _random;
    }
}


void sortbase_destroy() 
{
    free(sortbase_frame_stuff.array);
    for (int i = 0; i < sortbase_no_of_frames; ++i)
        free(sortbase_frame_stuff.array_of_arrays[i]);
    free(sortbase_frame_stuff.array_of_arrays);
    memset(&sortbase_frame_stuff, 0, sizeof(sortbase_frame));
}

void sortbase_swap(int* a, int* b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

void sortbase_print_everything()
{
    for (int i = 0; i < sortbase_frame_stuff.frame_index; i++)
    {
        for (int j = 0; j < sortbase_frame_stuff.array_size; j++)
            printf("%d\t", sortbase_frame_stuff.array_of_arrays[i][j]);
        printf("\n");
        printf("i:%d,j:%d", sortbase_frame_stuff.array_of_selection[i].first, sortbase_frame_stuff.array_of_selection[i].second);
        printf("\n");
    }
}

void sortbase_bubblesort(int*_array, int _size) 
{
    for (size_t i = 0; i < _size; i++) {
        for (size_t j = 0; j < i + 1; j++) {
            if (_array[j] > _array[i]) {
                sortbase_swap(&_array[i], &_array[j]);
                memcpy(sortbase_frame_stuff.array_of_arrays[sortbase_frame_stuff.frame_index], _array, _size * sizeof(int));
                sortbase_frame_stuff.frame_index++;
                sortbase_frame_stuff.array_of_selection[sortbase_frame_stuff.frame_index] = (sortbase_pair){ .first = i, .second = j };
            }
        }
    }
}

 