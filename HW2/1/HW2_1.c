#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

int array[1000000000];
void merge(int start ,int second_start , int end){
    int i;
    int first = second_start - start;
    int second = end- second_start + 1 ;
    int merged[end - start + 1];
    int t;
    i = 0;
    int j = 0;
    for(t = 0; t <= end; t++){
        if(i < first && j < second){
            if(array[i] < array[second_start + j])
                merged[t] = array[i++];
            else
                merged[t] = array[second_start + j++];
        }
        else if(i == first){
            for (int k = j ; k < second ; k++){
                merged[t] = array[second_start + k];
                t++;
            }
            break;
        }   
        else{
            for (int k = i ; k < first ; k ++) {
                merged[t] = array[k];
                t++;
            }
            break;
        }  
    } 
    for (int i = 0 ; i < second + first ; i++){
        array[i] = merged[i];
    }
}

int main(int argc, char *argv[])
{
    int n, m, i, j, count = 0;
    int x = 0, temp;
    scanf("%d", &n);
    scanf("%d", &m);
    //int array[n];
    do{
        scanf("%d", &array[count++]);
    }while(getchar() != '\n' && count < n);

    pid_t parent = getpid();
    for(i = 0; i < m - 1; i++){
        vfork();
    }
    for(i = 0;i < (n / m) - 1; i++){
        for(j = i + 1; j< (n /m); j++){
            if(array[j + x] < array[i + x]){
                temp = array[i+x];
                array[i+x] = array[j+x];
                array[j+x] = temp;
            }
        }
    }
    x += (n / m);
    if(parent != getpid()){
        exit(0);
    }
    for(i = 1; i < m; i++){
        merge(0 ,i * n/m, (i + 1) * n/m - 1 );
    }
    for(i = 0; i < n; i++){
        printf("%d ", array[i]);
    }
    exit(0);
}

