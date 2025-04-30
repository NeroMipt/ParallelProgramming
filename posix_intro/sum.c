#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

// Структура для передачи данных в поток
typedef struct {
    int start;
    int end;
    double partial_sum;
} ThreadData;

// Функция, которую выполняет каждый поток
void* calculate_partial_sum(void* arg) {
    ThreadData* data = (ThreadData*)arg;
    data->partial_sum = 0.0;

    for (int i = data->start; i <= data->end; i++) {
        data->partial_sum += 1.0 / i;
    }

    return NULL;
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Использование: %s <N> <количество потоков>\n", argv[0]);
        return 1;
    }

    int N = atoi(argv[1]);
    int num_threads = atoi(argv[2]);

    if (N <= 0 || num_threads <= 0) {
        fprintf(stderr, "N и количество потоков должны быть положительными числами\n");
        return 1;
    }

    pthread_t* threads = malloc(num_threads * sizeof(pthread_t));
    ThreadData* thread_data = malloc(num_threads * sizeof(ThreadData));

    // Распределение работы между потоками
    int chunk_size = N / num_threads;
    int remainder = N % num_threads;
    int current_start = 1;

    for (int i = 0; i < num_threads; i++) {
        thread_data[i].start = current_start;
        thread_data[i].end = current_start + chunk_size - 1;

        if (remainder > 0) {
            thread_data[i].end++;
            remainder--;
        }

        current_start = thread_data[i].end + 1;

        pthread_create(&threads[i], NULL, calculate_partial_sum, &thread_data[i]);
    }


    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }


    double total_sum = 0.0;
    for (int i = 0; i < num_threads; i++) {
        total_sum += thread_data[i].partial_sum;
    }

    printf("Частичная сумма гармонического ряда для N=%d: %.15f\n", N, total_sum);


    free(threads);
    free(thread_data);

    return 0;
}