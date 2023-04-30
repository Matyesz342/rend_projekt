#include <stdio.h>
#include <stdlib.h>
#include "creatbmp.h"

void ReceiveViaFile(int sig) {
    FILE *file;
    char *filename = "Measurement.txt";
    int *values = NULL;
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
        values = (int*) realloc(values, count * sizeof(int));
        values[count-1] = value;
    }
    
    // BMPcreator hívása
    BMPcreator(values, count);
    
    // Memória felszabadítása
    free(values);
}
