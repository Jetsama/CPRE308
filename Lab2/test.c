
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>

int main(){
    printf("lifes short\n");
    int child = fork();
    sleep(5);
    if (child == 0) {
        printf("see?");
        kill(getpid(), SIGTERM);
        
    }
    return 0;
}