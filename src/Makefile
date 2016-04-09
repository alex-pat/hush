CC     = gcc
RLINE  = -lreadline
ERRFLG = 

all: main, hush, builtin
	$(CC) obj/* -o bin/hush

main:
	$(CC) -c src/main.c $(RLINE) -o obj/main.o

hush:
	$(CC) -c src/hush.c -o obj/hush.o

builtin:
	$(CC) -c src/builtin.c -o obj/builtin.o
