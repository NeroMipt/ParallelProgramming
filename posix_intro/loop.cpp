//
// Created by egor on 4/22/25.
//
#include <iostream>
#include <pthread.h>
#include <vector>


int shared_variable = 0;
pthread_mutex_t mutex;
pthread_cond_t cond_var;
int current_thread = 0;


struct ThreadData {
    int id;
};


void* thread_function(void* arg) {
    ThreadData* data = static_cast<ThreadData*>(arg);
    int thread_id = data->id;


    pthread_mutex_lock(&mutex);


    while (thread_id != current_thread) {
        pthread_cond_wait(&cond_var, &mutex);
    }


    std::cout << "Поток " << thread_id << " получил доступ. ";
    shared_variable++;
    std::cout << "Новое значение переменной: " << shared_variable << std::endl;


    current_thread++;


    pthread_cond_broadcast(&cond_var);


    pthread_mutex_unlock(&mutex);

    return nullptr;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Использование: " << argv[0] << " <количество потоков>" << std::endl;
        return 1;
    }

    int num_threads = std::stoi(argv[1]);
    if (num_threads <= 0) {
        std::cerr << "Количество потоков должно быть положительным числом" << std::endl;
        return 1;
    }


    pthread_mutex_init(&mutex, nullptr);
    pthread_cond_init(&cond_var, nullptr);


    std::vector<pthread_t> threads(num_threads);
    std::vector<ThreadData> thread_data(num_threads);

    for (int i = 0; i < num_threads; ++i) {
        thread_data[i].id = i;
        pthread_create(&threads[i], nullptr, thread_function, &thread_data[i]);
    }


    for (int i = 0; i < num_threads; ++i) {
        pthread_join(threads[i], nullptr);
    }


    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond_var);

    std::cout << "Финальное значение переменной: " << shared_variable << std::endl;

    return 0;
}