#ifndef PMMS_HEADER
#define PMMS_HEADER

#define FALSE 0
#define TRUE !FALSE

/*Function definitions*/
int** readMatrix(char* filename, int rows, int columns);
void printMatrix(int** matrix, int rows, int columns);
void calculateC(int** matrixA, int** matrixB, int process, int rows, int columns);

#endif