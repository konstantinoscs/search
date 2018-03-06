CC = gcc
FLAGS = -Wall -g
DEPS = plist.h search.h sort.h trie.h utilities.h
NAME = minisearch
SOURCE = main.c plist.c search.c sort.c trie.c utilities.c
OBJ = $(SOURCE:.c=.o)

%.o : %.c
	$(CC) -c $<

$(NAME): $(OBJ)
	$(CC) $(FLAGS) -o $(NAME) $(OBJ) -lm

$(OBJ): $(DEPS)

.PHONY: clean

clean:
	rm *.o $(NAME)
