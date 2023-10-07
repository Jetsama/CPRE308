#include <stdio.h>
#include <stdlib.h>
#include "commands.h"
#include "createlog.h"
#include <sys/time.h>
#ifdef _WIN32
    // Windows-specific headers
    #include <process.h>
#else
    // Unix-specific headers
    #include <unistd.h>
#endif
#include <string.h>

#define FALSE 0


char** splitargs(char* input) {
    char **args = malloc(100 * sizeof(char*));
    char *p = strtok(input, " ");
    int i = 0;
    while (p != NULL && i < 100) {
        args[i] = malloc(strlen(p) + 1);

        strcpy(args[i], p);
        p = strtok(NULL, " ");
        i++;
    }

    // Make sure to terminate the args array with NULL
    args[i] = NULL;

    return args;
}

int getSize(char **args) {
    int size = 0;
    while (args[size] != NULL) {
        //printf("%d is %s\n", size, args[size]);
        size++;
    }
    return size;
}

exit_shell = FALSE;
BackgroundProcess *processes = NULL;
int main(int argc, char *argv[]) {

    path_start(); //get path of exe
    DirectoryUp();


    command commands[6] = {
    cd_command,
    exit_command,
    pid_command,
    ppid_command,
    pwd_command,
    jobs_command
    };


    char* prompt;
    char* cmd;
    cmd = "";
    prompt = "308sh> ";
    for(int i= 0; i< argc; i++){
        if(strcmp(argv[i], "-p") == 0 && i + 1 < argc){
           // printf("ag1 %s and ag2%s \n", argv[i], argv[i + 1]);
           prompt = argv[i + 1];
        }

    }
    printf("Hello world I'm the CPRE 308 shell!\n");

    char buffer[100];
    char buffer_string[100];
    int command_flag = 0;
    fd_set read_fds;
    struct timeval timeout;
    int stdin_fd = fileno(stdin);
    printf("%s",prompt); //add prompt "308sh> "
 fflush(stdout);
    while(exit_shell == FALSE){

        if(processes){

        }
        if(checkBackgroundProcessStatus(&processes)){ 
            printf("%s",prompt);
            fflush(stdout);
        };
        FD_ZERO(&read_fds);
        FD_SET(stdin_fd, &read_fds);
        timeout.tv_sec = 0;
        timeout.tv_usec = 0;

        int ready = select(stdin_fd + 1, &read_fds, NULL, NULL, &timeout);
        if (ready == -1) {
            perror("select");
            exit(EXIT_FAILURE);
        } else if (ready > 0 && FD_ISSET(stdin_fd, &read_fds)) {


        
        command_flag = 0; //command has not been processed


        
        fgets(buffer, sizeof(buffer), stdin);

            //REMOVING \n
            size_t length = strlen(buffer);
            if (length > 0 && buffer[length - 1] == '\n') {
                buffer[length - 1] = '\0'; // Replace '\n' with '\0' 
            }

            //printf("GOT INPUT\n");
            char delim[] = " ";
            strcpy(buffer_string,buffer);
            cmd  = strtok(buffer, delim); //get command from string!

       
        //printf("command\n");
        //printf(cmd);printf("\n");
        char ** arguments = splitargs(buffer_string);   //set args
        int count = getSize(arguments);

        for( int i=0; i< 6; i++){
            //printf("Compare %s and %s\n",commands[i].command,cmd);
            if(strcmp(commands[i].command,cmd) == 0){
                //printf("MATCH\n"); //it is registered command        
                //call command function                

                commands[i].function(count,arguments);
                command_flag = 1;
                printf("%s",prompt); 
                fflush(stdout);
                break; //no need to check other commands
            }
            
        }
        if(command_flag) continue;
        printf("Run external command\n\n");
        //works as an else statement where if not in valid commands then run with execvp
        //char* command = "ls";
        //char* argument_list[] = {"ls", "-l", NULL};

        int run_background = count >1 && strcmp(arguments[count-1],"&") == 0;
        //printf("run in background: %d\n",run_background);
        if(run_background){
            
        }



            char cmd_copy[100]; 
            strcpy(cmd_copy, cmd);

            char **arguments_copy = malloc((count + 1) * sizeof(char*)); // Allocate memory for arguments_copy

            for (int i = 0; i < count - 1; ++i) {
                arguments_copy[i] = malloc(strlen(arguments[i]) + 1);
                strcpy(arguments_copy[i], arguments[i]);
            }

            arguments_copy[count - 1] = NULL; 
        int logFile = 0;
        pid_t pid = fork();
        if(pid ==-1){
            perror("fork failed");
        }
       
        else if (pid == 0) { // child
        if (run_background) {
        char *logFilename = createLog(); // Get log filename
        logFile = openLog(logFilename);
        if (execvp(cmd_copy, arguments_copy) == -1) {
            perror("execvp failed");
            closeLog(logFile);
            exit(0); // Exit with failure status
        }
        exit(0); // Exit with success status
        } else {
            if (execvp(cmd, arguments) == -1) {
                perror("execvp failed");
                exit(0); // Exit with failure status
            }
            exit(0); // Exit with success status
        }
    }

        else {
            if (run_background) {
                // Parent process for background command
                // Create a background process entry with the process ID and command
                processes = addBackgroundProcess(&processes, pid, cmd);
                printf("%ld processes updated", processes->pid);
                printf("[%ld] %s\n", pid, cmd);
            } else {
            // Parent process for foreground command
            waitpid(pid, NULL, 0); // Wait for the foreground process to complete
        }
        }
        printf("%s",prompt); //add prompt "308sh> 
        fflush(stdout);
        }
    } //end while

    //printf("exit program");
    return 0;
}