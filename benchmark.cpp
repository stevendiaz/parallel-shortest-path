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

double pi = 0.0;
pthread_mutex_t mutex_lock;

double f(double x) {
    return (6.0/sqrt(1-x*x));
}

void* compute_pi(void *thread_number) {
    int num = *(int *)thread_number;

    int i;
    double x = STEP * num;
    for (i = num; i < NUM_POINTS; i += MAX_THREADS) {
        pthread_mutex_lock(&mutex_lock);
        pi += STEP*f(x);
        pthread_mutex_unlock(&mutex_lock);
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
    int i = 0;
    for (i = 0; i < MAX_THREADS; i++) {
        int *thread_numbers = (int *)malloc(sizeof(*thread_numbers));
        *thread_numbers = i;
        pthread_create(&p_threads[i], &attr, compute_pi, thread_numbers);
        free(thread_numbers);
    }
    for (i = 0; i < MAX_THREADS; i++) {
        pthread_join(p_threads[i], NULL);
    }
    clock_gettime(CLOCK_MONOTONIC_RAW, &stop);

    execTime = 1000000000 * (stop.tv_sec - start.tv_sec) + stop.tv_nsec - start.tv_nsec;
    printf("elapsed process CPU time = %llu nanoseconds\n", (long long unsigned int) execTime);
    printf("%.20f\n", pi);
}

int main(int argc, char *argv[]) {
    //uint64_t execTime; /*time in nanoseconds */
    //struct timespec tick, tock;
    pi_parallel();


    //clock_gettime(CLOCK_MONOTONIC_RAW, &tick);

    //double x = 0.0;
    //for (int i = 0; i < numPoints; i++) {
    //    pi = pi + step*f(x);  // Add to local sum
    //    x = x + step;  // next x
    //}

    //clock_gettime(CLOCK_MONOTONIC_RAW, &tock);

    //execTime = 1000000000 * (tock.tv_sec - tick.tv_sec) + tock.tv_nsec - tick.tv_nsec;
 
    //printf("elapsed process CPU time = %llu nanoseconds\n", (long long unsigned int) execTime);

    //printf("%.20f\n", pi);
    return 0;
}
