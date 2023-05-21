#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define NUM_PHILOSOPHERS 5

enum {
    LEFT,
    RIGHT
};

enum {
    THINKING,
    HUNGRY,
    EATING
};

int times_eaten[NUM_PHILOSOPHERS]; //global variable to keep track of how many times each philosopher has eaten to check for starvation
int state[NUM_PHILOSOPHERS]; //global variable to keep track of the state of each philosopher
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond[NUM_PHILOSOPHERS];

int left(int i) {
    return i;
}

int right(int i) {
    return (i + 1) % NUM_PHILOSOPHERS;
}

void test(int i) {
    if(times_eaten[i] <= times_eaten[left(i)] && times_eaten[i] <= times_eaten[right(i)]) { //check if the philosopher has eaten less than his neighbors
        if (state[i] == HUNGRY && state[left(i)] != EATING && state[right(i)] != EATING) {
            state[i] = EATING;
            pthread_cond_signal(&cond[i]);
        }
    }
}

void pickup(int i) {
    pthread_mutex_lock(&mutex);
    state[i] = HUNGRY;
    test(i);

    if (state[i] != EATING) {
        pthread_cond_wait(&cond[i],&mutex);
    }

    pthread_mutex_unlock(&mutex);
}

void putdown(int i) {
    pthread_mutex_lock(&mutex);
    state[i] = THINKING;
    test(left(i));
    test(right(i));

    pthread_cond_signal(&cond[left(i)]); // Signal left neighbor
    pthread_cond_signal(&cond[right(i)]); // Signal right neighbor

    pthread_mutex_unlock(&mutex);
}

void *philosopher(void *arg) {
    int id = *(int *)arg;
    while (1) {
        printf("Philosopher %d is thinking\n", id);
        sleep(rand() % 5 + 1); // Thinking for some time
        printf("Philosopher %d is hungry\n", id);
        pickup(id);
        printf("Philosopher %d is eating\n", id);
        times_eaten[id]++;
        printf("Philosopher %d has eaten\n", id);
        printf("%d | %d | %d | %d | %d\n", times_eaten[0], times_eaten[1], times_eaten[2], times_eaten[3], times_eaten[4]); // <- check for starvation
        sleep(rand() % 5 + 1); // Eating for some time
        putdown(id);
    }
    return NULL;
}

int main() {
    int i, id[NUM_PHILOSOPHERS];
    pthread_t tid[NUM_PHILOSOPHERS];
    for (i = 0; i < NUM_PHILOSOPHERS; i++) {
        state[i] = THINKING;
        pthread_cond_init(&cond[i], NULL);
        id[i] = i;
    }
    for (i = 0; i < NUM_PHILOSOPHERS; i++) {
        pthread_create(&tid[i], NULL, philosopher, &id[i]);
    }
    for (i = 0; i < NUM_PHILOSOPHERS; i++) {
        pthread_join(tid[i], NULL);
    }
    return 0;
}
