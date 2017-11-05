#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <dirent.h>
#include <sys/stat.h>
#include <stdbool.h>
#include <string.h>

int toatalLines = 0;
int filesNotOpen = 0;
// function to read file
void readFile(const char *fname){
    FILE *fp;
	int line_num = 1;
	int find_result = 0;
	char temp[512];
    int linesInFile = 0;

	//gcc users
	if((fp = fopen(fname, "r")) == NULL) {
		printf("Cannot open file");
        filesNotOpen++;
        return;
	}

	while(fgets(temp, 512, fp) != NULL) {
		linesInFile++;
	}
    printf("%s : %d\n",fname, linesInFile);
    toatalLines = toatalLines + linesInFile;
	//Close the file if still open.
	if(fp) {
		fclose(fp);
	}
}

// function to concat 2 pointer chars

char* concat(const char* str1, const char* str2){
    char * str3 = (char *) malloc(1 + strlen(str1)+ strlen(str2) );
    strcpy(str3, str1);
    strcat(str3, str2);
    return str3;
}

// this is to check if it is a directory
bool is_dir(const char* path) {
    struct stat buf;
    stat(path, &buf);
    return S_ISDIR(buf.st_mode);
}

int readingDir(const char* path){

    struct dirent *de;  // Pointer for directory entry

    // opendir() returns a pointer of DIR type.
    DIR *dr = opendir(path);

    if (dr == NULL)  // opendir returns NULL if couldn't open directory
    {
        printf("Could not open current directory" );
        return 0;
    }

    char *p;
    p = ".";
    char *q;
    q = "..";
    char *r;
    r = ".DS_Store";

    // for readdir()
    while ((de = readdir(dr)) != NULL){
        if (!strcmp(de->d_name, p) ||!strcmp(de->d_name, q) ||!strcmp(de->d_name, r)) {
            continue;
        }else{
            char *slash = "/";
            char *concatedPath = concat(path, slash);
            concatedPath = concat(concatedPath, de->d_name);
            //printf("%s\n",concatedPath);
            bool x = is_dir(concatedPath);
            if (x) {
                readingDir(concatedPath);
                //printf("Is dir\n");
            }else{
                readFile(concatedPath);
                //printf("Is file\n");
            }
        }
    }

    closedir(dr);
    return 1;
}

int main(void)
{
    printf("Starting the program\n");
    readingDir(".");

    printf("Toatl %d Lines of code has been written \n",toatalLines );
    printf("Toal files could not be open: %d\n", filesNotOpen);
    return 0;
}