#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>

#define MAX_CARS 5
static int Something;
static int RightCars; // Number of cars coming from the right
static int LeftCars;  // Number of cars coming from the left
static bool a = true;  // Unused boolean variable
static double f = 400000; // Sleep duration (in seconds)
// const char* leftMessage[] = {"Car", "from", "the", "left", "is", "passing"};
// const char* rightMessage[] = {"Car", "from", "the", "right", "is", "passing"};

sem_t mutex, leftSem, rightSem; // Semaphores for synchronization

// Function to be executed by the thread representing cars coming from the left
void* left(void* args) {
    while (LeftCars > 0) {
        sem_wait(&leftSem);
        sem_wait(&mutex);

        if (LeftCars >= MAX_CARS) {
            // If there are more cars than MAX_CARS, let MAX_CARS pass
            for (int j = 0; j < 5; j++) {
                printf("=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>\n");
                printf("Car no. %d from Left is on the Bridge\n",LeftCars--);
                printf("=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>\n");
                printf("\n");
                usleep(f);usleep(f);
            }
            for(int i = 0;i<10;i++){
                printf(".");fflush(stdout);;usleep(f);
            }
            // rollin();
            printf("\n");
            for (int j = 0; j < 5; j++) {
                printf("\033[0m=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>\n");
                printf("Car no. %d from Left is off of the Bridge\n",LeftCars+5-j);
                printf("=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>\n");
                printf("\n");
                usleep(f);
            }
            usleep(f);usleep(f);

            
        } else {
            // If there are fewer cars than MAX_CARS, let all remaining cars pass
            for (int j = 0; j < LeftCars; j++) {
                printf("=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>\n");
                printf("Car no. %d from Left is on the Bridge\n",j+1);
                printf("=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>\n");
                printf("\n");
                usleep(f);
            }
            for(int i = 0;i<10;i++){
                printf(".");fflush(stdout);;usleep(f);
            }
            printf("\n");
            for (int j = 0; j < LeftCars; j++) {
                printf("=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>\n");
                printf("Car no. %d from Left is off of the Bridge\n",j+1);
                printf("=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>\n");
                printf("\n");
                usleep(f);
            }
            usleep(f);usleep(f);
            LeftCars = 0;
        }
        
        
        // printf("\n");
        printf("\e[1m____________________\n");
        printf("Left Cars left :: %d\n", LeftCars);
        printf("Right Cars left:: %d\n", RightCars);
        printf("\e[m▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔\n");
        usleep(f);

        sem_post(&mutex);
        sem_post(&rightSem);
    }

    pthread_exit(NULL);
}

// Function to be executed by the thread representing cars coming from the right
void* right(void* args) {
    while (RightCars > 0) {
        sem_wait(&rightSem);
        sem_wait(&mutex);

         if (RightCars >= MAX_CARS) {
            // If there are more cars than MAX_CARS, let MAX_CARS pass
            for (int j = 0; j < 5; j++) {
                printf("<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=\n");
                printf("Car no. %d from Right is on the Bridge\n",RightCars--);
                printf("<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=\n");
                printf("\n");
                usleep(f);usleep(f);
            }
            // rolling_right();
            for(int i = 0;i<10;i++){
                printf(".");fflush(stdout);;usleep(f);
            }
            printf("\n");
            for (int j = 0; j < 5; j++) {
                printf("<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=\n");
                printf("Car no. %d from Right is off of the Bridge\n",RightCars+5-j);
                printf("<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=\n");
                printf("\n");
                usleep(f);
            }
            usleep(f);usleep(f);

            
        } else {
            // If there are fewer cars than MAX_CARS, let all remaining cars pass
            Something = RightCars;
            for (int j = 0; j < RightCars; j++) {
                printf("<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=\n");
                printf("Car no. %d from Right is on the Bridge\n",j+1);
                printf("<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=\n");
                printf("\n");
                usleep(f);
            }
            for(int i = 0;i<10;i++){
                printf(".");fflush(stdout);;usleep(f);
            }
            printf("\n");
            for (int j = 0; j < RightCars; j++) {
                printf("<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=\n");
                printf("Car no. %d from Right is off of the Bridge\n",j+1);
                printf("<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=\n");
                printf("\n");
                usleep(f);
            }
            usleep(f);usleep(f);
            RightCars = 0;
        }
        // usleep(f);

        printf("\e[1m____________________\n");
        printf("Left Cars left :: %d\n", LeftCars);
        printf("Right Cars left:: %d\n", RightCars);
        printf("\e[m▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔\n");

        if(RightCars==0){
            pthread_exit(NULL);
        }
        usleep(f);

        sem_post(&mutex);
        sem_post(&leftSem);
    }

    pthread_exit(NULL);
}

// Function to be executed by the monitoring thread
void* monitorCars(void* args) {
    while (true) {
        // If there are no cars on either side, terminate the program
        if (LeftCars == 0 && RightCars == 0) {
            sem_post(&mutex);
            sem_post(&rightSem);
            sem_post(&leftSem);
            pthread_exit(NULL);
        }

        // If there are no cars on the left, release the right semaphore
        if (LeftCars == 0) {
            sem_post(&mutex);
            sem_post(&rightSem);
        }

        // If there are no cars on the right, release the left semaphore
        if (RightCars == 0) {
            printf("Yes");
            sem_post(&mutex);
            sem_post(&leftSem);
        }

        usleep(100000); // Sleep for 100ms to avoid busy-waiting
    }

    pthread_exit(NULL);
}

int main() {
    pthread_t leftThread, rightThread, monitorThread;

    // Input the number of cars on the left and right sides
    printf("Enter the number of cars on the left side: ");
    scanf("%d", &LeftCars);

    printf("Enter the number of cars on the right side: ");
    scanf("%d", &RightCars);

    // Initialize semaphores
    sem_init(&mutex, 0, 1);
    sem_init(&leftSem, 0, 1);
    sem_init(&rightSem, 0, 0);

    // Create threads for left, right, and monitor functions
    pthread_create(&leftThread, NULL, left, NULL);
    pthread_create(&rightThread, NULL, right, NULL);
    pthread_create(&monitorThread, NULL, monitorCars, NULL);

    // Wait for threads to finish
    pthread_join(leftThread, NULL);
    pthread_join(rightThread, NULL);
    pthread_join(monitorThread, NULL);

    // Destroy semaphores
    sem_destroy(&mutex);
    sem_destroy(&leftSem);
    sem_destroy(&rightSem);

    return 0;
}