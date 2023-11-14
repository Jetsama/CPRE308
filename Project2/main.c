
#include "Bank.h"
#include <stdio.h>
#include "pthread.h"
#include "thread.h"
#include <stdlib.h>

int terminate_flag = 0;
extern struct queue request_queue;
extern char * filename;
int main(int argc, char *argv[]) {

    if(argc != 4){
        printf("INCORRECT NUMBER OF ARGUMENTS GOT %d INSTEAD OF 3\n",argc-1);
        
        return 1;
    }

    printf("INSTRUCTIONS\n");
    int workers_num = atoi(argv[1]);//number of workers
    printf("Create %d workers\n",workers_num);
    
    int account_num = atoi(argv[2]);
    printf("Create %d accounts\n",account_num);
    
    filename = argv[3];
    printf("Create ouput at %s\n",filename);
    
    //create accounts
    initialize_accounts(account_num);
    //create account mutex locks
    accounts_mutex = (pthread_mutex_t*)malloc(account_num * sizeof(pthread_mutex_t));
    for (int i = 0; i < account_num; ++i) {
        pthread_mutex_init(&accounts_mutex[i], NULL);
    }

    //create worker threads
    struct thread_data threads[workers_num];

    for(int i = 0; i < workers_num; i++){
        threads[i].simpleID = i;
        threads[i].status = 0;
        pthread_create(&threads[i].threadID, NULL, worker_thread,(void*)&threads[i]);
        printf("Created thread number %d with %ld id, status is currently %d\n",i,threads[i].threadID,threads[i].status);
    }
    




    //MAIN INPUT LOOP
    char* input = (char*)malloc(MAX_INPUT_SIZE);
    int requestID = 0;
output_file = fopen(filename, "w");
    if (output_file == NULL) {
        perror("Error opening output file");
        return 1;
    }
    if(DEBUG)fclose(output_file);


    while( fgets(input, MAX_INPUT_SIZE, stdin) != NULL && strcmp(input, "END\n") != 0) {
        requestID++;
        //printf("adding request");
        struct timeval time;
        gettimeofday(&time, NULL);
        char* input_copy = strdup(input);
        add_request(input_copy,requestID,time);
        free(input_copy);
        //printf("added request");
        //int command = convert_string_command(input);
        

        //asign the job to thread
        /*for (int j = 0; j < workers_num; ++j) { //TODO wait if all threads busy
            if (threads[j].status == 0) {
                printf("Assigning job to thread %d or %lu\n",threads[j].simpleID, threads[j].threadID);
                //GIVE INPUT DATA TO THREAD
                strcpy(threads[j].command,input);
                threads[j].status = requestID; // Set status to active (1) to assign the job
                break;
            }
        }*/
        //scanf("%s", input);
    }
    //wait for the tasks to finish
    terminate_flag = 1;
    //pthread_cond_wait(&request_queue.request_queue_notactive, &request_queue.queue_mutex);
    for (int i = 0; i < workers_num; ++i) {
        pthread_join(threads[i].threadID, NULL);
    }
    //end program cleanup
    if(!DEBUG)fclose(output_file);
    free(input);
    free_accounts();
    free(accounts_mutex);
}