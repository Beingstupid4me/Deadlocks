#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define NUM_PHILOSOPHERS 5
#define NUM_FORKS 5
#define NUM_BOWLS 2

pthread_mutex_t forks[NUM_FORKS];
pthread_mutex_t bowls[NUM_BOWLS];
pthread_cond_t can_eat[NUM_PHILOSOPHERS];

// Counter arrays to keep track of each philosopher's actions
int thought_count[NUM_PHILOSOPHERS] = {0};
int eat_count[NUM_PHILOSOPHERS] = {0};

void thinking(int philosopher_id) {
    printf("Philosopher %d is thinking.\n", philosopher_id);
    sleep(rand() % 3);  // Simulate thinking
    thought_count[philosopher_id]++;
}

void eating(int philosopher_id) {
    printf("Philosopher %d is eating.\n", philosopher_id);
    sleep(rand() % 3);  // Simulate eating
    eat_count[philosopher_id]++;
}

void printSummary() {
    printf("\nPhilosopher Summary:\n");
    for (int i = 0; i < NUM_PHILOSOPHERS; ++i) {
        printf("Philosopher %d: Thought %d times, Ate %d times.\n", i, thought_count[i], eat_count[i]);
    }
    printf("\n");
}

void* printSummaryThread(void* args) {
    while (1) {
        sleep(5); // Adjust the interval as needed
        pthread_mutex_lock(&forks[0]); // Lock to ensure consistency in output
        printSummary();
        pthread_mutex_unlock(&forks[0]);
    }
}

void* deadlockCheckThread(void* args) {
    while (1) {
        sleep(1); // Check for deadlock every second

        int deadlock_detected = 2;

        for (int i = 0; i < NUM_FORKS; ++i) {
            if (pthread_mutex_trylock(&forks[i]) == 0) {
                printf("                                           Fork no. %d is free\n",i);
                pthread_mutex_unlock(&forks[i]);
                deadlock_detected = 0;
                break;
            }
        }

        // for (int i = 0; i < NUM_BOWLS; ++i) {
        //     if (pthread_mutex_trylock(&bowls[i]) == 0) {
        //         pthread_mutex_unlock(&bowls[i]);
        //         printf("                                           Bowl no. %d is free\n",i);
        //         deadlock_detected -= 1;
        //         break;
        //     }
        // }

        if (deadlock_detected) {
            printf("\nDeadlock detected! Releasing all forks and bowls.\n\n");

            // Release all forks
            for (int i = 0; i < NUM_FORKS; ++i) {
                pthread_mutex_unlock(&forks[i]);
            }

            // Release all bowls
            for (int i = 0; i < NUM_BOWLS; ++i) {
                pthread_mutex_unlock(&bowls[i]);
            }
        }
    }
}

void* philosopher(void* args) {
    int philosopher_id = *((int*)args);
    int right_fork = philosopher_id;
    int left_fork = (philosopher_id + 1) % NUM_FORKS;

    while (1) {
        thinking(philosopher_id);

        // Acquire forks
        pthread_mutex_lock(&forks[left_fork]);
        printf("Fork no. %d is with Philosopher %d\n", left_fork, philosopher_id);
        pthread_mutex_lock(&forks[right_fork]);
        printf("Fork no. %d is with Philosopher %d\n", right_fork, philosopher_id);

        // Acquire any available bowl
        int bowl = -1;
        for (int i = 0; i < NUM_BOWLS; ++i) {
            if (pthread_mutex_trylock(&bowls[i]) == 0) {
                bowl = i;
                printf("Bowl no. %d is with Philosopher %d\n", bowl, philosopher_id);
                // Signal that philosopher can eat
                pthread_cond_signal(&can_eat[philosopher_id]);

                eating(philosopher_id);
                // Simulate philosopher eating
                // sleep(1);
                break;
            }
        }

        // Release forks and bowl
        pthread_mutex_unlock(&forks[left_fork]);
        pthread_mutex_unlock(&forks[right_fork]);
        if (bowl != -1) {
            pthread_mutex_unlock(&bowls[bowl]);
        }
    }

    pthread_exit(NULL);
}

int main() {
    pthread_t philosophers[NUM_PHILOSOPHERS];
    pthread_t summaryThread;
    pthread_t deadlockCheck;

    int philosopher_ids[NUM_PHILOSOPHERS];

    srand(time(NULL));

    // Initialize mutexes and conditions
    for (int i = 0; i < NUM_FORKS; ++i) {
        pthread_mutex_init(&forks[i], NULL);
    }

    for (int i = 0; i < NUM_BOWLS; ++i) {
        pthread_mutex_init(&bowls[i], NULL);
    }

    for (int i = 0; i < NUM_PHILOSOPHERS; ++i) {
        pthread_cond_init(&can_eat[i], NULL);
    }

    // Create philosopher threads
    for (int i = 0; i < NUM_PHILOSOPHERS; ++i) {
        philosopher_ids[i] = i;
        pthread_create(&philosophers[i], NULL, philosopher, &philosopher_ids[i]);
    }

    // Create thread for printing summary
    pthread_create(&summaryThread, NULL, printSummaryThread, NULL);

    // Create thread for deadlock check
    pthread_create(&deadlockCheck, NULL, deadlockCheckThread, NULL);


    // Join threads
    for (int i = 0; i < NUM_PHILOSOPHERS; ++i) {
        pthread_join(philosophers[i], NULL);
    }

    // Join summary thread
    pthread_join(summaryThread, NULL);

    // Join deadlock check thread
    pthread_join(deadlockCheck, NULL);

    // Print final summary
    printSummary();

    return 0;
}
