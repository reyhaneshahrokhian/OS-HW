//99521361
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>

int state[5];
int phil[5] = { 0, 1, 2, 3, 4 };
sem_t mutex;
sem_t sem[5];
 
void checker(int phnum)
{
    if (state[phnum] == 1 && state[(phnum + 4) % 5] != 0 && state[(phnum + 1) % 5] != 0) {
        state[phnum] = 0;
        printf("Philosopher %d takes fork %d and %d\n", phnum + 1, (phnum + 4) % 5 + 1, phnum + 1);
        printf("Philosopher %d is eating\n", phnum + 1);
        sem_post(&sem[phnum]);
    }
}
 
void put_down(int phnum)
{
    sem_wait(&mutex);
    state[phnum] = 2;
    printf("Philosopher %d putting fork %d and %d down\n", phnum + 1, (phnum + 4) % 5 + 1, phnum + 1);
    checker((phnum + 4) % 5);
    checker((phnum + 1) % 5);
    sem_post(&mutex);
}

void take(int phnum)
{
    sem_wait(&mutex);
    state[phnum] = 1;
    printf("Philosopher %d is hungry\n", phnum + 1);
    checker(phnum);
    sem_post(&mutex);
    sem_wait(&sem[phnum]);
}

void* dining_philosopher(void* num)
{
    int* i = num;
    take(*i);
    put_down(*i);
}
 
int main()
{
    pthread_t thread_id[5];

    sem_init(&mutex, 0, 1);

    for (int i = 0; i < 5; i++) sem_init(&sem[i], 0, 0); 
    for (int i = 0; i < 5; i++) {
        pthread_create(&thread_id[i], NULL, dining_philosopher, &phil[i]);
        printf("Philosopher %d is thinking\n", i + 1);
    }
    for (int i = 0; i < 5; i++) pthread_join(thread_id[i], NULL);

    return 0;
}