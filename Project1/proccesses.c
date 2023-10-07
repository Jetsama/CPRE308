
#include "commands.h"
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
void cmd_pid(){

    pid_t pid = getpid();
    printf("PID: %d\n", pid);
}
void cmd_ppid(){

    //pid_t ppid = getppid();
    //printf("Parent PID: %d\n", ppid);
}

command pid_command ={
    "pid",
     0, // dont know if needed really?
    cmd_pid
};
command ppid_command={
    "ppid",
     0, // dont know if needed really?
    cmd_ppid
};


void jobs_function(){

    BackgroundProcess* current = processes;

    printf("Background Processes:\n");
    while (current != NULL) {
        printf("[%d] %s\n", current->pid, current->command);
        current = current->next;
    }
}
command jobs_command={
    "jobs",
     0, // dont know if needed really?
    jobs_function
};


BackgroundProcess* addBackgroundProcess(BackgroundProcess** processes, pid_t pid, char* cmd) {
    BackgroundProcess* newProcess = malloc(sizeof(BackgroundProcess));
    if (newProcess == NULL) {
        perror("malloc failed");
        exit(0);
    }
    newProcess->pid = pid;
    newProcess->command = strdup(cmd);
    newProcess->completed = FALSE;
    newProcess->next = NULL;

    if (*processes == NULL) {
        *processes = newProcess; // Update the head pointer if the list is empty
    } else {
        BackgroundProcess* current = *processes;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = newProcess;
    }
    return *processes; // Return the updated list (optional)
}

void freeBackgroundProcesses(BackgroundProcess* processes) {
    while (processes != NULL) {
        BackgroundProcess* temp = processes;
        processes = processes->next;
        free(temp->command);
        free(temp);
    }
}

int checkBackgroundProcessStatus(BackgroundProcess** processes) {
    //printf("Processes pointer: %p\n", (void *)processes);
    BackgroundProcess* current = *processes;
    BackgroundProcess* prev = NULL;
    //
    while (current != NULL) {
        //printf("Checking process [%d] %s \n", current->pid, current->command);
        int status;
        pid_t result = waitpid(current->pid, &status, WNOHANG);
        if (result > 0) {
            // Process has completed, update the completed flag
            current->completed = TRUE;
            printf("\n[%d] %s finished with status: %d\n", current->pid, current->command, WEXITSTATUS(status));

            // Remove the completed process from the list
            if (prev == NULL) {
                // If it's the first element, update the head of the list
                *processes = current->next;
                free(current->command);
                free(current);
                current = *processes;  // Move to the next process in the list
            } else {
                prev->next = current->next;
                free(current->command);
                free(current);
                current = prev->next;  // Move to the next process in the list
            }
            return 1; // it ended process
        } else {
            // Move to the next process in the list
            prev = current;
            current = current->next;
        }
    }
}