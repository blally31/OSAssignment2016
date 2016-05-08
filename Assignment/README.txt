#README.txt


Steps to run Operating Systems Assignment.

	****Using Processes****

1. Navigate to the target directory ~/OS/assignment
2. Run the makefile using the command 'make' to compile the program
3. Run the program using ./pmms matrixA matrixB M N K ** 
	
	** where matrixA and matrixB are two text files that contain an M x N and an N x K matrix respectively.
	 	M is the number of rows in matrixA
	 	N is the number of columns in matrixA and rows in matrixB
	 	K is the number of columns in matrixB

		eg. ./pmms test2MatrixA test2MatrixB 20 11 40
		
		
	****Using Threads****

1. Navigate to the target directory ~/OS/assignment/Threads
2. Run the makefile using the command 'make' to compile the program
3. Run the program using ./pmmsThread matrixA matrixB M N K **
	
	** where matrixA and matrixB are two text files that contain an M x N and an N x K matrix respectively.
	 	M is the number of rows in matrixA
	 	N is the number of columns in matrixA and rows in matrixB
	 	K is the number of columns in matrixB
		
		eg. ./pmms test4MatrixA test4MatrixB 100 100 100