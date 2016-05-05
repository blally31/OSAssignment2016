CC = gcc
CFLAGS = -Wall -pedantic -ansi
OBJ = pmmsThread.o 
EXEC = pmmsThread

$(EXEC): $(OBJ)
	$(CC) -g $(OBJ) -o $(EXEC) -lm -lrt -pthread

pmmsThread.o: pmmsThread.c pmmsThread.h
	$(CC) -c pmmsThread.c $(CFLAGS)

clean:
	rm -f $(EXEC) $(OBJ)