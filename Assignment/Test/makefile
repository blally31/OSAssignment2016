CC = gcc
CFLAGS = -Wall -pedantic -ansi
OBJ = pmms1.o 
EXEC = pmms

$(EXEC): $(OBJ)
	$(CC) -g $(OBJ) -o $(EXEC) -lm -lrt

pmms1.o: pmms1.c pmms1.h
	$(CC) -c pmms1.c $(CFLAGS)

clean:
	rm -f $(EXEC) $(OBJ)