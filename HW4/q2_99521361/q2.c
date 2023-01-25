//99521361
#define _GNU_SOURCE
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <stdlib.h>
#include <sys/uio.h>
#include <limits.h>
#include <assert.h>
#include <errno.h>

#define GIGA 1000000000
#define GIBI 1073741824

char *produce_data(size_t size);
void consumer_func(int fd[2]);

int main(int argc, char const *argv[])
{
    int fd[2];
    pipe(fd);

    pid_t consumer = fork();
    if (consumer == 0)
    {
        close(fd[1]);

        consumer_func(fd);

        return 0;
    }
    char *data = produce_data((size_t)3 * GIBI);

    int n = write(fd[1], &consumer, sizeof(pid_t));
    assert(n != -1);
    n = write(fd[1], &data, sizeof(char *));
    assert(n != -1);
    wait(NULL);

    close(fd[0]);
    close(fd[1]);

    free(data);

    return 0;
}

void consumer_func(int fd[2])
{
    pid_t producer = getppid();                                                                                 
    char *remote_data;                                          
    char *local_data = malloc(sizeof(char) * (unsigned long)3 * GIBI); 
    unsigned long batch_size = ((unsigned long)3 * GIBI) / IOV_MAX;           

    unsigned long n = read(fd[0], &remote_data, sizeof(char *));
    close(fd[0]);
    assert(n != -1);

    struct iovec local[IOV_MAX];

    for (int i = 0; i < IOV_MAX; i++)
    {
        local[i].iov_base = &local_data[i * batch_size];
        local[i].iov_len = sizeof(char) * batch_size;
    }

    n = 0;
    int offset = IOV_MAX / 8; 

    for (int i = offset - 1; i < IOV_MAX; i += offset)
    {
        int diff = i - offset >= 0 ? i - offset : 0;
        n += process_vm_readv(producer, &local[diff], offset, &local[diff], offset, 0);
        assert(n != -1 && "you are probably not running with sudo thus failing.");

        printf("read %ld bytes\n", n);
    }

    free(remote_data);
}

char *produce_data(unsigned long size)
{
    char *data = malloc(sizeof(char) * size);
    unsigned long n = size / 8;
    printf("%ld\n", n);
    char str[] = {'D', 'A', 'M', 'N', '.', 'S', 'O', 'N'};
    for (unsigned long i = 0; i < 8; i++)
    {
        memset(data + sizeof(char) * n * i, str[i], sizeof(char) * n);
    }

    return data;
}