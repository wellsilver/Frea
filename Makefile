cc = gcc
libs = -l pthread
flags = -Wall

all: main.o run clean
build: main.o

main.o:
	$(cc) src/main.c $(flags) $(libs) -o main.o
run:
	./main.o
clean:
	rm main.o
