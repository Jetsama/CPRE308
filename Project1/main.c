#include <stdio.h>
#include "commands.h"
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

int main(int argc, char *argv[]) {

    path_start(); //get path of exe

    command commands[5] = {
    cd_command,
    exit_command,
    pid_command,
    ppid_command,
    pwd_command
    };


    char* prompt;
    char* cmd;
    cmd = "";
    prompt = "308sh> ";
    for(int i= 0; i< argc; i++){
        if(argv[i] == "-p"){
            i++; //next arg is prompt
            prompt = argv[i];
        }

    }
    printf("Hello world I'm the CPRE 308 shell!\n");

    char buffer[100];
    char buffer_string[100];
    int command_flag = 0;

    while(exit_shell == FALSE){
        command_flag = 0; //command has not been processed

        if(strcmp(prompt,"")){

            printf(prompt); //add prompt "308sh> "
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
        }

       
        printf("command\n");
        printf(cmd);printf("\n");
        char ** arguments = splitargs(buffer_string);   //set args
        int count = getSize(arguments);

        for( int i=0; i< 5; i++){
            printf("Compare %s and %s\n",commands[i].command,cmd);
            if(strcmp(commands[i].command,cmd) == 0){
                //printf("MATCH\n"); //it is registered command        
                //call command function                

                commands[i].function(count,arguments);
                command_flag = 1;
                break; //no need to check other commands
            }
            
        }
        if(command_flag) continue;
        printf("run external command");
        //works as an else statement where if not in valid commands then run with execvp
        //char* command = "ls";
        //char* argument_list[] = {"ls", "-l", NULL};
        pid_t pid = fork();
        if(pid ==-1){
            perror("fork failed");
        }
        else if (pid == 0) {//child
            if (execvp(cmd, arguments) == -1) {
                 perror("execvp failed");
            }
            exit_shell = TRUE;
        } else {
            waitpid(pid, NULL, 0); //add & check
            printf("external command finished");
        }
        
    }

    //printf("exit program");
    return 0;
}