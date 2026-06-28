/*
 * Projet 12 - Version THREADS
 * Chaque thread travaille sur son propre tableau local (malloc).
 * Accès aléatoires pour neutraliser le prefetcher.
 *
 * Compilation : gcc -O0 -pthread -o threads_cache threads_cache.c
 * Exécution   : taskset -c 0 perf stat -e cache-misses,cache-references ./threads_cache
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define ARRAY_SIZE 20000000   /* ~80 Mo > cache L2 */
#define ITERATIONS 5
#define YIELD_FREQ  1000

/* ------------------------------------------------------------------ */
/*  Tâche commune : chaque thread alloue et accède à son propre       */
/*  tableau → pas de race condition, comparaison équitable            */
/*  avec la version processus                                         */
/* ------------------------------------------------------------------ */
void do_work(int thread_id) {
    int *array = (int *)malloc(ARRAY_SIZE * sizeof(int));
    if (!array) {
        perror("malloc");
        exit(1);
    }

    volatile long sum = 0;

/* Écriture séquentielle → tout le tableau est rempli à coup sûr */
    for (int i = 0; i < ARRAY_SIZE; i++) {
        array[i] = thread_id + i;
        int idx = rand() % ARRAY_SIZE;
        sum += array[idx];
        if (i%YIELD_FREQ == 0 ){
            sched_yield();
        }

}

/* Lecture aléatoire → neutralise le prefetcher */
    if (sum == 0) printf("(sum inattendue)\n");

    free(array);
}

/* ------------------------------------------------------------------ */
/*  Fonction de chaque thread                                         */
/* ------------------------------------------------------------------ */
void *thread_func(void *arg) {
    int id = *(int *)arg;

    for (int iter = 0; iter < ITERATIONS; iter++) {
        printf("[Thread %d] Iteration %d - debut\n", id, iter + 1);
        do_work(id);
        printf("[Thread %d] Iteration %d - fin\n", id, iter + 1);
    }

    return NULL;
}

/* ------------------------------------------------------------------ */
/*  Main                                                              */
/* ------------------------------------------------------------------ */
int main(void) {
    pthread_t t1, t2;
    int id1 = 1, id2 = 2;

    printf("=== Version THREADS ===\n");
    printf("Taille tableau : %d entiers par thread\n", ARRAY_SIZE);
    printf("Iterations     : %d\n\n", ITERATIONS);

    pthread_create(&t1, NULL, thread_func, &id1);
    pthread_create(&t2, NULL, thread_func, &id2);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    printf("\n[OK] Termine.\n");
    return 0;
}
