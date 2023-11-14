#include "request.h"
#include <stdlib.h>
#include <string.h>

struct queue request_queue = {NULL, NULL, 0, PTHREAD_MUTEX_INITIALIZER};

int req_num = 0;
int add_count = 0;
void add_request(char* input, int request_id, struct timeval time){
    pthread_mutex_lock(&request_queue.queue_mutex);
    if(DEBUG){
        
        FILE* debug_file = fopen("debug", "a");
        //fprintf(debug_file,"Remove not null with req id = %d",removed_request->request_id);
        fprintf(debug_file,"LOCKED the queue to add\n");
        fclose(debug_file);
    }

    struct request* new_request;
    size_t length = strlen(input);
    while (length > 0 && (input[length - 1] == '\n' || input[length - 1] == '\r')) {
        input[--length] = '\0';
    }
    char* input_copy = strdup(input);
    int command = convert_string_command(input_copy);
    switch(command){
        case TRANS: 
            new_request = create_trans_request(input,request_id); 
            break;
        case CHECK: 
            new_request = create_check_request(input,request_id); 
            break;
        case ERROR:
            new_request = NULL;
            break;
    }
    if (new_request == NULL) {
        // Handle failed request creation
        //printf("FAIL\n");
            if(DEBUG){
        
        FILE* debug_file = fopen("debug", "a");
        //fprintf(debug_file,"Remove not null with req id = %d",removed_request->request_id);
        fprintf(debug_file,"FAILED in request unlock queue to add\n");
        fclose(debug_file);
        }

        pthread_mutex_unlock(&request_queue.queue_mutex);
        free(input_copy);
        return;
    }
    new_request->starttime = time; // set the start time

    //struct request* new_request = (struct request*)malloc(sizeof(struct request));
    
    //new_request->request_id = request_id;
    //new_request->
    req_num++;
        FILE* debug_file = fopen("debug", "a");
        //fprintf(debug_file,"Remove not null with req id = %d",removed_request->request_id);
        fprintf(debug_file,"ADD not null on add count %d total pending %d\n",add_count,req_num);
        //if (request_queue.head == NULL)fprintf(debug_file,"Head:%d Tail: %d \n",request_queue.head->request_id,request_queue.tail->request_id);
        add_count++;
        fclose(debug_file);
    if (request_queue.head == NULL) {
        // If the queue is empty, set both head and tail to the new_request
        request_queue.head = request_queue.tail = new_request;
        //printf("NEW");
    } else {
        // Otherwise, add the new_request to the tail of the queue
        request_queue.tail->next = new_request;
        request_queue.tail = new_request;

        //printf("TAIL");
    }

    request_queue.num_jobs++;
    if(request_queue.num_jobs > 1010 && DEBUG){
        printf("jobs exceeded 1010 %d \n",request_queue.num_jobs);
    }
    if(DEBUG){
        
        FILE* debug_file = fopen("debug", "a");
        //fprintf(debug_file,"Remove not null with req id = %d",removed_request->request_id);
        fprintf(debug_file,"CLOSE with success numjobs up to %d\n",request_queue.num_jobs);
        fclose(debug_file);
        }
    free(input_copy);
    //pthread_cond_signal(&request_queue.request_queue_active);
    pthread_mutex_unlock(&request_queue.queue_mutex);
}


void free_request(struct request* req) {
   // pthread_mutex_lock(&request_queue.queue_mutex);
    // Free the transactions array
    if(req->command == TRANS) free(req->transactions);
    // Free the request struct
    free(req);
    //pthread_mutex_unlock(&request_queue.queue_mutex);
}
int count = 0;



struct request* remove_request() {
    pthread_mutex_lock(&request_queue.queue_mutex);
    //if( request_queue.num_jobs < 1){
   //      pthread_cond_wait(&request_queue.request_queue_active, &request_queue.queue_mutex);
   // }

    if(DEBUG) {
        FILE* debug_file = fopen("debug", "a");
        fprintf(debug_file, "LOCKED the queue\n");
        fclose(debug_file);
    }

    if (request_queue.tail == NULL || request_queue.head == NULL) {
        // Queue is empty
        // Unlock the queue
        request_queue.num_jobs = 0 ;

        if(DEBUG) {
            FILE* debug_file = fopen("debug", "a");
            fprintf(debug_file, "Tail or head empty unlock the queue\n");
            fclose(debug_file);
        }
        pthread_mutex_unlock(&request_queue.queue_mutex);
        return NULL;
    }

