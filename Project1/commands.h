#ifndef COMMANDS_H
#define COMMANDS_H

extern int cmd_argc;
extern char *cmd_argv[];

typedef struct{
    char* command;
    int argnum; // dont know if needed really?
    void (* function)(int argc,char *cmd_argv[]);
} command;

void path_start();
#define FALSE 0
#define TRUE 1

extern int exit_shell;
extern char path[1024];

command cd_command;
command exit_command;
command pid_command;
command ppid_command;
command pwd_command;

#endif