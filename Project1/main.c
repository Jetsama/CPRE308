#include <stdio.h>
#include "commands.h"
#include <unistd.h>
#include <string.h>

#define FALSE 0




exit_shell = FALSE;

int main(int argc, char *argv[]) { 

    command commands[5] = {
    pathcommand,
    exit_command,
    pid_command,
    pathcommand,
    pathcommand
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
    printf("Hello world\n");

    char buffer[100];
    while(exit_shell == FALSE){
        if(strcmp(prompt,"")){

            printf(prompt); //add prompt "308sh> "
            fgets(buffer, sizeof(buffer), stdin);

            //REMOVING \n
            size_t length = strlen(buffer);
            if (length > 0 && buffer[length - 1] == '\n') {
                buffer[length - 1] = '\0'; // Replace '\n' with '\0' 
            }

            printf("GOT INPUT\n");
            char delim[] = " ";
            cmd  = strtok(buffer, delim); //get command from string!
        }

       
        printf("command\n");
        printf(cmd);printf("\n");

        for( int i=0; i< 4; i++){
            printf("Compare %s and %s\n",commands[i].command,cmd);
            if(strcmp(commands[i].command,cmd) == 0){
                printf("MATCH\n"); //it is registered command

                //call command function
                commands[i].function();

                break; //no need to check other commands
            }
            
        }
        //works as an else statement where if not in valid commands then run with execvp
        //char* command = "ls";
        //char* argument_list[] = {"ls", "-l", NULL};
        //execvp(command, argument_list);
    }

    //printf("exit program");
    return 0;
}