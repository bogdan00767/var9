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
    NumberArray arr = { NULL, 0, 0 };
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

int getMaxAbs(int* arr, int n) {
    int max_abs = abs(arr[0]);
    for (int i = 1; i < n; i++) {
        int current_abs = abs(arr[i]);
        if (current_abs > max_abs) {
            max_abs = current_abs;
        }
    }
    return max_abs;
}

void countSort(int* arr, int n, int exp) {
    int* output = (int*)malloc(n * sizeof(int));
    int count[10] = { 0 };  // Для цифр 0-9

    // Подсчет количества чисел для каждой цифры
    for (int i = 0; i < n; i++) {
        count[(arr[i] / exp) % 10]++;
    }

    // Преобразуем count в позиции в output[]
    for (int i = 1; i < 10; i++) {
        count[i] += count[i - 1];
    }

    // Строим выходной массив (идем с конца для стабильности)
    for (int i = n - 1; i >= 0; i--) {
        output[count[(arr[i] / exp) % 10] - 1] = arr[i];
        count[(arr[i] / exp) % 10]--;
    }

    // Копируем output обратно в arr[]
    for (int i = 0; i < n; i++) {
        arr[i] = output[i];
    }

    free(output);
}

void radixSort(int* arr, int n) {
    if (n <= 1) return;  // Не нужно сортировать пустой массив или из 1 элемента
    int iteration_count = 0;
    // Измеряем время до начала сортировки
    clock_t start_time = clock();

    // 1. Разделяем отрицательные и положительные числа
    int* neg = (int*)malloc(n * sizeof(int));
    int* pos = (int*)malloc(n * sizeof(int));
    int neg_size = 0, pos_size = 0;

    for (int i = 0; i < n; i++) {
        if (arr[i] < 0)
            neg[neg_size++] = -arr[i];  // Инвертируем отрицательные
        else
            pos[pos_size++] = arr[i];
    }

    // 2. Сортируем отрицательные числа (как положительные, затем разворачиваем)
    if (neg_size > 0) {
        int max_abs = getMaxAbs(neg, neg_size);
        for (int exp = 1; max_abs / exp > 0; exp *= 10) {
            countSort(neg, neg_size, exp);
            iteration_count++;  // Увеличиваем счётчик итераций
        }
        for (int i = 0; i < neg_size / 2; i++) {
            int temp = neg[i];
            neg[i] = neg[neg_size - 1 - i];
            neg[neg_size - 1 - i] = temp;
        }
        for (int i = 0; i < neg_size; i++) {
            arr[i] = -neg[i];
        }
    }

    // 3. Сортируем положительные числа
    if (pos_size > 0) {
        int max_abs = getMaxAbs(pos, pos_size);
        for (int exp = 1; max_abs / exp > 0; exp *= 10) {
            countSort(pos, pos_size, exp);
            iteration_count++;  // Увеличиваем счётчик итераций
        }
        for (int i = 0; i < pos_size; i++) {
            arr[neg_size + i] = pos[i];
        }
    }

    free(neg);
    free(pos);

    // Выводим количество итераций
    printf("Количество итераций сортировки: %d\n", iteration_count);

    // Измеряем время после завершения сортировки
    clock_t end_time = clock();

    // Выводим время сортировки
    double time_taken = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;
    printf("Время сортировки: %.6f секунд.\n", time_taken);
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