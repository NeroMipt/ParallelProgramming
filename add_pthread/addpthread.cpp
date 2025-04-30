#include <iostream>
#include <pthread.h>
#include <stdexcept>
#include <string>

void print_pthread_attr_defaults() {
    pthread_attr_t attr;
    int ret;


    ret = pthread_attr_init(&attr);
    if (ret != 0) {
        throw std::runtime_error("Ошибка при инициализации атрибутов потока: " + std::to_string(ret));
    }


    int scope;
    ret = pthread_attr_getscope(&attr, &scope);
    if (ret != 0) {
        std::cerr << "Ошибка при получении SCOPE: " << ret << std::endl;
    } else {
        std::cout << "SCOPE (область действия потока): ";
        switch (scope) {
            case PTHREAD_SCOPE_SYSTEM:
                std::cout << "PTHREAD_SCOPE_SYSTEM" << std::endl;
                break;
            case PTHREAD_SCOPE_PROCESS:
                std::cout << "PTHREAD_SCOPE_PROCESS" << std::endl;
                break;
            default:
                std::cout << "UNKNOW: " << scope << std::endl;
        }
    }


    int detachstate;
    ret = pthread_attr_getdetachstate(&attr, &detachstate);
    if (ret != 0) {
        std::cerr << "Ошибка при получении DETACHSTATE: " << ret << std::endl;
    } else {
        std::cout << "DETACHSTAT: ";
        switch (detachstate) {
            case PTHREAD_CREATE_JOINABLE:
                std::cout << "PTHREAD_CREATE_JOINABLE (поток завершается, но требует pthread_join для освобождения ресурсов)" << std::endl;
                break;
            case PTHREAD_CREATE_DETACHED:
                std::cout << "PTHREAD_CREATE_DETACHED (поток автоматически освобождает ресурсы после завершения)" << std::endl;
                break;
            default:
                std::cout << "Unknow: " << detachstate << std::endl;
        }
    }


    void* stackaddr;
    size_t stacksize;
    ret = pthread_attr_getstack(&attr, &stackaddr, &stacksize);
    if (ret != 0) {
        std::cerr << "Ошибка при получении STACKADDR/STACKSIZE: " << ret << std::endl;
    } else {
        std::cout << "STACKADDR (адрес стека): " << stackaddr << std::endl;
        std::cout << "STACKSIZE (размер стека): " << stacksize << " байт" << std::endl;
    }


    int inheritsched;
    ret = pthread_attr_getinheritsched(&attr, &inheritsched);
    if (ret != 0) {
        std::cerr << "Ошибка при получении INHERITSCHED: " << ret << std::endl;
    } else {
        std::cout << "INHERITSCHED (наследование параметров планирования): ";
        switch (inheritsched) {
            case PTHREAD_INHERIT_SCHED:
                std::cout << "PTHREAD_INHERIT_SCHED (наследует параметры родителя)" << std::endl;
                break;
            case PTHREAD_EXPLICIT_SCHED:
                std::cout << "PTHREAD_EXPLICIT_SCHED (использует явные параметры)" << std::endl;
                break;
            default:
                std::cout << "Неизвестное значение: " << inheritsched << std::endl;
        }
    }


    int schedpolicy;
    ret = pthread_attr_getschedpolicy(&attr, &schedpolicy);
    if (ret != 0) {
        std::cerr << "Ошибка при получении SCHEDPOLICY: " << ret << std::endl;
    } else {
        std::cout << "SCHEDPOLICY (политика планирования): ";
        switch (schedpolicy) {
            case SCHED_FIFO:
                std::cout << "SCHED_FIFO (реальное время, FIFO)" << std::endl;
                break;
            case SCHED_RR:
                std::cout << "SCHED_RR (реальное время, Round-Robin)" << std::endl;
                break;
            case SCHED_OTHER:
                std::cout << "SCHED_OTHER (стандартное планирование)" << std::endl;
                break;
            default:
                std::cout << "Неизвестное значение: " << schedpolicy << std::endl;
        }
    }


    pthread_attr_destroy(&attr);
}

int main() {
    try {
        std::cout << "Значения атрибутов потока по умолчанию:" << std::endl;
        print_pthread_attr_defaults();
    } catch (const std::exception& e) {
        std::cerr << "Ошибка: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}