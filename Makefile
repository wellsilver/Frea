cc = gcc
libs = -l pthread
flags = -Wall

main:
	$(cc) src/main.c $(flags) $(libs) -o main.o
clean:
	rm main.o