#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

int main(int argc, char *argv[]){
    int status;
    char *arg[] = {"cat", argv[1], NULL};
    pid_t pid = fork();
    if(pid == 0)			
	{
		status = execvp("cat", arg);

		while (wait(&status) != pid);
	}
}