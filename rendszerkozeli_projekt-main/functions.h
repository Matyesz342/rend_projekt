#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>


void version(void)
{
    time_t T1;
    int T2;
    T2 = time(&T1);
    #pragma omp parallel sections
    {
    #pragma omp section
      {
      printf("A program verzioszama: 1.0\n");
      }
    #pragma omp section
      {
      printf("A program letrejottenek az ideje: %s\n",ctime(&T1));
      }
    #pragma omp section
      {
      printf("A program keszitoje: Machaty Matyas\n");
      }
    }
}

void help(void)
{
    printf("Lehetseges parancssori argumentumok:\n");
    printf("--version: kiirja a program verzioszamat, elkeszultenek datumat es a fejleszto nevet\n");
    printf("-send: kuldo uzemmod\n");
    printf("-receive: fogado uzemmod\n");
    printf("-file: fajl kommunikacios mod\n");
    printf("-socket: socket kommunikacios mod\n");
}