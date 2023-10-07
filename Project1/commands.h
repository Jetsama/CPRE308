#ifndef COMMANDS_H
#define COMMANDS_H


#ifdef _WIN32
    // Windows-specific headers
    #include <process.h>
#else
    // Unix-specific headers
    #include <unistd.h>
#endif

extern int cmd_argc;
extern char *cmd_argv[];

typedef struct{
    char* command;
    int argnum; // dont know if needed really?
    void (* function)(int argc,char *cmd_argv[]);
} command;


typedef struct BackgroundProcess {
    pid_t pid;
    char* command;
    int completed; // Flag to indicate if the process has completed
    struct BackgroundProcess* next;
} BackgroundProcess;
extern BackgroundProcess *processes;

int checkBackgroundProcessStatus(BackgroundProcess** processes);
BackgroundProcess* addBackgroundProcess(BackgroundProcess** processes, pid_t pid, char* cmd);

void DirectoryUp();
void path_start();
#define FALSE 0
#define TRUE 1

extern int exit_shell;
extern char path[1024];

extern command cd_command;
extern command exit_command;
extern command pid_command;
extern command ppid_command;
extern command pwd_command;
extern command jobs_command;
#endif