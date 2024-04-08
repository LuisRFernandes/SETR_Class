DEFAULT_TARGET=main
OBJECTS = cmdproc.o main.o


CFLAGS = -g -Wall -O3
LDLIBS=
CC=gcc

# Alvo padrão
$(DEFAULT_TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $(DEFAULT_TARGET) $(OBJECTS)

# Limpar arquivos objeto e executável
clean:
	rm -f *.o $(DEFAULT_TARGET)

# Alvo para executar o programa
run:	$(DEFAULT_TARGET)
	./$(DEFAULT_TARGET)
