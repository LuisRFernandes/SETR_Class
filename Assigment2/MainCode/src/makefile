DEFAULT_TARGET=main
OBJECTS = cmdproc.o main.o


CFLAGS = -g -Wall
LDLIBS=
CC=gcc


$(DEFAULT_TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $(DEFAULT_TARGET) $(OBJECTS)


clean:
	rm -f *.o $(DEFAULT_TARGET)

run:	$(DEFAULT_TARGET)
	./$(DEFAULT_TARGET)
