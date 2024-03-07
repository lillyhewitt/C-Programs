// Write a program to report the free memory in the system and the number of memory buffers

#include <stdio.h>
#include <string.h>
#include <stddef.h>


int findFreeMem();
int numOfMemBuffers();

int main() {
	printf("The free memory is: %d kB\n", findFreeMem());
	printf("The number of memory buffers is: %d kB\n", numOfMemBuffers());
	return 0;
}


int findFreeMem(){
	FILE * file;
        char buffer[1024];
        size_t readFile;
        int freeMem;

        file =  fopen("/proc/meminfo", "r");
        readFile = fread(buffer, 1, sizeof(buffer), file);
        fclose(file);
        buffer[readFile] = '\0';

        char * find = strstr(buffer, "MemFree");
        if(find == NULL) {
                return 1;
        }
        sscanf(find, "MemFree : %d", &freeMem);
        return freeMem;
}

int numOfMemBuffers() {
	FILE * file;
        char buffer[1024];
        size_t readFile;
        int numBuffers;

        file =  fopen("/proc/meminfo", "r");
        readFile = fread(buffer, 1, sizeof(buffer), file);
        fclose(file);
        buffer[readFile] = '\0';

        char * find = strstr(buffer, "Buffers");
        if(find == NULL) {
                return 1;
        }
        sscanf(find, "Buffers : %d", &numBuffers);
        return numBuffers;
}
