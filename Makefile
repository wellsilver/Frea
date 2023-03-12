cc = gcc
libs = -lpthread
flags = -Wall

ifeq ($(OS),Windows_NT)
	delcmd = del
else
	delcmd = rm
endif
main:
	$(cc) src/main.c $(flags) $(libs) -o main.o
  
clean:
	$(delcmd) main.o