CC     = gcc
RLINE  = -lreadline
STD    = -std=c11
ERRFLG =
DEBUG = 

all: main hush builtin
	$(CC) $(STD) $(DEBUG) $(RLINE) obj/* -o bin/hush

main:
	$(CC) $(STD) $(DEBUG) $(ERRFLG) -c src/main.c $(RLINE) -o obj/main.o

hush:
	$(CC) $(STD) $(DEBUG) $(ERRFLG) -c src/hush.c -o obj/hush.o

builtin:
	$(CC) $(STD) $(DEBUG) $(ERRFLG) -c src/builtin.c -o obj/builtin.o
