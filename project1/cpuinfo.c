// write a program to report the number of processors and the amount of cache in each CPU.
#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
	FILE * file;
        char buffer[1024];
        size_t readFile;
        int processorNum;
	int cacheSize;

        file =  fopen("/proc/cpuinfo", "r");

	while((readFile = fread(buffer, 1, sizeof(buffer), file) != 0)) {	
       		char * find = strstr(buffer, "processor");
   		if(find == NULL) {
               		exit(1);
        	}
        	sscanf(find, "processor : %d", &processorNum);
		printf("Process: %d\n", processorNum);
   
            	char * findCache = strstr(buffer, "cache size");
        	if(findCache == NULL) {
                	exit(1);
        	}
        	sscanf(findCache, "cache size : %d", &cacheSize);
 		printf("Cache size: %d KB\n", cacheSize);
	}
	fclose(file);

	return 0;
}
