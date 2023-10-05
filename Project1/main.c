#include <stdio.h>
#include "commands/commands.h"
#define FALSE 0;






int main(int argc, char *argv[]) { 

    static command commands[1];
    int exit_shell = FALSE; //set true to break
    char* prompt;
    prompt = "308sh> ";
    for(int i= 0; i< argc; i++){
        if(argv[i] == "-p"){
            i++; //next arg is prompt
            prompt = argv[i];
        }

    }

    while(!exit_shell){

        for( int i=0; i< sizeof(commands[i]) / sizeof(commands[0]); i++){

            continue;
        }
        execvp(prompt);
    }

    return 0;
}