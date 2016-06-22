CC     = gcc
STD    = -std=c11
ERRFLG =
DEBUG  =

all: main hush builtin
	$(CC) $(STD) $(DEBUG) $(ERRFLG) -L readline obj/* -o bin/hush readline/libreadline.a -lcurses

main:
	mkdir -p obj bin
	$(CC) $(STD) $(DEBUG) $(ERRFLG) -c src/main.c -o obj/main.o

hush:
	$(CC) $(STD) $(DEBUG) $(ERRFLG) -c src/hush.c -o obj/hush.o

builtin:
	$(CC) $(STD) $(DEBUG) $(ERRFLG) -c src/builtin.c -o obj/builtin.o

