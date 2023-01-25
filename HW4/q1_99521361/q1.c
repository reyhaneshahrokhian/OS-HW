#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <assert.h>
#include <sys/wait.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>

typedef struct prime
{
    int value;
    int counter;
} prime_t;

prime_t **find_primes(int n)
{
    int i = 3;
    int count = 1;
    prime_t **primes = malloc(sizeof(prime_t *) * n);
    primes[count - 1] = malloc(sizeof(prime_t));
    primes[count - 1]->value = 2;
    primes[count - 1]->counter = 2;

    while (count != n)
    {
        bool flag = true;
        for (int j = 2; j * j <= i; j++){
            if (i % j == 0){
                flag = false;
                break;;
            }
        } 
        if (flag)
        {
            primes[count] = malloc(sizeof(prime_t));
            primes[count]->value = i;
            primes[count]->counter = i;
            count++;
        }
        i += 2;
    }

    return primes;
}
int main()
{
    int parent_fd[2];
    int fd[16];
    pid_t child[8];

    pipe(parent_fd);

    for (int i = 0; i < 8; i++) pipe(&fd[2 * i]);
    
    for (int i = 0; i < 8; i++)
    {
        child[i] = fork();
        assert(child[i] >= 0 && "failed in forking new processes!");
        if (child[i] == 0)
        {
            close(parent_fd[0]);
            prime_t p;
            while (1)
            {
                read(fd[2 * i], &p, sizeof(prime_t));
                p.value += i;
                p.counter--;
                if (p.counter == 0) write(parent_fd[1], &p, sizeof(prime_t));
                else write(fd[2 * ((i + 1) % 8) + 1], &p, sizeof(prime_t));
            }
            return 0;
        }
    }
    prime_t **primes = find_primes(100);
    close(parent_fd[1]);
    for (int i = 0; i < 100; i++) write(fd[2 * (primes[i]->value % 8) + 1], primes[i], sizeof(prime_t));
    
    prime_t p;
    for (int i = 0; i < 100; i++)
    {
        read(parent_fd[0], &p, sizeof(prime_t));
        printf("Done %d:%d\n", p.value, p.counter);
    }

    for (int i = 0; i < 8; i++) kill(child[i], SIGINT);
    for (int i = 0; i < 100; i++) free(primes[i]);
    
    free(primes);

    return 0;
}