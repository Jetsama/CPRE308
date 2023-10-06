#include "commands.h"
#include <stdio.h>

char*  path = "";

void path_start(){

}

void path_cd(){


}

void path_pwd(){


}

void close(){
    printf("Leaving shell! Smell yah later!\n");
    exit_shell = TRUE;
}

command pathcommand ={
    "ls",
     1, // dont know if needed really?
    path_start
};

command exit_command ={
    "exit",
     0, // dont know if needed really?
    close
};


command cd_command={
    "cd",
     0, // dont know if needed really?
    path_cd
};
command pwd_command={
    "pwd",
     0, // dont know if needed really?
    close
};