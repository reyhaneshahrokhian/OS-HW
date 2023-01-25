#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <math.h>
#include <sys/wait.h>
#include <string.h>
#include <sys/time.h>

char* logMsg(char* fullLog);
int indx_pip(int pipes[5][2]);

int main() {
    int pipes[5][2];
    int pid;
    int index = indx_pip(pipes); 

    if(index == -1){ //parent
        int finish[5];
        for(int i = 0; i < 5; i++) finish[i] = 0;
        char str[1024] = "$"; // 'read' reading 1024 bytes
        int Count = 0;
        FILE* createFile = fopen("log.txt", "w+");
        fclose(createFile);
        FILE* fPtr = fopen("log.txt", "a");
        while (Count < 5) {
            for (int i = 0; i < 5; i++) {
                if (finish[i]) continue;
                
                read(pipes[i][0], &str, sizeof(char) * 1024);
                fputs(str, fPtr);
                fputs("\n", fPtr);
            
                if (strcmp(logMsg(str), "finish") == 0) {
                    finish[i] = 1;
                    Count++;
                }
            }
        }
    }else{ // fork() == 0 (child)
        char message[10] = "start";
        char str[1024] = "$"; // 'write' reading 1024 bytes
        int n = getpid() % 5 + 2;
        for (int i = 0; i < n; i++) {
            struct timeval current;
            gettimeofday(&current, NULL);
            time_t sec = current.tv_sec; 
            if (i == n - 1) strcpy(message, "finish");
            else if (i > 0) strcpy(message, "running");
                      
            sprintf(str, "pid:%d timestamp:%ld log:%s",getpid(), sec, message);
            printf("pid:%d timestamp:%ld log:%s\n",getpid(), sec, message); 
            write(pipes[index][1], &str, sizeof(char) * 1024);   
            sleep(1);
        } 
    }
    return 0;
}

int indx_pip(int pipes[5][2]){
    for (int i = 0; i < 5; i++) {
        pipe(pipes[i]);
        if (fork() == 0)return i;       
    }
    return -1;
}

char* logMsg(char* fullLog) {
    for (int i = 0; i < strlen(fullLog) - 3; i++) 
        if (fullLog[i] == 'l' && fullLog[i + 1] == 'o' && fullLog[i + 2] == 'g' && fullLog[i + 3] == ':') return &fullLog[i + 4];   
    return NULL;
}