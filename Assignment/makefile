CC = gcc
CFLAGS = -Wall -pedantic 
OBJ = pmms.o 
EXEC = pmms

$(EXEC): $(OBJ)
	$(CC) -g $(OBJ) -o $(EXEC) -lm -lrt -pthread

pmms.o: pmms.c pmms.h
	$(CC) -c pmms.c $(CFLAGS)

clean:
	rm -f $(EXEC) $(OBJ)