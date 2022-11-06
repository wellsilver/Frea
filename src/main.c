#include <stdio.h>
#include <stdlib.h>
#include <pthread.h> // mingw provides windows ports of pthread

typedef enum bool {BOOL_FALSE,BOOL_TRUE} bool; // just useful

//
#include "players/players.h"
#include "files.h"

int main(int argc,char **argv) {
	printf("Reading files");
	int loop;
	bool *files_that_exist = files_check();
	for (loop=1;loop<4;loop++) {
		if (files_that_exist[loop-1]==BOOL_FALSE) {
			
		}
	}
}