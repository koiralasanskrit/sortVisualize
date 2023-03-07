#include <stdlib.h>
#include <string.h>
#include "sortbase.h"

unsigned int sortbase_no_of_items = 10;
unsigned int sortbase_no_of_frames = 100;

sortbase_frame sortbase_frame_stuff;

void
sortbase_randomize() 
{
    sortbase_no_of_frames = 100 * sortbase_no_of_items;
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

void
sortbase_destroy() 
{
    free(sortbase_frame_stuff.array);
    for (int i = 0; i < sortbase_no_of_frames; ++i)
        free(sortbase_frame_stuff.array_of_arrays[i]);
    free(sortbase_frame_stuff.array_of_arrays);
}

void
sortbase_swap(int* a, int* b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

void
sortbase_print_everything()
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

void
sortbase_push_to_frame(int* _array, int _size, int i, int j)
{
	memcpy(sortbase_frame_stuff.array_of_arrays[sortbase_frame_stuff.frame_index], _array, _size * sizeof(int));
	sortbase_frame_stuff.frame_index++;
	sortbase_frame_stuff.array_of_selection[sortbase_frame_stuff.frame_index] = (sortbase_pair){ .first = i, .second = j };
}

void
sortbase_bubblesort(int*_array, int _size) 
{
    for (size_t i = 0; i < _size; i++) {
        for (size_t j = 0; j < i + 1; j++) {
            if (_array[j] > _array[i]) {
                sortbase_swap(&_array[i], &_array[j]);
                sortbase_push_to_frame(_array, _size, i, j);
            }
        }
    }
}

void
sortbase_selectionsort(int *_array, int _size)
{
    int i, j, minimum_index;
  
    for (i = 0; i < _size-1; i++)
    {
        minimum_index = i;
        for (j = i + 1; j < _size; j++) {
            if (_array[j] < _array[minimum_index]) {
                minimum_index = j;
                sortbase_push_to_frame(_array, _size, i, j);
            }
        }
  
        if (minimum_index != i) {
            sortbase_swap(&_array[minimum_index], &_array[i]);
            sortbase_push_to_frame(_array, _size, i, j);
        }
    }
} 

void
sortbase_insertionsort(int *_array, int _size)
{
    int i, key, j;
    for (i = 1; i < _size; i++) {
        key = _array[i];
        j = i - 1;
 
        while (j >= 0 && _array[j] > key) {
            _array[j + 1] = _array[j];
            j = j - 1;
            sortbase_push_to_frame(_array, _size, i, j);
        }
        _array[j + 1] = key;
    }
}

void
sortbase_merge(int *_array, int l, int m, int r)
{
    int i, j, k;
    int n1 = m - l + 1;
    int n2 = r - m;
  
    int* L = (int*)malloc(sizeof(int) * n1);
    int* R = (int*)malloc(sizeof(int) * n2);
  
    for (i = 0; i < n1; i++)
        L[i] = _array[l + i];
    for (j = 0; j < n2; j++)
        R[j] = _array[m + 1 + j];
  
    i = 0; 
    j = 0;
    k = l;
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            _array[k] = L[i];
            i++;
        }
        else {
            _array[k] = R[j];
            j++;
        }
        k++;
    }
  
    while (i < n1) {
        _array[k] = L[i];
        i++;
        k++;
    }
  
    while (j < n2) {
        _array[k] = R[j];
        j++;
        k++;
    }

    free(L);
    free(R);
}
  
void
sortbase_mergesort(int *_array, int l, int r, int _size)
{
    if (l < r) {
        int m = l + (r - l) / 2;
        sortbase_mergesort(_array, l, m, _size);
        sortbase_push_to_frame(_array, _size, l, m);
        sortbase_mergesort(_array, m + 1, r, _size);
        sortbase_merge(_array, l, m, r);
        sortbase_push_to_frame(_array, _size, m, r);
    }
}

int
sortbase_partition(int *_array, int low, int high)
{
    int pivot = _array[high]; // pivot
    int i = (low - 1);
               
    for (int j = low; j <= high - 1; j++) {
        if (_array[j] < pivot) {
            i++; 
            sortbase_swap(_array[i], _array[j]);
        }
    }
    sortbase_swap(_array[i + 1], _array[high]);
    return (i + 1);
}
 
void
sortbase_quicksort(int *_array, int low, int high, int _size)
{
    printf("low:%d high:%d size:%d", low, high, _size);
    if (low < high) {
        int pi = sortbase_partition(_array, low, high);
        sortbase_quicksort(_array, low, pi - 1, _size);
        sortbase_push_to_frame(_array, _size, low, pi - 1);
        sortbase_quicksort(_array, pi + 1, high, _size);
        sortbase_push_to_frame(_array, _size, pi + 1, high);
    }
}

void
sortbase_heapify(int *_array, int N, int i, int _size)
{
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;
    if (left < N && _array[left] > _array[largest])
        largest = left;
    if (right < N && _array[right] > _array[largest])
        largest = right;
    if (largest != i) {
        sortbase_swap(&_array[i], &_array[largest]);
        sortbase_heapify(_array, N, largest, _size);
        sortbase_push_to_frame(_array, _size, i, largest);
    }
}

void
sortbase_heapsort(int *_array, int _size)
{
    for (int i = _size / 2 - 1; i >= 0; i--) 
    {
        sortbase_heapify(_array, _size, i, _size);
    }
    for (int i = _size - 1; i >= 0; i--) {
        sortbase_swap(&_array[0], &_array[i]);
        sortbase_heapify(_array, i, 0, _size);
        sortbase_push_to_frame(_array, _size, i, 0);
    }
}
  
int
sortbase_getmax(int *_array, int n)
{
    int mx = _array[0];
    for (int i = 1; i < n; i++)
        if (_array[i] > mx)
            mx = _array[i];
    return mx;
}
 
void
sortbase_countsort(int *_array, int n, int exp)
{
    int* output = (int*)malloc(n * sizeof(int));
    int i, count[10] = { 0 };
 
    for (i = 0; i < n; i++) {
        count[(_array[i] / exp) % 10]++;
        sortbase_push_to_frame(_array, n, i, 0);
    }
 
    for (i = 1; i < 10; i++) {
        count[i] += count[i - 1];
        sortbase_push_to_frame(_array, n, 0, i);
    }
 
    for (i = n - 1; i >= 0; i--) {
        output[count[(_array[i] / exp) % 10] - 1] = _array[i];
        count[(_array[i] / exp) % 10]--;
        sortbase_push_to_frame(_array, n, i, 0);
    }
 
    for (i = 0; i < n; i++) {
        _array[i] = output[i];
        sortbase_push_to_frame(_array, n, 0, i);
    }

    free(output);
}
 
void
sortbase_radixsort(int *_array, int n)
{
    int m = sortbase_getmax(_array, n);
 
    for (int exp = 1; m / exp > 0; exp *= 10) {
        sortbase_countsort(_array, n, exp);
    }
}
