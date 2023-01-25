#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>
#include <semaphore.h>
#include <limits.h>

int countFinished(int * a) {
    int res = 0;
    for (int i = 0; i < 5; i++) res += (a[i] == 5);
    return res;
}

char* NameSet(int n, char set) {
    char* result = (char*) malloc(sizeof(char) * (7));
    for (int i = 0; i < n + 1; i++) result[i] = set;
    result[5] = '\0';
    return result;
}

int main() {
    sem_t *sem[5];
    for (int i = 0; i < 5; i++) sem[i] = sem_open(NameSet(i, 'i'), O_CREAT, 0666, 0);
    
    sem_t *sem2 = sem_open("hi", O_CREAT, 0666, 0);

    int a[25];
    srand(time(NULL));
    for (int i = 0; i < 25; i++) a[i] = rand();
    
    int shm_fd[5];
    int* ptr[5];
    for (int i = 0; i < 5; i++) { 
        shm_fd[i] = shm_open(NameSet(i, 'u'), O_CREAT | O_RDWR, 0666);
        ftruncate(shm_fd[i], 10000);
        ptr[i] = (int *)mmap(0, 10000, PROT_WRITE, MAP_SHARED, shm_fd[i], 0);
        for (int j = 0; j < 5; j++) ptr[i][j] = a[i * 5 + j];
        
        sem_post(sem[i]);
    }
    
    for (int i = 0; i < 5; i++) sem_wait(sem2); 

    int counters[5] = {0};
    int primary_indx = 0;
    while (countFinished(counters) != 5) {
        int min = INT_MAX;
        int min_indx;
        for (int i = 0; i < 5; i++) {
            if (counters[i] != 5 && ptr[i][counters[i]] < min) {
                min = ptr[i][counters[i]];
                min_indx = i;
            }
        }
        a[primary_indx++] = min;
        counters[min_indx]++;
    }

    for (int i = 0; i < 25; i++) printf("%d\n", a[i]);
    
    for (int i = 0; i < 5; i++) {
        munmap(ptr[i], 10000);
        close(shm_fd[i]);
        sem_close(sem[i]);
        sem_unlink(NameSet(i, 'i'));
    }
    sem_close(sem2);
    sem_unlink("hi");
    return 0;
}