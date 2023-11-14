
#include <sys/time.h>
#include "pthread.h"
#include <stdio.h>
#define DEBUG 1

#define MAX_INPUT_SIZE 1000 //for input buffer

struct trans { // structure for a transaction pair
    int acc_id; // account ID
    int amount; // amount to be added, could be positive or negative
};

struct request {
    int command;
    struct request * next; // pointer to the next request in the list
    int request_id; // request ID assigned by the main thread
    int check_acc_id; // account ID for a CHECK request
    struct trans * transactions; // array of transaction data
    int num_trans; // number of accounts in this transaction
    struct timeval starttime, endtime; // starttime and endtime for TIME
};

struct queue {
    struct request * head, * tail; // head and tail of the list
    int num_jobs; // number of jobs currently in queue
    pthread_mutex_t queue_mutex;
    pthread_mutex_t head_tail_mutex;
    pthread_cond_t request_queue_active;
    pthread_cond_t request_queue_notactive;
};

extern int req_flag;
extern int terminate_flag;
extern struct queue request_queue;
extern char * filename;
enum Command{
    CHECK,
    TRANS,
    ERROR
};

void add_request(char* input,int request_id, struct timeval time);
void free_request(struct request* req);
struct request* remove_request();
int convert_string_command(char* command_string);

struct request* create_trans_request(char* input, int request_id);
struct request* create_check_request(char* input, int request_id);