#include <pthread.h> /*used in other parts of the assignment */
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <stdint.h>  /* for uint64  */
#include <time.h>    /* for clock_gettime */
#include <atomic>    /*used in other parts of the assignment */
#define MAX_THREADS 8
#define NUM_POINTS 100000000
#define STEP (0.5/NUM_POINTS)

std::atomic<double>  pi{0};
double global_pi = 0;
pthread_mutex_t mutex_lock;
double sum[MAX_THREADS];

double f(double x) {
    return (6.0/sqrt(1-x*x));
}

void add_to_pi(double x) {
    auto current = pi.load();
    while (!pi.compare_exchange_weak(current, current + x));
}

void* part4_compute_pi(void *thread_number) {
    double num = *(double *)thread_number;
    int index = (int)num;

    int i;
    double x = STEP * num;
    double temp_sum = 0;
    for (i = index; i < NUM_POINTS; i += MAX_THREADS) {
        temp_sum += STEP*f(x);
        x += STEP * MAX_THREADS;
    }
    sum[index] = temp_sum;
    pthread_exit(NULL);
}

void* part3_compute_pi(void *thread_number) {
    double num = *(double *)thread_number;
    int index = (int)num;

    int i;
    double x = STEP * num;
    sum[index] = 0;
    for (i = index; i < NUM_POINTS; i += MAX_THREADS) {
        sum[index] += STEP*f(x);
        x += STEP * MAX_THREADS;
    }
    pthread_exit(NULL);
}


void* part1_compute_pi(void *thread_number) {
    double num = *(double *)thread_number;
    int index = (int)num;

    int i;
    double x = STEP * num;
    for (i = index; i < NUM_POINTS; i += MAX_THREADS) {
        pthread_mutex_lock(&mutex_lock);
        global_pi += STEP*f(x);
        pthread_mutex_unlock(&mutex_lock);
        x += STEP * MAX_THREADS;
    }
    pthread_exit(NULL);
}

void* part2_compute_pi(void *thread_number) {
    double num = *(double *)thread_number;
    int index = (int)num;

    int i;
    double x = STEP * num;
    for (i = index; i < NUM_POINTS; i += MAX_THREADS) {
        add_to_pi(STEP*f(x));
        x += STEP * MAX_THREADS;
    }
    pthread_exit(NULL);
}

void pi_parallel() {
    pthread_t p_threads[MAX_THREADS];
    pthread_attr_t attr;
    pthread_attr_init (&attr);
    pthread_mutex_init(&mutex_lock, NULL);

    uint64_t execTime;
    struct timespec start, stop;

    clock_gettime(CLOCK_MONOTONIC_RAW, &start);
    int i;
    for (i = 0; i < MAX_THREADS; ++i) {
        sum[i] = i;
        pthread_create(&p_threads[i], &attr, part3_compute_pi,(void *)&sum[i]);
    }
    double total_sum = 0;
    for (i = 0; i < MAX_THREADS; i++) {
        pthread_join(p_threads[i], NULL);
        total_sum += sum[i];
    }
    clock_gettime(CLOCK_MONOTONIC_RAW, &stop);

    execTime = 1000000000 * (stop.tv_sec - start.tv_sec) + stop.tv_nsec - start.tv_nsec;
    printf("elapsed process CPU time = %llu nanoseconds\n", (long long unsigned int) execTime);
    printf("%.20f\n",total_sum);
}

int main(int argc, char *argv[]) {
    pi_parallel();
    return 0;
}
