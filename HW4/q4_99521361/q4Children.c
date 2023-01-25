#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>
#include <errno.h>
#include <semaphore.h>

char* NameSet(int n, char set) {
    char* result = (char*) malloc(sizeof(char) * (7));
    for (int i = 0; i < n + 1; i++) result[i] = set;
    result[5] = '\0';
    return result;
}

int indx(){
    for (int i = 0; i < 5; i++) if (fork() == 0)return i;          
    return -1;
}

void bubbleSort(int* ptr){
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 4; j++) {
            if (ptr[j] > ptr[j + 1]) {
                int tmp = ptr[j];
                ptr[j] = ptr[j + 1];
                ptr[j + 1] = tmp;
            }
        }
    }
}
int main() {
    sem_t *sem2;
    sem2 = sem_open("hi", O_RDONLY, 0666, 0);

    int ind = indx();
    sem_t *sem_i = sem_open(NameSet(ind, 'i'), O_RDONLY, 0666, 0);
    
    int shm_fd = shm_open(NameSet(ind, 'u'), O_RDWR, 0666);
    int* ptr = (int *)mmap(0, 10000, PROT_WRITE, MAP_SHARED, shm_fd, 0);

    sem_wait(sem_i);

    bubbleSort(ptr);

    sem_post(sem2);

    sem_close(sem2);
    sem_close(sem_i);

    munmap(ptr, 10000);
    close(shm_fd);
    return 0;
}