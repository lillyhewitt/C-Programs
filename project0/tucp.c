#include <dirent.h> //file system
#include <sys/stat.h> //check if file or not
#include <stdio.h> //open, close, write to files
#include <stdlib.h> //exit program if errors occur
#include <string.h>
#include <unistd.h>
#include <errno.h>

void tucp(char * source, char * destination);

int main(int argc, char *argv[]) {
	struct stat statbuf;
        stat(argv[argc - 1], &statbuf);

	 // checks if 1 argument is a file, runs tucp() with inputted file
        if((argc == 2) && S_ISREG(statbuf.st_mode)) {
            tucp(argv[1], argv[1]);
        }
        // checks if last argument is a directory, runs tucp() function with each inputted file and directory
        else if(S_ISDIR(statbuf.st_mode)) {
                for(int i = 1; i < argc - 1; i++) {
			// allocates memory to create path to inputted directory
       			char * path = (char *) malloc(4000);
			// copies inputted directory to path
			strcpy(path, argv[argc - 1]);
			// adds / to path 
			strcat(path, "/");
			// adds source file to path
			strcat(path, argv[i]);
			// runs tucp() with source file and path to directory
			tucp(argv[i], path);
			// free up memory space once path is used
			free(path);
                }
        }
	// checks if 2 argyments are files, run tucp() with source and destination files
	else if(argc < 4) {
		tucp(argv[1], argv[2]);
	}
        // makes sure enough arguments are inputted
        else {
	    // prints error if no files are inputted
            perror("Invalid number of arguments or file(s) do not exist.");
            exit(0);
        }

        // ends program
        return(0);
}

void tucp(char * source, char * destination) {
        // open source and desintation file in binary
	FILE * file = fopen(source, "rb"); 
	FILE * copy = fopen(destination, "wb");
	// check if files opened/exist
	if((file == NULL) || (copy == NULL)) {
        	// prints error if one of files does not open
		perror("Could not copy file.");
                exit(1);
        }

	// use stat to find information about file contents
	struct stat statbuf;
	stat(source, &statbuf);
	
	// create memory space for file contents
	char * text = (char *) malloc(statbuf.st_size);
	
	// read contents from source file
	fread(text, 1, statbuf.st_size, file); 

	// write source file contens to the copy file
	fwrite(text, 1, statbuf.st_size, copy);

	// close both files
	fclose(file);
	fclose(copy);

	// free memory space used for file contents
	free(text);
}
