#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <ctype.h>
#include <unistd.h>
#include <time.h>
#include <omp.h>
#include <signal.h>
#include "creatbmp.h"

#define MAX_BUF 256

#define BMP_HEADER_SIZE 62
#define DIB_HEADER_SIZE 40

#define PORT 3333
extern int valami = 0;

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


void BMPcreator(int *values, int num_values)
{
    // Transform the input values
    int *transformed_values = (int *)malloc(num_values * sizeof(int));
    transformed_values[0] = 0;

    for (int i = 1; i < num_values; i++)
    {
        if (values[i] < values[i - 1])
        {
            transformed_values[i] = -1;
        }
        else if (values[i] > values[i - 1])
        {
            transformed_values[i] = 1;
        }
        else
        {
            transformed_values[i] = 0;
        }
    }

    // Calculate row size and BMP size
    int row_size;
    if (num_values % 32 == 0)
    {
        row_size = num_values;
    }
    else
    {
        row_size = ((num_values / 32) + 1) * 32;
    }
    int bmp_size = BMP_HEADER_SIZE + ((row_size / 8) * num_values);

    // Set BMP header values
    char *bmp_data = (char *)calloc(bmp_size, sizeof(char));
    bmp_data[0] = 'B';
    bmp_data[1] = 'M';
    *(int *)&bmp_data[2] = bmp_size;
    *(int *)&bmp_data[10] = BMP_HEADER_SIZE;
    *(int *)&bmp_data[14] = DIB_HEADER_SIZE;
    *(int *)&bmp_data[18] = num_values;
    *(int *)&bmp_data[22] = num_values;
    *(short *)&bmp_data[26] = 1;
    *(short *)&bmp_data[28] = 1;
    *(int *)&bmp_data[34] = row_size / 8 * num_values;
    *(int *)&bmp_data[38] = 2835; // horizontal resolution (72 DPI)
    *(int *)&bmp_data[42] = 2835; // vertical resolution (72 DPI)
    *(int *)&bmp_data[46] = 0;    // color palette
    *(int *)&bmp_data[50] = 0;    // important colors
    bmp_data[54] = 0;
    bmp_data[55] = 0;
    bmp_data[56] = 0;
    bmp_data[57] = 255;
    bmp_data[58] = 0;
    bmp_data[59] = 0;
    bmp_data[60] = 255;
    bmp_data[61] = 255;

    // Set the starting point at the middle of the first row
    bmp_data[BMP_HEADER_SIZE + (num_values / 2) * (row_size / 8)] |= 0x80;

    // Iterate over the transformed values and set the appropriate pixel values
    int current_row = num_values / 2;
    for (int i = 0; i < num_values; i++)
    {
        int byte_index = i / 8;
        current_row += transformed_values[i];
        if (current_row > num_values - 1)
        {
            bmp_data[BMP_HEADER_SIZE + byte_index + (num_values - 1) * (row_size / 8)] |= 1 << (7 - (i % 8));
        }
        else if (current_row < 0)
        {
            bmp_data[BMP_HEADER_SIZE + byte_index + 0 * (row_size / 8)] |= 1 << (7 - (i %
                                                                                      8));
        }
        else
        {
            bmp_data[BMP_HEADER_SIZE + byte_index + current_row * (row_size / 8)] |= 1 << (7 - (i % 8));
        }
    } // Write the BMP data to a file
    FILE *bmp_file = fopen("output.bmp", "wb");
    fwrite(bmp_data, sizeof(char), bmp_size, bmp_file);
    fclose(bmp_file);

    // Free allocated memory
    free(bmp_data);
    free(transformed_values);
}

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

int Measurement(int **Values) {
    time_t T1;
    int T2;
    T2 = time(&T1);

    srand((unsigned) time(&T1));

    int maxValues = T2 % 900;

    if(maxValues <= 99)
    {
        maxValues = 100;
    }

    // Dinamikus memória foglalása az értékek tárolására
    int *values = (int *)malloc(maxValues * sizeof(int));

    // A bolyongás implementálása
    int x = 0; // Kezdőérték
    values[0] = x;
    for (int i = 1; i < maxValues; i++) {
        int random = rand() % 100; // Véletlenszerű szám generálása 0 és 99 között

        // Az xi+1 = xi + 1 állítás 42,8571% eséllyel való teljesülése
        if (random < 42.8571) {
            x += 1;
        }
        // Az xi+1 = xi - 1 állítás 11/31-ed eséllyel való teljesülése
        else if (random < 42.8571 + (11.0 / 31.0) * 100) {
            x -= 1;
        }
        // Egyébként xi+1 = xi
        // (az xi+1 = xi eset a maradék 45,1429% eséllyel fordul elő)
        // A két szomszédos érték különbsége maximum 1
        // Azonos érték is előfordulhat
        values[i] = x;
    }

    // Az értékek átadása a Values pointernek
    *Values = values;

    // Az előállított értékek számával való visszatérés
    return maxValues;
}

void ReceiveViaFile(int sig) {
    FILE *file;
    char *filename = "Measurement.txt";
    int *ertek = NULL;
    int count = 0;
    
    file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error opening file %s", filename);
        return;
    }
    
    // Beolvasás és memória foglalás
    int value;
    while (fscanf(file, "%d", &value) == 1) {
        count++;
        ertek = (int*) realloc(ertek, count * sizeof(int));
        ertek[count-1] = value;
        printf("%d\n", ertek[count-1]);
    }
    
    // BMPcreator hívása
    BMPcreator(ertek, count);
    
    // Memória felszabadítása
    free(ertek);
}

void SendViaFile(int *Values, int NumValues)
{
    FILE *file = fopen("Measurement.txt","w");
    for (int i = 0; i < NumValues; i++)
    {
        fprintf(file, "%d\n",Values[i]);
    }
    
    fclose(file);

    int pid = FindPID();

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

void SignalHandler(int sig) {
  switch(sig) {
    case SIGINT:
      printf("Program terminated by user.\n");
      exit(0);
    case SIGUSR1:
      if(valami = 1)
      {
        ReceiveViaFile;
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