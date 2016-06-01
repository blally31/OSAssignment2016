/****************************************************************************
** FILE: pmmsThread.c
** AUTHOR: Brendan Lally
** STUDENT ID: 18407220
** UNIT: COMP2006 (Operating Systems)
** PURPOSE: Imports two matrices and calculates the product using multi-threading
**			while summing each value into a row subtotal and overall total.
**			POSIX threads are used for thread creating and synchronization.
** LAST MOD: 07/05/16
****************************************************************************
**/

#include "pmmsThread.h"

int main(int argc, char *argv[])
{
	int i, j, total = 0, errorCheck = FALSE;
	pthread_t *tid; /*Thread Identifier */
	
	/* Error check the number of command line arguements. */
	if (argc != 6)
	{
		printf("Incorrect number of command line arguements!\n");
		errorCheck = TRUE;
	}
	/* If there was the correct number of command line arguments
	   then continue with the program. */
	if (errorCheck != TRUE)
	{
		s = (Shared*)malloc(sizeof(Shared));

		/* Convert command-line args from string to int*/
		s->m = atoi(argv[3]);
		s->n = atoi(argv[4]);
		s->k = atoi(argv[5]);
		
		if (s->m < 0 || s->n < 0 || s->k < 0)
		{
			printf("One or more matrix dimensions are invalid!\n");
			return 0;
		}

		s->subtotal = 0;
		s->thread = 0;
		s->matrixA = (int*)malloc(sizeof(int)*s->m*s->n);
		s->matrixB = (int*)malloc(sizeof(int)*s->n*s->k);
		s->matrixC = (int*)malloc(sizeof(int)*s->m*s->k);

		readMatrix(argv[1], s->m, s->n, s->matrixA);
		readMatrix(argv[2], s->n, s->k, s->matrixB);

		tid = (pthread_t*)malloc(sizeof(pthread_t)*s->m);

		/* Initialise mutex and condition variables */
		pthread_mutex_init(&mutex, NULL);
		pthread_cond_init(&empty, NULL);
		pthread_cond_init(&full, NULL);

		/* Create m threads*/
		for (i = 0; i < s->m; i++)
		{
			/*Thread creation error*/
			if (pthread_create(&tid[i], NULL, calculateSubtotal, 
				(void *)(intptr_t)(i+1)) != 0)
			{
				printf("Thread Creation Failed!");
				exit(0);
			}
			/* Mark each thread so its resources are auto released when the 
			   thread terminates. */
			pthread_detach(tid[i]);	
		}

		/*Consumer Process*/
		for (j = 1; j <= s->m; j++)
		{
			pthread_mutex_lock(&mutex);
			while (s->subtotal == 0)
			{
				pthread_cond_wait(&full, &mutex);
			}
			/*Consume buffer*/
			printf("Subtotal produced by thread with ID %lu: %d\n", 
				(unsigned long)s->thread, s->subtotal);
			total += s->subtotal;
			s->subtotal = 0;
	      	pthread_cond_signal(&empty);
			pthread_mutex_unlock(&mutex);
		}
		printf("Total: %d\n", total);

		/* Clean up allocated memory. */
		free(tid);
		free(s->matrixA);
		free(s->matrixB);
		free(s->matrixC);
		free(s);

		pthread_mutex_destroy(&mutex);	/* Free up mutex */
  		pthread_cond_destroy(&empty);	/* Free up producer condition variable */
  		pthread_cond_destroy(&full);	/* Free up consumer condition variable */
		
	}
	return 0;
}

/* Reads the matrix from a file into memory. Imports the filename, 
** matrix dimensions and a pointer to the shared memory where the matrix
** will be stored.
*/
void readMatrix(char* filename, int rows, int columns, int* matrix)
{
	int ii, jj;
	FILE* fMatrix = fopen(filename, "r");

	if (fMatrix == NULL)
	{
		perror("Error opening file");
	}
	else
	{
		for (ii = 0; ii < rows; ii++)
		{
			for(jj = 0; jj < columns; jj++)
			{
				fscanf(fMatrix, "%d", &matrix[getIndex(ii, jj, columns)]);
			}
		}
	}

	fclose(fMatrix);
}

/* The producer function that each created thread runs. Each producer thread
** calculates the subtotal for its given row in the matrix. Imports a pointer
** to the row number the thread is to calculate.
*/
void* calculateSubtotal(void* ptr)
{
	int i, j, value, subtotal = 0, process, n, k;

	/* Each thread should get its values mutually exclusively. */
	pthread_mutex_lock(&mutex);
	process = (intptr_t)ptr - 1;
	n = s->n; 
	k = s->k; 
	pthread_mutex_unlock(&mutex);

	for (i = 0; i < k; i++)
	{
		value = 0;

		for (j = 0; j < n; j++)
		{
			value += s->matrixA[getIndex(process, j, n)]*s->matrixB[getIndex(j, i, k)];
		}
		
		s->matrixC[getIndex(process, i, k)] = value;
		subtotal += value;
	}

	pthread_mutex_lock(&mutex);
	
	while (s->subtotal != 0)
	{
		pthread_cond_wait(&empty, &mutex);
	}
	/* Produce subtotal to be consumed. Stores the thread id and the subtotal. */
	s->thread = pthread_self();
	s->subtotal = subtotal;

	pthread_cond_signal(&full);
	pthread_mutex_unlock(&mutex);

	return 0;
}

/* Function to calculate the index of the 1D array that corresponds to a given
** 2D array. Imports the rows, columns and number of columns for a 2D array and
** returns the index.
*/
int getIndex(int rows, int columns, int ncols)
{
	return rows*ncols + columns;
}
