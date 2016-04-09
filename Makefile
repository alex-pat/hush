CC     = gcc
RLINE  = -lreadline
STD    = -std=c11
ERRFLG = 

all: main hush builtin
	$(CC) $(STD) $(RLINE) obj/* -o bin/hush

main:
	$(CC) $(STD) -c src/main.c $(RLINE) -o obj/main.o

hush:
	$(CC) $(STD) -c src/hush.c -o obj/hush.o

builtin:
	$(CC) $(STD) -c src/builtin.c -o obj/builtin.o
