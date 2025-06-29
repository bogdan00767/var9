#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <time.h>

#define INITIAL_CAPACITY 1000
#define MAX_INPUT_LENGTH 20
#define MAX_FILENAME_LENGTH 100

typedef struct {
    int* data;
    int count;
    int capacity;
} NumberArray;

void sortIfNeeded(NumberArray* arr);
void showMainMenu();
void getNums(NumberArray* arr);
void autoGen(NumberArray* arr);
void saveNumsToFile(NumberArray* arr, const char* filename);
void loadNumsFromFile(NumberArray* arr, const char* filename);
void printNums(NumberArray* arr);
void radixSort(int* arr, int n);
int getMax(int* arr, int n);
void countSort(int* arr, int n, int exp);
int isInteger(const char* input);
void clearScreen();
void addTxtExtensionIfMissing(char* fileName);
int fileExists(const char* filename);
int isValidFileName(const char* name);
void ensureCapacity(NumberArray* arr, int minCapacity);

int main() {
    setlocale(LC_ALL, "Rus");
    NumberArray arr = {NULL, 0, 0};
    char choice;

    // Выделение начальной памяти для массива
    arr.data = (int*)malloc(INITIAL_CAPACITY * sizeof(int));
    if (!arr.data) {
        printf("Ошибка выделения памяти!\n");
        return 1;
    }
    arr.capacity = INITIAL_CAPACITY;

    while (1) {
        clearScreen();
        showMainMenu();
        printf("Ваш выбор: ");
        scanf(" %c", &choice);

        switch (choice) {
            case '1':
                getNums(&arr);
                sortIfNeeded(&arr);
                break;
            case '2':
                autoGen(&arr);
                sortIfNeeded(&arr);
                break;
            case '3':
                free(arr.data);
                printf("Программа завершена.\n");
                return 0;
            default:
                printf("Неверный ввод. Пожалуйста, выберите 1, 2 или 3.\n");
                printf("\nНажмите Enter чтобы продолжить...");
                getchar(); getchar();
        }
    }
}

// Проверка и увеличение вместимости массива
void ensureCapacity(NumberArray* arr, int minCapacity) {
    if (arr->capacity < minCapacity) {
        int newCapacity = arr->capacity * 2;
        if (newCapacity < minCapacity) {
            newCapacity = minCapacity;
        }
        // Перевыделение памяти
        int* newData = (int*)realloc(arr->data, newCapacity * sizeof(int));
        if (!newData) {
            printf("Ошибка выделения памяти!\n");
            exit(1);
        }
        arr->data = newData;
        arr->capacity = newCapacity;
    }
}

void sortIfNeeded(NumberArray* arr) {
    if (arr->count > 0) {
        char unsortedFilename[MAX_FILENAME_LENGTH];
        char sortedFilename[MAX_FILENAME_LENGTH];

        clearScreen();
        printf("Введите имя файла для сохранения неотсортированных чисел: ");
        scanf("%99s", unsortedFilename);
        addTxtExtensionIfMissing(unsortedFilename);

        saveNumsToFile(arr, unsortedFilename);
        printf("\nСгенерированные числа сохранены в файл \"%s\"\n", unsortedFilename);
        printf("\nСгенерированные числа:\n");
        printNums(arr);
        
        printf("\nХотите отсортировать числа? (y/n): ");
        char sortChoice;
        scanf(" %c", &sortChoice);
        
        if (sortChoice == 'y' || sortChoice == 'Y') {
            printf("Введите имя файла для сохранения отсортированных чисел: ");
            scanf("%99s", sortedFilename);
            addTxtExtensionIfMissing(sortedFilename);

            radixSort(arr->data, arr->count);
            printf("\nОтсортированные числа:\n");
            printNums(arr);
            
            saveNumsToFile(arr, sortedFilename);
            printf("\nОтсортированные числа сохранены в файл \"%s\"\n", sortedFilename);
        }
        
        printf("\nНажмите Enter чтобы продолжить...");
        getchar(); getchar();
    }
}

