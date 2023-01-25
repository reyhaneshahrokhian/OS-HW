#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <string.h>

int main(int argc, char *argv[]){
    char command[300] = "cat ";
    int i =4;
    while(argv[1][i - 4] != '\0'){
        command[i] = argv[1][i - 4];
        i++;
    }
    //printf("%s",command);
    system(command);
}