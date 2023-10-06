
#include "commands.h"
#include <stdio.h>
#include <unistd.h>

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