CC = gcc
CFLAGS = -pthread
SRC = Projeto.c
TARGET = prog

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

clean:
	rm -f $(TARGET)
