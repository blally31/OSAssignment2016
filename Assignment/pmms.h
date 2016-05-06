#ifndef PMMS_HEADER
#define PMMS_HEADER

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>
#include <sys/types.h>
#include <semaphore.h>
#include <signal.h>

#define FALSE 0
#define TRUE !FALSE

/* Stores the semaphores and buffer for all the processes. */
typedef struct 
{
	int subtotal; /*Subtotal of ith row*/
	int process; /*Process ID*/
	sem_t mutex;
	sem_t empty; /*Producer semaphore*/
	sem_t full; /*Consumer semaphore*/
} Shared;

/*Function definitions*/
void* createMemory(char* name, int size);
void readMatrix(char* filename, int rows, int columns, int* matrix);
void printMatrix(int* matrix, int rows, int columns);
int getIndex(int rows, int columns, int ncols);
void childProcess(int* matrixA, int* matrixB, int* matrixC, Shared* ptr, int process, int m, int n, int k);

#endif