#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include "pmms1.h"

int main(int argc, char *argv[])
{
	int m, n, k, errorCheck = FALSE;
	/*2D integer arrays*/
	int **matrixA_ptr = NULL, **matrixB_ptr = NULL/*, **matrixC_ptr = NULL*/;
	/* Names of the shared memory objects. */

	/* Error check the number of command line arguements. */
	if (argc != 6)
	{
		printf("Incorrect number of command line arguements!\n");
		errorCheck = TRUE;
	}

	/* If there was the correct number of command line arguments
	   then continue with the program */
	if (errorCheck != TRUE)
	{
		/* Convert command-line args from string to int*/
		m = atoi(argv[3]);
		n = atoi(argv[4]);
		k = atoi(argv[5]);

		matrixA_ptr = readMatrix(argv[1], m, n);
		matrixB_ptr = readMatrix(argv[2], n, k);

		calculateC(matrixA_ptr, matrixB_ptr, 0, m, k);
		calculateC(matrixA_ptr, matrixB_ptr, 1, m, k);
		calculateC(matrixA_ptr, matrixB_ptr, 2, m, k);
/*		printMatrix(matrixA_ptr, m, n);
		printMatrix(matrixB_ptr, n, k);*/

	}
	return 0;
}

/* Reads the matrix from a File*/
int** readMatrix(char* filename, int rows, int columns)
{
	int i, ii, jj;
	int** matrix;
	FILE* fMatrix = fopen(filename, "r");
	
	if (fMatrix == NULL)
	{
		perror("Error opening file");
	}
	else
	{
		matrix = (int**)malloc(rows * sizeof(int*));
		for (i = 0; i < rows; i++)
		{
			matrix[i] = (int*)malloc(columns * sizeof(int));
		}

		/*scan each column for each row of the test file and store in 2D array */
		for (ii = 0; ii < rows; ii++)
		{
			for(jj = 0; jj < columns; jj++)
			{
				fscanf(fMatrix, "%d", &matrix[ii][jj]);
			}
		}
	}
	return matrix;
}

void printMatrix(int** matrix, int rows, int columns)
{
	int ii, jj;
	/*print out each column for each row of the 2D array for testing purposes*/
	for (ii = 0; ii < rows; ii++)
	{
		for(jj = 0; jj < columns; jj++)
		{
			printf("%d\n", matrix[ii][jj]);
		}
	}
	printf("\n");
}

void calculateC(int** matrixA, int** matrixB, int process, int rows, int columns)
{
	int i, j, subtotal = 0;
	int ** matrixC;

	matrixC = (int**)malloc(rows * sizeof(int*));
	for (i = 0; i < rows; i++)
	{
		matrixC[i] = (int*)malloc(columns * sizeof(int));
	}

	for (j = 0; j < columns; j++)
	{
		matrixC[process][j] = (matrixA[process][0]*matrixB[0][j]) + (matrixA[process][1]*matrixB[1][j]);
		subtotal += matrixC[process][j];
	}
	
	printMatrix(matrixC, rows, columns);
	printf("Subtotal = %d\n", subtotal);
}