void showMainMenu() {
    printf("========================================\n");
    printf("|         Меню управления числами      |\n");
    printf("========================================\n");
    printf("1. Ввод чисел вручную\n");
    printf("2. Заполнить автоматически\n");
    printf("3. Завершить программу\n");
    printf("========================================\n");
}

void getNums(NumberArray* arr) {
    clearScreen();
    printf("Введите числа от -10000 до 10000 (для завершения введите 'end'):\n");
    
    arr->count = 0;
    char input[MAX_INPUT_LENGTH];
    
    while (1) {
        printf("Число #%d: ", arr->count + 1);
        scanf("%s", input);
        
        if (strcmp(input, "end") == 0) {
            break;
        }
        
        if (!isInteger(input)) {
            printf("Ошибка! Введите целое число от -10000 до 10000.\n");
            continue;
        }
        
        int num = atoi(input);
        if (num < -10000 || num > 10000) {
            printf("Число должно быть в диапазоне от -10000 до 10000.\n");
            continue;
        }
        
        // Проверка и при необходимости увеличение вместимости
        ensureCapacity(arr, arr->count + 1);
        arr->data[arr->count++] = num;
    }
    
    printf("\nВведено %d чисел.\n", arr->count);
}

void autoGen(NumberArray* arr) {
    clearScreen();
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
    
    ensureCapacity(arr, n);
    arr->count = n;
    
    srand(time(NULL));
    for (int i = 0; i < n; i++) {
        arr->data[i] = rand() % (max - min + 1) + min;
    }
    
    printf("\nСгенерировано %d чисел в диапазоне от %d до %d.\n", n, min, max);
}

void saveNumsToFile(NumberArray* arr, const char* filename) {
    FILE* file = fopen(filename, "w");
    if (!file) {
        printf("Ошибка сохранения в файл '%s'.\n", filename);
        return;
    }
    
    for (int i = 0; i < arr->count; i++) {
        fprintf(file, "%d\n", arr->data[i]);
    }
    
    fclose(file);
}

void loadNumsFromFile(NumberArray* arr, const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        return;
    }
    
    arr->count = 0;
    int num;
    while (fscanf(file, "%d", &num) == 1) {
        ensureCapacity(arr, arr->count + 1);
        arr->data[arr->count++] = num;
    }
    
    fclose(file);
}

void printNums(NumberArray* arr) {
    for (int i = 0; i < arr->count; i++) {
        printf("%d ", arr->data[i]);
        if ((i + 1) % 10 == 0) printf("\n");
    }
    printf("\n");
}

int getMax(int* arr, int n) {
    int mx = arr[0];
    for (int i = 1; i < n; i++)
        if (arr[i] > mx)
            mx = arr[i];
    return mx;
}

// Вспомогательная функция сортировки подсчетом для поразрядной
void countSort(int* arr, int n, int exp) {
    int* output = (int*)malloc(n * sizeof(int));
    int i, count[10] = {0};
    
    // Смещение для отрицательных
    int bias = 0;
    int min = 0;
    for (i = 0; i < n; i++) {
        if (arr[i] < min) min = arr[i];
    }
    if (min < 0) bias = -min;
    
    for (i = 0; i < n; i++)
        count[((arr[i] + bias) / exp) % 10]++;
    
    for (i = 1; i < 10; i++)
        count[i] += count[i - 1];
    
    for (i = n - 1; i >= 0; i--) {
        output[count[((arr[i] + bias) / exp) % 10] - 1] = arr[i];
        count[((arr[i] + bias) / exp) % 10]--;
    }
    
    for (i = 0; i < n; i++)
        arr[i] = output[i];
    
    free(output);
}

void radixSort(int* arr, int n) {
    int m = getMax(arr, n);
    
    for (int exp = 1; m / exp > 0; exp *= 10)
        countSort(arr, n, exp);
}

int isInteger(const char* input) {
    char* endptr;
    strtol(input, &endptr, 10);
    return (*endptr == '\0');
}

void clearScreen() {
    system("cls || clear");
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