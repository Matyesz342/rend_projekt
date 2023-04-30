#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>

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