CC = gcc
CFLAGS = -pthread
SRC = Projeto.c
EXE = prog

all: $(EXE)

$(EXE): $(SRC)
	$(CC) $(CFLAGS) -o $(EXE) $(SRC)

clean:
	rm -f $(EXE)
