#include "sharedFunctions.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void autoGen(NumberArray* arr) {
    int min, max, n;
    printf("Введите минимальное значение: ");
    scanf("%d", &min);
    printf("Введите максимальное значение: ");
    scanf("%d", &max);
    printf("Введите количество чисел: ");
    scanf("%d", &n);

    if (min > max) {
        int temp = min;
        min = max;
        max = temp;
    }

    if (arr->capacity < n) {
        arr->data = realloc(arr->data, n * sizeof(int));
        arr->capacity = n;
    }

    arr->count = n;
    srand(time(NULL));
    for (int i = 0; i < n; i++) {
        arr->data[i] = rand() % (max - min + 1) + min;
    }

    printf("\nСгенерировано %d чисел в диапазоне от %d до %d.\n", n, min, max);
}

int isInteger(const char* input) {
    char* endptr;
    strtol(input, &endptr, 10);
    return (*endptr == '\0');
}

void addTxtExtensionIfMissing(char* fileName) {
    if (!strchr(fileName, '.')) {
        strcat(fileName, ".txt");
    }
}

int fileExists(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file) {
        fclose(file);
        return 1;
    }
    return 0;
}

int isValidFileName(const char* name) {
    const char* invalidChars = "\\/:*?\"<>|";
    for (int i = 0; name[i] != '\0'; i++) {
        if (strchr(invalidChars, name[i])) return 0;
    }
    return 1;
}