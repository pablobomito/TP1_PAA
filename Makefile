CC = gcc


CFLAGS = -Wall -Wextra -g


SRC = main.c leitura/leitura.c backtracking/algoritmo.c gerador_mapa/geradorMapa.c

TARGET = programa

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

clean:
	rm -f $(TARGET)