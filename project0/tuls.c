#include <dirent.h> //file system
#include <sys/stat.h> //check if file or not
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void tuls(char * dir);

int main(int argc, char *argv[]) {
	struct dirent **namelist;
	int n;

	// checks if input is valid
	if(argc >= 1) {
		// prints all files and directories in current directory
		if(argc == 1) {
			n = scandir(".", &namelist, NULL, alphasort);
			// checks if scandir() worked correctly, ends program if not
			if(n == -1){
				perror("tuls: cannot open directory");
				exit(1);
			}
			// prints all directories and files in current directory
			while(n--) {
				 // only displays actual files and directories, not branches 
				 if(strcmp(namelist[n] -> d_name, ".") && strcmp(namelist[n] -> d_name, "..")) {
					printf("%s\n", namelist[n] -> d_name);
					free(namelist[n]);
				}
			}
		}
		// if a specified directory is called, runs the tuls function 
		else {
			tuls(argv[1]);
		}
	}
	// ends program if input is invalid
	else {
		printf("Invalid input.");
		exit(1);
	}

	// ends program
	exit(0);
}

// prints all the files and directories in the current directory and all contents in subdirectories as well
void tuls(char * dir) {
	// updates what directory we should be in
	DIR * current_directory = opendir(dir);
	struct dirent * entry;
	struct stat statbuf;

	// checks is directory exists
	if(current_directory == NULL) {
		return;
	}
	
	// checks if directory is readable
       	entry = readdir(current_directory);
        while(entry != NULL) {
		// use stat() to find out file information
		stat(entry -> d_name, &statbuf);

		// prints out file and/or directory name 
		if(strcmp(entry -> d_name, ".") && strcmp(entry -> d_name, "..")) {
			printf("%s\n", entry -> d_name);
		}

		// checks if file is a directory, upates path and reruns tuls() so that the directory's contents are printed as well
		if((S_ISDIR(statbuf.st_mode)) && (strcmp(entry -> d_name, ".") != 0) && (strcmp(entry -> d_name, "..") != 0)) {
			char path[1000] = {0};
			strcat(path, dir);
		       	strcat(path, "/");
			strcat(path, entry -> d_name);
			tuls(path);
		}
		entry = readdir(current_directory);
	}
	// close directory once all branches are read
	closedir(current_directory);
}
