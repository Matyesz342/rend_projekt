#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <ctype.h>
#include <unistd.h>

#define MAX_BUF 256

int FindPID() {
    DIR *dir;
    struct dirent *ent;
    FILE *fp;
    char path[MAX_BUF];
    char line[MAX_BUF];
    int pid = -1;

    // Open /proc directory
    if ((dir = opendir("/proc")) == NULL) {
        perror("opendir() error");
        return -1;
    }

    // Traverse subdirectories
    while ((ent = readdir(dir)) != NULL) {
        // Check if subdirectory name starts with a digit
        if (isdigit(*ent->d_name)) {
            snprintf(path, MAX_BUF, "/proc/%s/status", ent->d_name);

            // Open status file
            if ((fp = fopen(path, "r")) != NULL) {
                // Read first line
                if (fgets(line, MAX_BUF, fp) != NULL) {
                    // Check if line contains "chart" string
                    if (strstr(line, "chart") != NULL) {
                        // Search for Pid: number
                        while (fgets(line, MAX_BUF, fp) != NULL) {
                            if (strncmp(line, "Pid:\t", 5) == 0) {
                                pid = atoi(line+5);
                                break;
                            }
                        }
                    }
                }
                fclose(fp);

                // Found PID, return it
                if (pid > 0) {
                    closedir(dir);
                    return pid;
                }
            }
        }
    }

    closedir(dir);
    return pid;
}

