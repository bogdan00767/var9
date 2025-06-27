#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <time.h>

// Константы программы
#define INITIAL_CAPACITY 1000  // Начальный размер массива
#define MAX_INPUT_LENGTH 20    // Максимальная длина ввода числа
#define MAX_FILENAME_LENGTH 100 // Максимальная длина имени файла

// Структура для хранения массива чисел и информации о нём
typedef struct {
    int* data;     // Указатель на динамический массив чисел
    int count;     // Текущее количество чисел в массиве
    int capacity;  // Текущая вместимость массива
} NumberArray;

// Прототипы функций
void sortIfNeeded(NumberArray* arr);  // Основная функция обработки чисел
void showMainMenu();  // Отображение главного меню
void getNums(NumberArray* arr);  // Ручной ввод чисел
void autoGen(NumberArray* arr);  // Автогенерация чисел
void saveNumsToFile(NumberArray* arr, const char* filename);  // Сохранение в файл
void loadNumsFromFile(NumberArray* arr, const char* filename);  // Загрузка из файла
void printNums(NumberArray* arr);  // Вывод чисел на экран
void radixSort(int* arr, int n);  // Поразрядная сортировка
int getMax(int* arr, int n);  // Нахождение максимального числа
void countSort(int* arr, int n, int exp);  // Вспомогательная сортировка подсчетом
int isValidNumber(const char* input);  // Проверка корректности ввода числа
void clearScreen();  // Очистка экрана консоли
void addTxtExtensionIfMissing(char* fileName);  // Добавление .txt к имени файла
int fileExists(const char* filename);  // Проверка существования файла
int isValidFileName(const char* name);  // Проверка корректности имени файла
void ensureCapacity(NumberArray* arr, int minCapacity);  // Проверка и увеличение вместимости массива

int main() {
    setlocale(LC_ALL, "Rus");  // Установка русской локали
    NumberArray arr = {NULL, 0, 0};  // Инициализация структуры массива
    char choice;  // Выбор пользователя в меню

    // Выделение начальной памяти для массива
    arr.data = (int*)malloc(INITIAL_CAPACITY * sizeof(int));
    if (!arr.data) {
        printf("Ошибка выделения памяти!\n");
        return 1;
    }
    arr.capacity = INITIAL_CAPACITY;  // Установка начальной вместимости

    // Основной цикл программы
    do {
        clearScreen();
        showMainMenu();
        printf("Ваш выбор: ");
        scanf(" %c", &choice);

        switch (choice) {
            case '1':
                getNums(&arr);  // Ручной ввод чисел
                sortIfNeeded(&arr);  // Обработка чисел
                break;
            case '2':
                autoGen(&arr);  // Автогенерация чисел
                sortIfNeeded(&arr);  // Обработка чисел
                break;
            case '3':
                free(arr.data);  // Освобождение памяти перед выходом
                printf("Программа завершена.\n");
                return 0;
            default:
                printf("Неверный ввод. Пожалуйста, выберите 1, 2 или 3.\n");
                printf("\nНажмите Enter чтобы продолжить...");
                getchar(); getchar();
        }
    } while (1);
}

// Проверяет и при необходимости увеличивает вместимость массива
void ensureCapacity(NumberArray* arr, int minCapacity) {
    if (arr->capacity < minCapacity) {
        // Увеличиваем вместимость в 2 раза или до требуемого размера
        int newCapacity = arr->capacity * 2;
        if (newCapacity < minCapacity) {
            newCapacity = minCapacity;
        }
        // Перевыделение памяти с проверкой
        int* newData = (int*)realloc(arr->data, newCapacity * sizeof(int));
        if (!newData) {
            printf("Ошибка выделения памяти!\n");
            exit(1);
        }
        arr->data = newData;
        arr->capacity = newCapacity;
    }
}

// Функция обработки чисел: вывод, сортировка и сохранение
void sortIfNeeded(NumberArray* arr) {
    if (arr->count > 0) {
        char unsortedFilename[MAX_FILENAME_LENGTH];  // Для неотсортированных чисел
        char sortedFilename[MAX_FILENAME_LENGTH];   // Для отсортированных чисел

        clearScreen();
        printf("Введите имя файла для сохранения неотсортированных чисел: ");
        scanf("%99s", unsortedFilename);
        addTxtExtensionIfMissing(unsortedFilename);

        // Сохраняем неотсортированные числа
        saveNumsToFile(arr, unsortedFilename);
        printf("\nСгенерированные числа сохранены в файл \"%s\"\n", unsortedFilename);
        printf("\nСгенерированные числа:\n");
        printNums(arr);
        
        // Предлагаем пользователю сортировку
        printf("\nХотите отсортировать числа? (y/n): ");
        char sortChoice;
        scanf(" %c", &sortChoice);
        
        if (sortChoice == 'y' || sortChoice == 'Y') {
            printf("Введите имя файла для сохранения отсортированных чисел: ");
            scanf("%99s", sortedFilename);
            addTxtExtensionIfMissing(sortedFilename);

            radixSort(arr->data, arr->count);  // Сортируем числа
            printf("\nОтсортированные числа:\n");
            printNums(arr);
            
            // Сохраняем отсортированные числа
            saveNumsToFile(arr, sortedFilename);
            printf("\nОтсортированные числа сохранены в файл \"%s\"\n", sortedFilename);
        }
        
        printf("\nНажмите Enter чтобы продолжить...");
        getchar(); getchar();
    }
}

// Вывод главного меню
void showMainMenu() {
    printf("========================================\n");
    printf("|         Меню управления числами      |\n");
    printf("========================================\n");
    printf("1. Ввод чисел вручную\n");
    printf("2. Заполнить автоматически\n");
    printf("3. Завершить программу\n");
    printf("========================================\n");
}

