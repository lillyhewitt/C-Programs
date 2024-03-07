// Lilly Hewitt
// Purpose of file: parse command line to extract information from /proc file
#include <unistd.h> // to implement getopt()
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <sys/stat.h>
#include <dirent.h>
#include <stdbool.h>
#include <ctype.h>
#include "options.c"

void checkArgs(char * name, bool getPId, bool getState, bool Utime, bool Stime, bool getMem, bool getCMDline); 

int main(int argc, char *argv[]) {
	// https://www.geeksforgeeks.org/getopt-function-in-c-to-parse-command-line-arguments/
	   // used to implement getopt()
	int opt;
	bool getPID = false;
	bool getState = false;
	bool getMem = false;
	bool getUtime = true;
	bool getStime = false;
	bool getCMDline = true;
	char * PID = NULL;
	int userID = getuid();
	char * uid = malloc(100);
	int checkP = -1;
	sprintf(uid, "%d", userID);

	// parsing command line
	while((opt = getopt(argc, argv, ":p:sUSvc")) != -1) { 
		switch(opt) {
			// checks for -p
			case 'p':
				// saves PID from next argument
				PID = optarg;
				checkP = 1;
				getPID = true;
				break;
			// checks for -s
			case 's':	
				getState = true;			
				break;
			// checks for -u
			case 'U':
				getUtime = false;
				break;
			// checks for -S
			case 'S':
				getStime = true;
				break;
			// checks for -v
			case 'v':
				getMem = true;
				break;
			// checks for -c
			case 'c':
				getCMDline = false;
				break;
		}
	}	
	if(checkP == -1) {
		perror("ERROR");
		exit(1);
	}
	// if PID was inputted, run inputted commands
	if(PID != NULL) {
		char check[1000] = {0};
		strcat(check, "/proc/");
		strcat(check, PID);
		strcat(check, "/status");
		FILE * file = fopen(check, "r");
		if(file != NULL) {
			checkArgs(PID, getPID, getState, getUtime, getStime, getMem, getCMDline); 
		}
		else {
			perror("ERROR.");
		}
	}	
	// if not -p inputted, run all process lists matching UID of user
	else if(!getPID) {
		getPID = true;
		DIR * current_directory = opendir("/proc");
	      	struct dirent * entry;
		// checks is directory exists
        	if(current_directory == NULL) {
                	perror("ERROR.");
        	}
        	while((entry = readdir(current_directory)) != NULL) {
                        if(entry -> d_name[0] != '.' && isdigit(entry -> d_name[0])) {  
		  		// create path to status file
				char path[1000] = {0};
			        strcat(path, "/proc/");
        			strcat(path, entry -> d_name);
        			strcat(path, "/status");

				// open proc/PID/status
				FILE * file = fopen(path, "r");
				size_t n = 0;
				char * line = NULL;

				// checks if process UID matches UID of user, runs commands if so
				while(getline(&line, &n, file) > 0) {
					if(strstr(line, "Uid") && strcmp(line, uid)) {
							checkArgs(entry -> d_name, getPID, getState, getUtime, getStime, getMem, getCMDline);
					}
				}
			}
		}
	}
	else {
		puts("ERROR.");
	}
	return 0;
} 

// function running the commands that were inputted in command line
void checkArgs(char * name, bool getPID, bool getState, bool getUtime, bool getStime, bool getMem, bool getCMDline) {
	if(getPID) {
        	displayPid(name);
        }
        if(getState) {
                displayStateInfo(name);
	}
        if(getUtime) {
                amountOfUserTime(name);
        }
        if(getStime) {
                amountOfSystemTime(name);
        }
        if(getMem) {
                amountOfVirtualMemory(name);
	}        
	if(getCMDline) {
		displayCMDline(name);
        } 
	puts("\n");
}
