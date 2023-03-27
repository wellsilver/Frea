cc = g++
libs = -lpthread
flags = -Wall

ifeq ($(OS),Windows_NT)
	delcmd = del
else
	delcmd = rm
endif
main:
	$(cc) src/main.cpp $(flags) $(libs) -o main.o
  
clean:
	$(delcmd) main.o