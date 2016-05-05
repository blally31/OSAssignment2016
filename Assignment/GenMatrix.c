#include <time.h>
#include <stdio.h>
#include <stdlib.h>
	

int main(int argc, char const *argv[])
{
	int ii, jj, rows, columns;

	srand(time(NULL));
	rows = rand() % 30;
	columns = rand() % 30;

	for (ii = 0; ii < rows; ii++)
	{
		for(jj = 0; jj < columns; jj++)
		{
			fscanf(fMatrix, "%d", &matrix[getIndex(ii, jj, columns)]);
		}
	}
	return 0;
}

	







int getIndex(int rows, int columns, int ncols)
{
	return rows*ncols + columns;
}