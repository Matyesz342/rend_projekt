#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include "receiveviafile.h"
extern int valami = 0;

void SignalHandler(int sig) {
  switch(sig) {
    case SIGINT:
      printf("Program terminated by user.\n");
      exit(0);
    case SIGUSR1:
      if(valami = 1)
      {
        ReceiveViaFile(sig);
      }
      else
      {
        printf("File transfer service is not available.\n");
      }
      break;
    case SIGALRM:
      printf("Server did not respond within the time limit.\n");
      exit(1);
    default:
      break;
  }
}
