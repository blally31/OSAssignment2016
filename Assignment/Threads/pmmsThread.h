#ifndef PMMS_HEADER
#define PMMS_HEADER

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>

#define FALSE 0
#define TRUE !FALSE

/* Stores the data required for each thread to compute the subtotal*/
typedef struct 
{
	int subtotal; /*Subtotal of ith row*/
	pthread_t thread; /*Thread ID*/
	int* matrixA;
	int* matrixB;
	int* matrixC;
	int m; /*Number of rows for matrix A and matrix C*/
	int n; /*Number of columns for matrix A and rows for matrix B*/
	int k; /*Number of columns for matrix B and matrix C*/
} Shared;

/*Shared objects - Global variables*/
Shared* s;
pthread_mutex_t mutex;
pthread_cond_t empty; /*Producer variable*/
pthread_cond_t full; /*Consumer variable*/

/*Function definitions*/
void* createMemory(char* name, int size);
void readMatrix(char* filename, int rows, int columns, int* matrix);
void printMatrix(int* matrix, int rows, int columns);
int getIndex(int rows, int columns, int ncols);
void* calculateSubtotal(void* ptr);

#endif