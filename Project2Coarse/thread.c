#include "pthread.h"
#include "Bank.h"
#include <stdio.h>
#include "thread.h"
#include <sys/time.h>


pthread_mutex_t* accounts_mutex;
pthread_mutex_t accounts_locks_mutex;
FILE* output_file;
pthread_mutex_t output_file_mutex;
pthread_mutex_t check_queue_mutex;
char * filename;

void* worker_thread(void* arg){
    struct thread_data* data = (struct thread_data*)arg;

    while (!terminate_flag|| request_queue.num_jobs) {
        // Check if the thread is assigned a job (status = 1)
        struct request *request = remove_request();

        if (request != NULL) {
            data->status = request->request_id;
            // Perform the assigned task
            if(DEBUG) printf("Thread %d or %lu is working...\n",data->simpleID, data->threadID);

            execute_command(request);

            // Simulate work with a sleep (adjust as needed)
            //usleep(1000000); // Sleep for 1 second
            if(DEBUG) printf("Thread %d or %lu is finished task %d!\n", data->simpleID, data->threadID,request->request_id);
            // Set status back to 0 to indicate completion
            data->status = 0;
            free_request(request);
        }
    }
    if(DEBUG) printf("Thread %d or %lu is dies!\n", data->simpleID, data->threadID);
    pthread_exit(NULL);
}



void execute_command(struct request * req){
    int requestID = req->request_id;
    struct timeval time;        

    
    char output[512];
    if(req->command == CHECK){ //CHECK
            int id = req->check_acc_id;
            if(id == 0){
                printf("INVALID CHECKING ATTEMPT\n");
                return;
            }
            pthread_mutex_lock(&accounts_mutex[id - 1]);
            int balance = read_account(id);
           

            snprintf(output, sizeof(output),"%d BAL %d",requestID,balance);
            gettimeofday(&time, NULL);
            pthread_mutex_unlock(&accounts_mutex[id - 1]);

    }
    else if(req->command == TRANS){ //TRAN
        int success = 1;
        int acctid;
        int accounts[10];
        int amounts[10];
        pthread_mutex_lock(&accounts_locks_mutex);
            for(int i = 0; i< req->num_trans; i++){ //check if valid
                acctid = req->transactions[i].acc_id;
                int current = read_account(acctid);
                int amount = req->transactions[i].amount;
                amounts[i] = (current+amount);
                accounts[i] = acctid;
                success = (current+amount)>=0;
                if(!success) break;
            }
        

            if(success){
                for(int i = 0; i< req->num_trans; i++){ //set values
                    int current = read_account(accounts[i]);
                    int amount = req->transactions[i].amount;
                    write_account(accounts[i], (current+amount));
                }
                snprintf(output, sizeof(output), "%d OK",requestID);
            }
            else snprintf(output, sizeof(output), "%d ISF %d",requestID,acctid);
            gettimeofday(&time, NULL);
        pthread_mutex_unlock(&accounts_locks_mutex);

    }
    
    pthread_mutex_lock(&output_file_mutex);
    if(DEBUG){
        output_file = fopen(filename, "a");
        if (output_file == NULL) {
            perror("Error opening output file");
            return 1;
        }
    }
    fprintf(output_file,output);
    fprintf(output_file," TIME %ld.%06ld %ld.%06ld\n",req->starttime.tv_sec, req->starttime.tv_usec, time.tv_sec, time.tv_usec);
    if(DEBUG)(output_file);
    pthread_mutex_unlock(&output_file_mutex);

}

