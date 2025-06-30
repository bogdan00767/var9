#include "radixAlg.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

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
    // Для цифр от 0 до 9
    int count[10] = { 0 };

    // Подсчет количества чисел для каждой цифры
    for (int i = 0; i < n; i++) {
        count[(arr[i] / exp) % 10]++;
    }

    // Преобразуем count в позиции в output[]
    for (int i = 1; i < 10; i++) {
        count[i] += count[i - 1];
    }

    // Строим выходной массив с конца к началу
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
    if (n <= 1) return;
    int iteration_count = 0;
    // Измеряем время до начала сортировки
    clock_t start_time = clock();

    // Разделяем отрицательные и положительные числа
    int* neg = (int*)malloc(n * sizeof(int));
    int* pos = (int*)malloc(n * sizeof(int));
    int neg_size = 0, pos_size = 0;

    for (int i = 0; i < n; i++) {
        if (arr[i] < 0)
        // Инвертируем отрицательные
            neg[neg_size++] = -arr[i];
        else
            pos[pos_size++] = arr[i];
    }

    // Сортируем отрицательные числа
    if (neg_size > 0) {
        int max_abs = getMaxAbs(neg, neg_size);
        for (int exp = 1; max_abs / exp > 0; exp *= 10) {
            countSort(neg, neg_size, exp);
            // Увеличиваем счётчик итераций
            iteration_count++;
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

    // Сортируем положительные числа
    if (pos_size > 0) {
        int max_abs = getMaxAbs(pos, pos_size);
        for (int exp = 1; max_abs / exp > 0; exp *= 10) {
            countSort(pos, pos_size, exp);
             // Увеличиваем счётчик итераций
            iteration_count++;
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