#include <iostream>
#include <cmath>
#include <pthread.h>
#include <semaphore.h>
#include <sys/time.h>
#include <iomanip>

// Глобальные структуры данных
struct SharedData {
    double s_all = 0.0;
    int ntask = 0;
    int nactive = 0;
    int maxtask = 1000;
    pthread_mutex_t mutex;
    sem_t sem_task_present;
};

struct Task {
    double a, b;
    double fa, fb;
    double sab;
};

SharedData sdat;
Task* global_stack = nullptr;
const int nproc = 1;
const double eps = 1e-8;

// Функция для вычисления
double fun(double x) {
    return cos(1.0 / (x));
}


bool BreakCond(double sacb, double sab) {
    return fabs(sacb - sab) >= eps * fabs(sacb);
}


void* slave_thr(void* arg) {
    double s = 0.0;
    int sp = 0;
    Task local_stack[100];

    while (true) {

        sem_wait(&sdat.sem_task_present);


        pthread_mutex_lock(&sdat.mutex);
        sdat.ntask--;
        Task current_task = global_stack[sdat.ntask];

        if (sdat.ntask > 0) {
            sem_post(&sdat.sem_task_present);
        }

        if (current_task.a <= current_task.b) {
            sdat.nactive++;
        }
        pthread_mutex_unlock(&sdat.mutex);


        if (current_task.a > current_task.b) {
            break;
        }


        double a = current_task.a, b = current_task.b;
        double fa = current_task.fa, fb = current_task.fb;
        double sab = current_task.sab;

        while (true) {
            double c = (a + b) / 2;
            double fc = fun(c);
            double sac = (fa + fc) * (c - a) / 2;
            double scb = (fc + fb) * (b - c) / 2;
            double sacb = sac + scb;

            if (!BreakCond(sacb, sab)) {
                s += sacb;
                if (sp == 0) {
                    break;
                }

                sp--;
                a = local_stack[sp].a;
                b = local_stack[sp].b;
                fa = local_stack[sp].fa;
                fb = local_stack[sp].fb;
                sab = local_stack[sp].sab;
            } else {

                if (sp < 100) {
                    local_stack[sp].a = c;
                    local_stack[sp].b = b;
                    local_stack[sp].fa = fc;
                    local_stack[sp].fb = fb;
                    local_stack[sp].sab = scb;
                    sp++;
                }

                b = c;
                fb = fc;
                sab = sac;
            }


            if ((sp > 1) && (sdat.ntask == 0)) {
                pthread_mutex_lock(&sdat.mutex);
                if (sdat.ntask == 0) {
                    sem_post(&sdat.sem_task_present);
                }
                while ((sp > 0) && (sdat.ntask < sdat.maxtask)) {
                    sp--;
                    global_stack[sdat.ntask] = local_stack[sp];
                    sdat.ntask++;
                }
                pthread_mutex_unlock(&sdat.mutex);
            }
        }


        pthread_mutex_lock(&sdat.mutex);
        sdat.nactive--;
        if ((sdat.nactive == 0) && (sdat.ntask == 0)) {

            for (int i = 0; i < nproc; i++) {
                global_stack[sdat.ntask].a = 2.0;
                global_stack[sdat.ntask].b = 1.0;
                sdat.ntask++;
            }
            sem_post(&sdat.sem_task_present);
        }
        pthread_mutex_unlock(&sdat.mutex);
    }


    pthread_mutex_lock(&sdat.mutex);
    sdat.s_all += s;
    pthread_mutex_unlock(&sdat.mutex);

    return nullptr;
}

int main() {
    struct timeval ts, te;
    gettimeofday(&ts, NULL);

    global_stack = new Task[sdat.maxtask];
    pthread_mutex_init(&sdat.mutex, nullptr);
    sem_init(&sdat.sem_task_present, 0, 0);


    global_stack[0].a = 0.01;
    global_stack[0].b = 8.0;
    global_stack[0].fa = fun(global_stack[0].a);
    global_stack[0].fb = fun(global_stack[0].b);
    global_stack[0].sab = (global_stack[0].fa + global_stack[0].fb) *
                          (global_stack[0].b - global_stack[0].a) / 2;
    sdat.ntask = 1;
    sem_post(&sdat.sem_task_present);


    pthread_t threads[nproc];
    for (int i = 0; i < nproc; i++) {
        pthread_create(&threads[i], nullptr, slave_thr, nullptr);
    }


    for (int i = 0; i < nproc; i++) {
        pthread_join(threads[i], nullptr);
    }

    gettimeofday(&te, NULL);
    double elapsed;
    elapsed = (te.tv_sec - ts.tv_sec) * 1000;
    elapsed += (te.tv_usec - ts.tv_usec);
    std::cout << "Elapsed time: " << elapsed << std::endl;

    std::cout << "Integral of cos(1/x) from 0.001 to 8.0 = " << std::setprecision(8) << sdat.s_all << std::endl;


    delete[] global_stack;
    pthread_mutex_destroy(&sdat.mutex);
    sem_destroy(&sdat.sem_task_present);

    return 0;
}