CC = gcc
CFLAGS = -std=c17 -Wall -Werror -pedantic -g

HookBook: hookbook.o pirate_list.o pirate.o helper.o
	$(CC) $(CFLAGS) -o $@ $^

hookbook.o: hookbook.c

pirate_list.o: pirate_list.c 

pirate.o: pirate.c

helper.o: helper.c

clean:
	rm -f *.o
	rm -f HookBook