    struct request* removed_request = request_queue.head;
    request_queue.num_jobs--;

    if(DEBUG) {
    FILE* debug_file = fopen("debug", "a");
    fprintf(debug_file, "Remove not null on count %d total pending %d\n", count, req_num);
    count++;
    fclose(debug_file);

    debug_file = fopen("debug2", "a");

    if (request_queue.head != NULL && request_queue.tail != NULL) {
        if (request_queue.head->request_id != 0 && request_queue.tail->request_id != 0) {
            fprintf(debug_file, "Head:%d Tail: %d \n", request_queue.head->request_id, request_queue.tail->request_id);
            fprintf(debug_file, "Removed request: %p, request_id: %d\n", (void*)removed_request, removed_request->request_id);
        } else {
            fprintf(debug_file, "Head or Tail has an invalid request_id\n");
            fclose(debug_file);

            // Unlock the queue before returning
            pthread_mutex_unlock(&request_queue.queue_mutex);
            return NULL;
        }
    } else {
        fprintf(debug_file, "Head or Tail is NULL\n");
        fclose(debug_file);

        // Unlock the queue before returning
        pthread_mutex_unlock(&request_queue.queue_mutex);
        return NULL;
    }

    fclose(debug_file);
    }


    request_queue.head = removed_request->next;

    if (request_queue.head == NULL) {
        // The queue is now empty
        request_queue.head = NULL;
        request_queue.tail = NULL;
    }

    if(DEBUG) {
        FILE* debug_file = fopen("debug", "a");
        fprintf(debug_file, "SUCCESS at removal no unlock queue\n");
        fclose(debug_file);
    }
    if( request_queue.num_jobs < 1) pthread_cond_signal(&request_queue.request_queue_notactive);

    // Unlock the queue
    pthread_mutex_unlock(&request_queue.queue_mutex);

    return removed_request;
}








int convert_string_command(char* input){

    int command = ERROR;
    char* input_copy = strdup(input);
    char* command_string = strtok(input, " ");
    if (strcmp(command_string, "CHECK") == 0) {
        command = CHECK;
    } else if (strcmp(command_string, "TRANS") == 0) {
        command = TRANS;
    }
    else{
        command = ERROR;
    }
    free(input_copy);

    return command;
}

struct request* create_trans_request(char* input, int request_id){

    int account_count = 0;
    int accounts[10];
    int amounts[10];
    char input_copy[MAX_INPUT_SIZE];  // Assuming MAX_INPUT_SIZE is the maximum size of your input
    strcpy(input_copy, input);

    strtok(input_copy, " ");
    
    char* token = strtok(NULL, " ");
    while (token != NULL) {
        //printf("token is %s and input copy is  %s",token,input_copy);
        accounts[account_count] = atoi(token);
        amounts[account_count] = atoi(strtok(NULL, " "));
        account_count++;
        token = strtok(NULL, " ");
    }

    struct request* new_request = (struct request*)malloc(sizeof(struct request));

    new_request->transactions = (struct trans*)malloc(account_count * sizeof(struct trans));

    // Populate the transactions array
    for (int i = 0; i < account_count; i++) {
        new_request->transactions[i].acc_id = accounts[i];
        new_request->transactions[i].amount = amounts[i];
    }
    
    new_request->request_id = request_id;
    new_request->num_trans =  account_count;
    new_request->command = TRANS;
    new_request->next = NULL;
    return new_request;
}

struct request* create_check_request(char* input, int request_id) {
    struct request* new_request = (struct request*)malloc(sizeof(struct request));
    if (new_request == NULL) {
        printf("ALLOCATION ERROR");
        // Handle memory allocation failure
        return NULL;
    }

    new_request->request_id = request_id;
    char* acc_id_str = strtok(NULL, " ");
    if (acc_id_str != NULL) {
        new_request->check_acc_id = atoi(acc_id_str);
    } else {
        new_request->check_acc_id = atoi(input);
        // Handle missing account ID
        printf("MISSING ACCOUNT ID");
        free(new_request);
        return NULL;
    }
    new_request->command = CHECK;
    new_request->num_trans = 0;
    new_request->next = NULL;
    return new_request;
}
