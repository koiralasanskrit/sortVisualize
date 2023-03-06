#pragma once
struct sortbase_pair
{
    int first;
    int second;
};
typedef struct sortbase_pair sortbase_pair;

struct sortbase_frame
{
    int* array;
    int array_size;

    int** array_of_arrays;
    int frame_index;
    sortbase_pair* array_of_selection;
    int frame_array_size;
};
typedef struct sortbase_frame sortbase_frame;

/* New datas for sortbase */
extern unsigned int sortbase_no_of_items;
extern unsigned int sortbase_no_of_frames;


/* Generate Frames and stuffs */
extern sortbase_frame sortbase_frame_stuff;

void sortbase_randomize();

void sortbase_destroy();

void sortbase_swap(int* a, int* b);

void sortbase_print_everything();

void sortbase_bubblesort(int* _array, int _size);