// Функция ручного ввода чисел с динамическим расширением массива
void getNums(NumberArray* arr) {
    clearScreen();
    printf("Введите числа от -10000 до 10000 (для завершения введите 'end'):\n");
    
    arr->count = 0;  // Сбрасываем счетчик чисел
    char input[MAX_INPUT_LENGTH];  // Буфер для ввода
    
    while (1) {
        printf("Число #%d: ", arr->count + 1);
        scanf("%s", input);
        
        if (strcmp(input, "end") == 0) {  // Проверка на завершение ввода
            break;
        }
        
        if (!isValidNumber(input)) {  // Проверка корректности числа
            printf("Ошибка! Введите целое число от -10000 до 10000.\n");
            continue;
        }
        
        int num = atoi(input);
        if (num < -10000 || num > 10000) {  // Проверка диапазона
            printf("Число должно быть в диапазоне от -10000 до 10000.\n");
            continue;
        }
        
        // Проверяем и при необходимости увеличиваем вместимость
        ensureCapacity(arr, arr->count + 1);
        arr->data[arr->count++] = num;  // Добавляем число в массив
    }
    
    printf("\nВведено %d чисел.\n", arr->count);
}

// Функция автоматической генерации чисел
void autoGen(NumberArray* arr) {
    clearScreen();
    int min, max, n;
    
    printf("Введите минимальное значение: ");
    scanf("%d", &min);
    printf("Введите максимальное значение: ");
    scanf("%d", &max);
    printf("Введите количество чисел: ");
    scanf("%d", &n);
    
    if (min > max) {  // Если минимум больше максимума, меняем местами
        int temp = min;
        min = max;
        max = temp;
    }
    
    // Гарантируем достаточную вместимость массива
    ensureCapacity(arr, n);
    arr->count = n;  // Устанавливаем количество чисел
    
    srand(time(NULL));  // Инициализация генератора случайных чисел
    for (int i = 0; i < n; i++) {
        arr->data[i] = rand() % (max - min + 1) + min;  // Генерация числа
    }
    
    printf("\nСгенерировано %d чисел в диапазоне от %d до %d.\n", n, min, max);
}

// Функция сохранения чисел в файл
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

// Функция загрузки чисел из файла с динамическим расширением массива
void loadNumsFromFile(NumberArray* arr, const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        return;
    }
    
    arr->count = 0;
    int num;
    while (fscanf(file, "%d", &num) == 1) {
        ensureCapacity(arr, arr->count + 1);  // Проверяем вместимость
        arr->data[arr->count++] = num;  // Добавляем число
    }
    
    fclose(file);
}

// Функция вывода чисел на экран
void printNums(NumberArray* arr) {
    for (int i = 0; i < arr->count; i++) {
        printf("%d ", arr->data[i]);
        if ((i + 1) % 10 == 0) printf("\n");  // Перенос строки каждые 10 чисел
    }
    printf("\n");
}

// Находит максимальное число в массиве (для поразрядной сортировки)
int getMax(int* arr, int n) {
    int mx = arr[0];
    for (int i = 1; i < n; i++)
        if (arr[i] > mx)
            mx = arr[i];
    return mx;
}

// Вспомогательная функция сортировки подсчетом для поразрядной сортировки
void countSort(int* arr, int n, int exp) {
    int* output = (int*)malloc(n * sizeof(int));  // Выходной массив
    int i, count[10] = {0};  // Массив подсчета (10 цифр)
    
    // Для обработки отрицательных чисел используем смещение
    int bias = 0;
    int min = 0;
    for (i = 0; i < n; i++) {
        if (arr[i] < min) min = arr[i];
    }
    if (min < 0) bias = -min;
    
    // Подсчет количества чисел для каждой цифры
    for (i = 0; i < n; i++)
        count[((arr[i] + bias) / exp) % 10]++;
    
    // Преобразуем count в позиции в выходном массиве
    for (i = 1; i < 10; i++)
        count[i] += count[i - 1];
    
    // Строим выходной массив
    for (i = n - 1; i >= 0; i--) {
        output[count[((arr[i] + bias) / exp) % 10] - 1] = arr[i];
        count[((arr[i] + bias) / exp) % 10]--;
    }
    
    // Копируем выходной массив обратно
    for (i = 0; i < n; i++)
        arr[i] = output[i];
    
    free(output);  // Освобождаем временный массив
}

// Основная функция поразрядной сортировки
void radixSort(int* arr, int n) {
    int m = getMax(arr, n);  // Максимальное число определяет количество разрядов
    
    // Сортируем для каждого разряда
    for (int exp = 1; m / exp > 0; exp *= 10)
        countSort(arr, n, exp);
}

// Проверка, является ли строка корректным целым числом
int isValidNumber(const char* input) {
    char* endptr;
    strtol(input, &endptr, 10);
    return (*endptr == '\0');
}

// Очистка экрана консоли
void clearScreen() {
    system("cls || clear");
}

// Добавление расширения .txt, если оно отсутствует
void addTxtExtensionIfMissing(char* fileName) {
    if (!strchr(fileName, '.')) {
        strcat(fileName, ".txt");
    }
}

// Проверка существования файла
int fileExists(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file) {
        fclose(file);
        return 1;
    }
    return 0;
}

// Проверка корректности имени файла (запрещенные символы)
int isValidFileName(const char* name) {
    const char* invalidChars = "\\/:*?\"<>|";
    for (int i = 0; name[i] != '\0'; i++) {
        if (strchr(invalidChars, name[i])) return 0;
    }
    return 1;
}