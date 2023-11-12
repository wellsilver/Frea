cc = gcc
libs = -l pthread
flags = 

# sudo tcpdump -i any -nn src host 10.0.2.15 and port 25565

all: main.o run clean
build: main.o

out:
	mkdir -p out
main.o:
	$(cc) src/main.c $(flags) $(libs) -o main.o
run:
	./main.o
clean:
	rm main.o
