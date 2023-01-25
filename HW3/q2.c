//99521361
#include <stdio.h>
#include <pthread.h>
#include <stdbool.h>

#define max_size 100000007

typedef struct {int thread_id;} thread_str;

int pow_num = 0;
int n = 0;
int arr[max_size];
int pointer = 0;
bool flag = false;
pthread_mutex_t lock;

int power(){
    int pow = 1;
    for(int i = 0; i < pow_num; i++) pow *= arr[i];
    return pow;
}

void *calculator(void *_arg)
{
    thread_str *arg = (thread_str *)_arg;
    pthread_mutex_lock(&lock);
    arr[pointer++] = n;
    pthread_mutex_unlock(&lock);

    while (pointer != pow_num){}

    pthread_mutex_lock(&lock);

    if(flag);
    else printf("pow is %d", power());  

    flag = true;
    pthread_mutex_unlock(&lock);

    pthread_exit(NULL);
}

int main()
{
    printf("enter the number you want: \n");
    scanf("%d", &n);
    printf("enter the power you want: \n");
    scanf("%d", &pow_num);

    pthread_mutex_init(&lock, NULL);
    thread_str args[pow_num];
    pthread_t threads[pow_num];
  
    for(int i = 0; i < pow_num; i++) args[i].thread_id = i;
    for(int i = 0; i < pow_num; i++) pthread_create(&threads[i], NULL, calculator, (void *)&args[i]);
    for(int i = 0; i < pow_num; i++) pthread_join(threads[i], NULL);

    pthread_mutex_destroy(&lock);
    return 0;
}