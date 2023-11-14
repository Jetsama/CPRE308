
#include "pthread.h"
#include <string.h>
#include "request.h"


struct thread_data{
    pthread_t threadID;
    int simpleID;
    int status;
};

extern pthread_mutex_t* accounts_mutex;
extern FILE* output_file;
extern pthread_mutex_t output_file_mutex;




void* worker_thread(void* arg);
void execute_command(struct request * req);