#ifndef COMMANDS_H
#define COMMANDS_H

int sharedVariable;

typedef struct{
    char* command;
    int argnum; // dont know if needed really?
    void (* function)();
} command;


#define FALSE 0
#define TRUE 1

extern int exit_shell;

command pathcommand;
command exit_command;
command pid_command;
command ppid_command;
command cd_command;
command pwd_command;

#endif