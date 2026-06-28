/*

 * Chaque processus travaille sur son propre tableau local (malloc).
 * Accès aléatoires pour neutraliser le prefetcher.
 *
 * Compilation : gcc -O0 -o process_cache process_cache.c
 * Exécution   : taskset -c 0 perf stat -e cache-misses,cache-references ./process_cache
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define ARRAY_SIZE 10000000   /* ~80 Mo > cache L2 */
#define ITERATIONS 5
#define YIELD_FREQ 1


/*  Tâche commune : identique à la version threads                    */
/*  Chaque processus a son propre espace mémoire (séparation totale)  */

void do_work(int proc_id) {
    int *array = (int *)malloc(ARRAY_SIZE * sizeof(int));
    if (!array) {
        perror("malloc");
        exit(1);
    }

    volatile long sum = 0;

/* Écriture séquentielle  */
    for (int i = 0; i < ARRAY_SIZE; i++) {
        array[i] = proc_id + i;
        int idx = rand() % ARRAY_SIZE;
        sum += array[idx];
        if (i%YIELD_FREQ == 0 ){
            sched_yield();
        }
}

    if (sum == 0) printf("(sum inattendue)\n");

    free(array);
}


/*  Main                                                              */

int main(void) {
    printf("=== Version PROCESSUS ===\n");
    printf("Taille tableau : %d entiers par processus\n", ARRAY_SIZE);
    printf("Iterations     : %d\n\n", ITERATIONS);

    pid_t pid = fork();

    if (pid < 0) {
        perror("fork");
        return 1;

    } else if (pid == 0) {
        /* -------- PROCESSUS FILS -------- */
        for (int iter = 0; iter < ITERATIONS; iter++) {
            printf("[Fils  PID=%d] Iteration %d - debut\n", getpid(), iter + 1);
            do_work(2);
            printf("[Fils  PID=%d] Iteration %d - fin\n", getpid(), iter + 1);
        }
        exit(0);

    } else {
        /* -------- PROCESSUS PÈRE -------- */
        for (int iter = 0; iter < ITERATIONS; iter++) {
            printf("[Pere  PID=%d] Iteration %d - debut\n", getpid(), iter + 1);
            do_work(1);
            printf("[Pere  PID=%d] Iteration %d - fin\n", getpid(), iter + 1);
        }

        wait(NULL);
    }

    printf("\n[OK] Termine.\n");
    return 0;
}
