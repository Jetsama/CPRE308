#include "commands.h"
#include <stdio.h>

#ifdef _WIN32
    // Windows-specific headers
    #include <windows.h>
#else
    // Unix-specific headers
    #include <unistd.h>
#endif


void DirectoryUp() {
    char *lastBackslash = strrchr(path, '\\');
    if (lastBackslash != NULL) {
        *lastBackslash = '\0';
    }
}

char path[1024];
int cmd_argc;
char *cmd_argv[];
void path_start(){
    #ifdef _WIN32
        // Windows code
        DWORD length = GetModuleFileName(NULL, path, sizeof(path));
        if (length != 0) {
            printf("Path of the executable: %s\n", path);
        } else {
            perror("Error getting executable path");
        }
    #else
        // Unix code
        ssize_t len = readlink("/proc/self/exe", path, sizeof(path) - 1);
        if (len != -1) {
            path[len] = '\0';
            printf("Path of the executable: %s\n", path);
        } else {
            perror("Error getting executable path");
        }
    #endif
    
}

void path_cd(int argc, char *argv[]){
    //printf("%s\n", path);
    //DirectoryUp();
    char *dir = argv[1];
    //printf("dir is %s", dir);
    #ifdef _WIN32
        if (SetCurrentDirectory(dir)) {
            printf("Changed directory to: %s\n", dir);
        } else {
            printf("Error changing directory");
        }
    #else
        if (chdir(dir) == 0) {
            printf("Changed directory to: %s\n", dir);
        } else {
            printf("Error changing directory");
        }
    #endif
}

void path_pwd(){
    char cwd[MAX_PATH];
   if (getcwd(cwd, sizeof(cwd)) != NULL) {
       printf("Current working dir: %s\n", cwd);
   } else {
       perror("Error getting path");
   }
}
void close_shell(){
    printf("Leaving shell! Smell yah later!\n");
    exit_shell = TRUE;
}

command exit_command ={
    "exit",
     0, // dont know if needed really?
    close_shell
};

command cd_command={
    "cd",
     0, // dont know if needed really?
    path_cd
};
command pwd_command={
    "pwd",
     0, // dont know if needed really?
    path_pwd
};




