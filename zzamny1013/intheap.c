#include <stdio.h>
#include <stdlib.h> // malloc, rand
#include <time.h> // time
#include <assert.h>

#define MAX_ELEM	20

typedef struct
{
	int *heapArr;
	int	last;
	int	capacity;
} HEAP;

HEAP *heapCreate( int capacity);
void heapDestroy( HEAP *heap);
int heapInsert( HEAP *heap, int data);
static void _reheapUp( HEAP *heap, int index);
int heapDelete( HEAP *heap, int* data);
static void _reheapDown( HEAP *heap, int index);
void heapPrint( HEAP *heap);

int main(void)
{
	HEAP *heap;
	int data;
	
	heap = heapCreate(MAX_ELEM);
	
	srand( time(NULL));
	
	for (int i = 1; i < MAX_ELEM; i++)
	{
		data = rand() % MAX_ELEM * 3 + 1; // 1 ~ MAX_ELEM*3 random number
				
		fprintf( stdout, "Inserting %d: ", data);
		
		// insert function call
		heapInsert( heap, data);
		
		heapPrint( heap);
 	}

	while (heap->last >= 0)
	{
		// delete function call
		heapDelete( heap, &data);

		fprintf( stdout, "Deleting %d: ", data);

		heapPrint( heap);
 	}
	
	heapDestroy( heap);
	
	return 0;
}

HEAP *heapCreate( int capacity)
{
	HEAP* newHeap = (HEAP*)malloc(sizeof(HEAP));

	newHeap->heapArr = (int*)malloc(sizeof(int)*(capacity-1));
	newHeap->last = -1;
	newHeap->capacity = capacity;

	return newHeap;
}

void heapDestroy( HEAP *heap)
{
	free(heap->heapArr);
	free(heap);
}

int heapInsert( HEAP *heap, int data)
{
	if(heap->capacity == (heap->last)+1)
		return 0;
	
	(heap->last)++;
	(heap->heapArr)[heap->last] = data;
	_reheapUp(heap, heap->last);

	return 1;
}

static void _reheapUp( HEAP *heap, int index)
{
	if(index > 0){
		int parentIndex = (index-1)/2;
		if(heap->heapArr[parentIndex] < heap->heapArr[index]){
			int tmp = heap->heapArr[index];
			heap->heapArr[index] = heap->heapArr[parentIndex];
			heap->heapArr[parentIndex] = tmp;
			_reheapUp(heap, parentIndex);
		}
	}
}

int heapDelete( HEAP *heap, int* data)
{
	*data = heap->heapArr[0];
	heap->heapArr[0] = heap->heapArr[heap->last];
	heap->heapArr[heap->last] = 0;
	(heap->last)--;
	_reheapDown(heap, 0);
	
	return 1;
}

static void _reheapDown( HEAP *heap, int index)
{
	if(2*index+2 <= 18){
		int largeindex = -10;
		if(heap->heapArr[2*index+1] >= 1 && heap->heapArr[2*index+1] <= MAX_ELEM*3){
			if(heap->heapArr[2*index+2] >= 1 && heap->heapArr[2*index+2] <= MAX_ELEM*3){
				if(heap->heapArr[2*index+1] >= heap->heapArr[2*index+2]){
					largeindex = 2*index+1;
				}
				else{
					largeindex = 2*index+2;
				}
			}
			else{
				largeindex = 2*index+1;
			}

		}
		if(largeindex != -10){
			if(heap->heapArr[index] < heap->heapArr[largeindex]){
				int tmp;
				tmp = heap->heapArr[index];
				heap->heapArr[index] = heap->heapArr[largeindex];
				heap->heapArr[largeindex] = tmp;
				_reheapDown(heap, largeindex);
			}
		}
	}
	
}

void heapPrint( HEAP *heap)
{
	for(int i = 0; i <= heap->last; i++){
		printf("    %d", heap->heapArr[i]);
	}
	printf("\n");
}