cc = g++
libs = -l pthread
flags = -g -O3

# sudo tcpdump -i any -nn src host 10.0.2.15 and port 25565

all: build run clean

out:
	mkdir -p out
build:
	$(cc) src/main.cpp $(flags) $(libs) -o Frea
run:
	./Frea
clean:
	rm Frea