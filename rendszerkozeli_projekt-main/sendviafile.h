#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include "findPID.h"

void SendViaFile(int *Values, int NumValues)
{
    FILE *file = fopen("Measurement.txt","w");
    for (int i = 0; i < NumValues; i++)
    {
        fprintf(file, "%d\n",Values[i]);
    }
    
    fclose(file);

    int pid = FindPID();
    printf("%d\n",pid);
    if (pid == -1)
    {
        fprintf(stderr, "Hiba: Nem található fogadó üzemmódban műküdő folyamat\n");
        exit(1);
    }
    else
    {
        kill(pid, SIGUSR1);
    }
}
