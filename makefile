# Nome do executável
EXEC = ArvoreBPlus

# Compilador
CC = gcc

# Flags de compilação
CFLAGS = -Wall -Wextra -g

# Arquivos-fonte
SRCS = main.c BPlusTree.c

all:
	$(CC) $(CFLAGS) -o $(EXEC) $(SRCS)

clean:
	rm -f $(OBJS) $(EXEC)
