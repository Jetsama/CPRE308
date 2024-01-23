
#include <stdio.h>
#include <pthread.h>
void* thread1hello(){
    //sleep(5);
    printf("Hello from thread1\n");

}
void* thread2hello(){
    sleep(5);
    printf("Hello from thread2\n");

}

void main(){

    pthread_t	i1; 
    pthread_t	i2; 
    pthread_create(&i1,NULL,(void*)&thread1hello, NULL); //create thread 1
    pthread_create(&i2,NULL,(void*)&thread2hello, NULL); //create thread 2
    pthread_join(i2, NULL); //join thread 2
    printf("Hello from main\n");
    pthread_join(i1, NULL); //join thread 1


    printf("Hello from main\n");
}