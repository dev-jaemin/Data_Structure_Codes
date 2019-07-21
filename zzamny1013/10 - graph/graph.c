#include <stdio.h>
#include <stdlib.h>

int** makeMatrix(FILE* fp, int* num);

void destroyMatrix(int** matrix, int n);

void dfTraverse(int** matrix, int n);

void bfTraverse(int** matrix, int n);

void adjacent(int** matrix, int n, int i, int* array);

void queueShifting(int* array, int* top);

void deleteFlag(int** matrix, int n);


int main(int argc, char** argv)
{
    FILE* fp;
    int** matrix;
    int matrix_num;
    if (argc != 2)
	{
		fprintf( stderr, "Usage: %s FILE\n", argv[0]);
		return 1;
	}
	
	fp = fopen( argv[1], "rt");
	if (fp == NULL)
	{
		fprintf( stderr, "File open error: %s\n", argv[1]);
		return 1;
	}

    matrix = makeMatrix(fp, &matrix_num);
    fclose(fp);

    dfTraverse(matrix, matrix_num);

    bfTraverse(matrix, matrix_num);


    destroyMatrix(matrix, matrix_num);

    return 0;
}

int** makeMatrix(FILE* fp, int* num)
{
    char str[15];
    int i=1;
    int n;
    int first;
    int second;

    while(i <= 3){
        fscanf(fp, "%s", str);
        if(i == 2){
            n = atoi(str);
        }
        i++;
    }

    *num = n;

    int** matrix = (int**)malloc(sizeof(int*)*(n+1));
    for(int j=0;j<=n;j++){
        matrix[j] = (int*)malloc(sizeof(int)*(n+1));
    }


    for(int i=0;i<=n;i++){
        for(int j=0;j<=n;j++){
            matrix[i][j] = 0;
        }
    }

    while(fscanf(fp, "%s", str) == 1){

        first = atoi(str);
        fscanf(fp, "%s", str);
        second = atoi(str);

        matrix[first][second] = 1;
        matrix[second][first] = 1;
        matrix[first][0] = 2;
        matrix[second][0] = 2;
    }

    return matrix;
}

void destroyMatrix(int** matrix, int n)
{
    int i = 0;
    for(i=0;i<=n;i++){
        free(matrix[i]);
        matrix[i] = NULL;
    }
    free(matrix);
    matrix = NULL;
}

void dfTraverse(int** matrix, int n)
{
    int stack[100];
    int top = -1;
    int adjacents[100];
    int curVertex = 1;
    int startingVertex = 1;

    printf("DFS : ");
    while(startingVertex != -1){
        top++;
        stack[top] = startingVertex;
        matrix[stack[top]][0] = 1;
        curVertex = stack[top];
        while(top > -1){
            printf("%d ", stack[top]);
            adjacent(matrix, n, curVertex, adjacents);
            top--;
            int i = 0;
            while(adjacents[i] != 0){
                if(matrix[adjacents[i]][0] != 1){    
                    top++;
                    stack[top] = adjacents[i];
            
                    matrix[adjacents[i]][0] = 1;
                }
                i++;
            }
            curVertex = stack[top];
        }
        for(int i=1;i<=n;i++){
            if(matrix[i][0] == 2){
                startingVertex = i;
                break;
            }
            else{
                startingVertex = -1;
            }
        }  
    } 
    printf("\n");
    deleteFlag(matrix, n);

}

void bfTraverse(int** matrix, int n)
{
    int queue[100];
    int top = -1;
    int adjacents[100];
    int curVertex = 1;
    int startingVertex = 1;

    printf("BFS : ");
    while(startingVertex != -1){
        top++;
        queue[0] = startingVertex;
        matrix[queue[0]][0] = 1;
        curVertex = queue[0];
        while(top > -1){
            printf("%d ", queue[0]);
            adjacent(matrix, n, curVertex, adjacents);
            queueShifting(queue, &top);

            int i = 0;
            while(adjacents[i] != 0){
                if(matrix[adjacents[i]][0] != 1){ 
                    top++;
                    queue[top] = adjacents[i];
                    matrix[adjacents[i]][0] = 1;
                }
                i++;
            }
            curVertex = queue[0];
        }
        for(int i=1;i<=n;i++){
            if(matrix[i][0] == 2){
                startingVertex = i;
                break;
            }
            else{
                startingVertex = -1;
            }
        }
    }
    printf("\n");
    deleteFlag(matrix, n);
    
}

void adjacent(int** matrix, int n, int i, int* array)
{
    int index = 0;
    for(int j=1;j<=n;j++){
        if(matrix[i][j] == 1){
            array[index] = j;
            index++;
        }
    }
    for(int j=index;j<100;j++){
        array[j] = 0;
    }
}

void queueShifting(int* array, int* top)
{
    for(int i=0;i<(*top);i++){
        array[i] = array[i+1];
    }
    (*top)--;
}

void deleteFlag(int** matrix, int n)
{
    for(int i=0; i<=n; i++){
        matrix[i][0] = 2;
    }
}