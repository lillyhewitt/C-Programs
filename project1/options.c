#include <unistd.h> 
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

void displayPid(char * name);
void displayStateInfo(char * name);
void amountOfVirtualMemory(char * name);
void amountOfUserTime(char * name);
void amountOfSystemTime(char * name);  
void displayCMDline(char * name);

// reads the /proc/PID/status file to find PID number of process
void displayPid(char * name) {
        FILE * file;
        char buffer[1024];
        size_t readFile;
        int PID;

	// create path with process
        char path[1000] = {0};
        strcat(path, "/proc/");
        strcat(path, name);
        strcat(path, "/status");

	// opens /proc/PID/status file
        file =  fopen(path, "r");
        
	int count = 1;
	while((readFile = fread(buffer, 1, sizeof(buffer), file) != 0)) {	
       		char * find = strstr(buffer, "Pid");
		if(find == NULL) {
			return;
		}
        	sscanf(find, "Pid : %d", &PID);
		if(count == 1) {
			printf("Process ID: %d\n", PID);
		}
		count++;
	}
	fclose(file);
 }

// reads the /proc/file/stat file to fine state of process
void displayStateInfo(char * name) {
        FILE * file;
        char buffer[1024];
        size_t readFile;
        char state;

        // create path with process
	char path[1000] = {0};
        strcat(path, "/proc/");
        strcat(path, name);
        strcat(path, "/stat");

	// opens /proc/PID/stat file
        file =  fopen(path, "r");
        readFile = fread(buffer, 1, sizeof(buffer), file);
        fclose(file);
        buffer[readFile] = '\0';

	// finds state in file and copies following number
        char * find = strstr(buffer, ") ");
        if(find == NULL) {
		return;
	}
        sscanf(find, ") %c", &state);
        printf("State information: %c\n", state);
}

// reads the /proc/file/stat file to fine utime of process
void amountOfUserTime(char * name) {
        FILE * file;
        char buffer[256];
	int count = 0;

        // create path with process
	char path[1000] = {0};
        strcat(path, "/proc/");
        strcat(path, name);
        strcat(path, "/stat");

	// opens /proc/PID/stat file
	file =  fopen(path, "r");
	// scans file until 14th string found (utime field)
        while(fscanf(file, "%99s", buffer) != EOF) {
                count++;
                if(count == 14) {
                        printf("Utime: %s\n", buffer);
                        break;
                }
        }
        fclose(file);
}

// reads the /proc/file/stat file to fine stime of process
void amountOfSystemTime(char * name) {
        FILE * file;
        char buffer[1024];
        int count = 0;

        // create path with process
	char path[1000] = {0};
        strcat(path, "/proc/");
        strcat(path, name);
        strcat(path, "/stat");

	// opens /proc/PID/stat file
	file =  fopen(path, "r");
	// scans file until 15th string is found (stime field)
        while(fscanf(file, "%99s", buffer) != EOF) {
                count++;
                if(count == 15) {
                        printf("Stime: %s\n", buffer);
                        break;
                }
        }
        fclose(file);
}


// reads the /proc/file/statm file to fine stime of process
void amountOfVirtualMemory(char * name) {
        FILE * file;
        char buffer[256];

	// create path with process
        char path[1000] = {0};
        strcat(path, "/proc/");
        strcat(path, name);
        strcat(path, "/statm");

	// opens /proc/PID/statm file
        file = fopen(path, "r");
	// scanfs file for first string and copies it to buffer
        fscanf(file, "%s", buffer);
        fclose(file);
        printf("Virtual memory: %s\n", buffer);
}

// reads the /proc/PID/cmdline file to find cmdline arguments
void displayCMDline(char * name) {
	FILE * file;
        size_t readFile;
        char * cmdArgs = malloc(sizeof(char) * 4096);

        // create path with process
	char path[1000] = {0};
        strcat(path, "/proc/");
        strcat(path, name);
        strcat(path, "/cmdline");

	// opens /proc/PID/cmdline file
        file =  fopen(path, "r");
        
        readFile = fread(cmdArgs, 1, sizeof(char) * 4096, file);
        fclose(file);
        cmdArgs[readFile] = '\0';

	//parse command line and print arguments
	puts("Command Line Arguments: \n");
	int index = 1;
	printf("\t[%d] ", index);
	for(size_t i = 0; i < readFile - 1; i++) {
		// check for EOF sign
		if(cmdArgs[i] == '\0') {
			// check for 2 EOF signs
			if(i < readFile - 1 && cmdArgs[i+1] == '\0') {
				break;

			}
			// indentation for new argument
			index++;
			printf("\n\t[%d] ", index);
		}
		else {
			// print argument
			printf("%c", cmdArgs[i]);
		}
	}
}
