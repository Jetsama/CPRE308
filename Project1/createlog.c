#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include <time.h>
#include "commands.h"

char* createLog(){
        int original_stdout = dup(STDOUT_FILENO);

        int logNumber = 1;
        //char logFilename[256]; // Adjust the size based on your naming convention
        char* logFilename = malloc(strlen(path) + 20); // Adjust the size as needed
        struct stat fileStat;
        mkdir("logs", 0700);
        while (1) {
             
            sprintf(logFilename, "%s/logs/output%d.log", path, logNumber);
            if (stat(logFilename, &fileStat) == -1) {
                // The file does not exist, use this filename
                break;
            }
            logNumber++;
        }

        // Open the log file in write-only mode, create if not exists, truncate if exists
        int logFile = open(logFilename, O_WRONLY | O_CREAT | O_TRUNC, 0666);
        
        // Redirect stdout to the log file
        dup2(logFile, STDOUT_FILENO);

        time_t currentTime;
        if (time(&currentTime) == -1) {
            printf("Error getting current time\n");
        }
        char *timeString = ctime(&currentTime);
        if (timeString == NULL) {
            printf("Error converting time to string\n");
        }

        // current time
        printf("Created log file!\n");
        printf("Current time: %s\n", timeString);
        printf("Log file path: %s\n", logFilename);
        pwd_command.function(NULL,NULL); //log direcotry
        dup2(original_stdout, STDOUT_FILENO);
        fflush(stdout);
        close(logFile);
        close(original_stdout);
        //printf("Log file path: %s\n", logFilename);

    return logFilename;

}

int openLog(char *logFilename) {
    // Open the log file in append mode
    printf("opening log file");
    int logFile = open(logFilename, O_WRONLY | O_APPEND);
    if (logFile == -1) {
        perror("Error opening log file");
        return -1; // Return -1 to indicate failure
    }

    // Redirect stdout to the log file
    dup2(logFile, STDOUT_FILENO);

    return logFile;
}

void closeLog(int logFile){
    close(logFile);
    
}