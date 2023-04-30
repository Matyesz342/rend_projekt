#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include "functions.h"
#include "measurement.h"
#include "sendviafile.h"
#include "signalhandler.h"

int main(int argc, char* argv[])
{
    int *Values;
    int numValues;
    int send_mode = 1;
    int file_mode = 1;
    signal(SIGINT, SignalHandler);
    //signal(SIGUSR1, SignalHandler);
    signal(SIGALRM, SignalHandler);
    // set up signal handling for SIGUSR1
    /*struct sigaction act;
    act.sa_handler = ReceiveViaFile;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    sigaction(SIGUSR1, &act, NULL);*/

    if (strcmp(argv[0], "./chart") != 0)
    {
        fprintf(stderr, "Nem ./chart ként hívta meg a főprogramot\n");
        exit(1);
    }
    else if (argc == 1)
    {
        numValues = Measurement(&Values);
        SendViaFile(Values, numValues);
        free(Values);
        return 0;
    }
    else if (argc == 2)
    {
        if (strcmp(argv[1], "--version") == 0)
        {
            version();
            return 0;
        }
        else if (strcmp(argv[1], "-send") == 0)
        {
            numValues = Measurement(&Values);
            SendViaFile(Values, numValues);
            free(Values);
            return 0;
        }
        else if (strcmp(argv[1], "-receive") == 0)
        {
            while (1)
            {
                valami = 1;
                signal(SIGUSR1,SignalHandler);
            }
        }
        else if (strcmp(argv[1], "-file") == 0)
        {
            numValues = Measurement(&Values);
            SendViaFile(Values, numValues);
            free(Values);
            return 0;
        }
        else if (strcmp(argv[1], "-socket") == 0)
        {
            numValues = Measurement(&Values);
            SendViaFile(Values, numValues);
            free(Values);
            return 0;
        }
        else
        {
            help();
        }
        
    }
    else if (argc == 3)
    {
        if (strcmp(argv[1], "-send") == 0 && strcmp(argv[2], "-file") == 0 || strcmp(argv[2], "-send") == 0 && strcmp(argv[1], "-file") == 0)
        {
            numValues = Measurement(&Values);
            SendViaFile(Values, numValues);
            free(Values);
            return 0;
        }
        else if (strcmp(argv[1], "-receive") == 0 && strcmp(argv[2], "-file") == 0 || strcmp(argv[1], "-file") == 0 && strcmp(argv[2], "-receive") == 0)
        {
            while (1)
            {
                valami = 1;
                signal(SIGUSR1,SignalHandler);
            }
        }
        else if (strcmp(argv[1], "-socket") == 0 && strcmp(argv[2], "-send") == 0 || strcmp(argv[2], "-socket") == 0 && strcmp(argv[1], "-send") == 0)
        {
            numValues = Measurement(&Values);
            SendViaFile(Values, numValues);
            free(Values);
            return 0;
        }
        else if (strcmp(argv[1], "-receive") == 0 && strcmp(argv[2], "-socket") == 0 || strcmp(argv[2], "-receive") == 0 && strcmp(argv[1], "-socket") == 0)
        {
            while (1)
            {
                valami = 1;
                signal(SIGUSR1,SignalHandler);
            }
        }
        else
        {
            help();
        }
    }
    else
    {
        help();
        return 0;
    }
    return 0;
}