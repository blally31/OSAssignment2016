#include "pmms.h"

int main(int argc, char *argv[])
{
	int i, j, m, n, k, pid, total = 0, errorCheck = FALSE;
	/* 2D integer arrays to point to shared memory block. */
	int *matrixA_ptr = NULL, *matrixB_ptr = NULL, *matrixC_ptr = NULL;
	/* Names of the shared memory objects. */
	char *matrixA = "matrixA", *matrixB = "matrixB", *matrixC = "matrixC", 
		*subtotal = "subtotal";
	Shared *ptr;

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

		if (m < 0 || n < 0 || k < 0)
		{
			printf("One or more matrix dimensions are invalid!\n");
			return 0;
		}

		/* Create shared memory objects*/
		matrixA_ptr = (int*)createMemory(matrixA, sizeof(int)*m*n);
		matrixB_ptr = (int*)createMemory(matrixB, sizeof(int)*n*k);
		matrixC_ptr = (int*)createMemory(matrixC, sizeof(int)*m*k);
		ptr = (Shared*)createMemory(subtotal, sizeof(Shared));

		readMatrix(argv[1], m, n, matrixA_ptr);
		readMatrix(argv[2], n, k, matrixB_ptr);

		/* Initialise semaphores. */
		sem_init(&ptr->mutex, 1, 1);
		sem_init(&ptr->empty, 1, 1);
		sem_init(&ptr->full, 1, 0);

		/* Kills Zombie processes. */
		signal(SIGCHLD, SIG_IGN);

		/* Create m child processes. */
		for (i = 0; i < m; i++)
		{
			pid = fork();
			/*Fork error*/
			if (pid < 0)
			{
				perror("Fork Failed");
			}
			/*Do child processing*/
			else if (pid == 0)
			{
				childProcess(matrixA_ptr, matrixB_ptr, matrixC_ptr, ptr, i, m, n, k);
				exit(0);
			}
		}
		/*Parent Process*/
		for (j = 0; j < m; j++)
		{
			sem_wait(&ptr->full);
			sem_wait(&ptr->mutex);
			/*Consume buffer*/
			printf("Subtotal produced by process with ID x%d: %d\n", ptr->process+1, ptr->subtotal);
			total += ptr->subtotal;
	      	sem_post(&ptr->mutex);
	      	sem_post(&ptr->empty);
		}
		printf("Total: %d\n", total);	
	}
	/*sleep(20);*/
	return 0;
}

/* Creates a shared memory block and returns a pointer to that block. Imports
** the name and size of a block to create. 
*/
void* createMemory(char* name, int size)
{
	int shm_fd;
	void* shm_ptr;

	/* Create shared memory object. */
	shm_fd = shm_open(name, O_RDWR | O_CREAT, 0666);

	/* Configure the size of the shared memory object. */
	ftruncate(shm_fd, size);

	/* Memory map the shared object. */
	shm_ptr = mmap(0, size, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);

	close(shm_fd);

	if (shm_ptr == MAP_FAILED)
	{
		printf("MEMORY MAP FAILED\n");
		exit(0);
	}	

	return shm_ptr;
}

/* Reads the matrix from a file into shared memory. Imports the filename, 
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

/* The producer function that each child process runs. Each child producer 
** calculates the subtotal for its given row in the matrix. Imports pointers
** to all the shared memory blocks
*/
void childProcess(int* matrixA, int* matrixB, int* matrixC, Shared* ptr, 
	int process, int m, int n, int k)
{
	int i, subtotal = 0;

	for (i = 0; i < k; i++)
	{
		
		matrixC[getIndex(process, i, k)] = 
			matrixA[getIndex(process, 0, n)]*matrixB[getIndex(0, i, k)] 
				+ matrixA[getIndex(process, 1, n)]*matrixB[getIndex(1, i, k)];

		subtotal += matrixC[getIndex(process, i, k)];
	}
	
	sem_wait(&ptr->empty);
	sem_wait(&ptr->mutex);
	/*Produce subtotal to be consumed. Stores the process number and the subtotal*/
	ptr->process = process;
	ptr->subtotal = subtotal;
	sem_post(&ptr->mutex);
	sem_post(&ptr->full);
}

/* Function to calculate the index of the 1D array that corresponds to a given
** 2D array. Imports the rows, columns and number of columns for a 2D array and
** returns the index.
*/
int getIndex(int rows, int columns, int ncols)
{
	return rows*ncols + columns;
}












void printMatrix(int* matrix, int rows, int columns)
{
	int ii, jj;
	/*print out each column for each row of the 2D array for testing purposes*/
	for (ii = 0; ii < rows; ii++)
	{
		for(jj = 0; jj < columns; jj++)
		{
			printf("%d\n", matrix[getIndex(ii, jj, columns)]);
		}
	}
	printf("\n");
}