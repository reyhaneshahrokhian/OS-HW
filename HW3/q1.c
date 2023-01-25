#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <math.h>

double sum = 0.0;
int count = 0;
int n = 0;
int t = 0;
pthread_mutex_t lock;

void* calculator()
{
    pthread_mutex_lock(&lock);
        
    for (int i = count; i < n; i += t)
    {
        if (i == 0) sum += 1;           
        else{
            double x = i % 2 != 0 ? -1.0 : 1.0;               
            double y = 2.0 * i + 1;

            sum += x / y;
        }
    }
    count++;
        
    pthread_mutex_unlock(&lock);

    return NULL;
}

int main(void)
{
    printf("enter number of sequences : \n");
    scanf("%d", &n);
    
    printf("enter number of threads : \n");
    scanf("%d", &t);

    pthread_t tid[t];

    pthread_mutex_init(&lock, NULL);

    for(int i = 0; i < t; i++) pthread_create(&(tid[i]),NULL, &calculator, NULL);  
    for(int i = 0; i < t; i++) pthread_join(tid[i], NULL);

    pthread_mutex_destroy(&lock);
    printf("pi is equal to : %f",sum*4.0);

    return 0;
}