#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

int factor;

sem_t car_Load;
sem_t car_Unload;
sem_t seat_filled;
sem_t seat_empty;
sem_t Start_rides;
sem_t trip_complete;
sem_t temp;
sem_t irreg;

int capacity;
int temp_[];
int num_passengers;

void *car(void *param) {
    while(1>0){
    sem_wait(&Start_rides);
    load();
    sem_wait(&seat_filled);
    printf("\nNow starting the ride.\n");
    sleep(2); // ride duration
    printf("Ride is over.\n\n");
    unload();
    printf("\nSeats are empty now.\n");
    sem_post(&trip_complete);
    }
}

void load(){

    printf("The car is loaded\n\n");
    sem_post(&car_Load);

}

void unload(){

    sem_post(&car_Unload); // signal car unload
    sem_wait(&seat_empty);

}


void *passenger(int *param) {
    sem_wait(&car_Load);
    board(*param);
    sleep(2); // time to board
    int fer;
    sem_getvalue(&temp,&fer);
    if((fer%capacity)==1||capacity==1){
        sem_post(&seat_filled);
    }
    else {
        sem_post(&car_Load);
    }
    sem_wait(&car_Unload); // wait for car to unload
    unboard(*param);
    sleep(1); 
    sem_getvalue(&temp,&fer);
    if(fer==1){
        sem_post(&seat_empty);
    }
    else {
        sem_post(&car_Unload);
    }
}

void board(int param){
    printf("Passenger %d is boarding.\n", param);
    sem_post(&temp);
}

void unboard(int param){
    printf("Passenger %d is deboarding.\n", param);
    sem_wait(&temp);
}


void create_passenger_thread(int a){
    if(a==0){
        return;
    }
    pthread_t temp;
    temp_[a]=a;
    pthread_create(&temp, NULL, passenger, &temp_[a]);
    create_passenger_thread(a-1);
}

void start_trips(int a){
for(int i=a;i>0;i--){
    printf("--------------------------\n");
    if(i==1){
        if(num_passengers%capacity!=0){capacity=num_passengers%capacity;}
    }
    sem_post(&Start_rides);
    sem_wait(&trip_complete);
}
exit(0);
}


int main(int argc, char *argv[]) {

    printf("Enter the number of passengers - ");
    scanf("%d",&num_passengers);
    printf("Enter the capacity - ");
    scanf("%d",&capacity);

    if(num_passengers%capacity==0){
        factor=num_passengers/capacity;
    }
    else{
        factor=(num_passengers/capacity)+1;
    }

    pthread_t car_thread;

    sem_init(&car_Load, 0, 0);
    sem_init(&car_Unload, 0, 0);
    sem_init(&temp, 0, 1);
    sem_init(&seat_filled, 0, 0);
    sem_init(&seat_empty, 0, 0);
    sem_init(&irreg, 0, 1);
    sem_init(&Start_rides,0,0);
    sem_init(&trip_complete,0,0);

    pthread_create(&car_thread, NULL, car, NULL);

    create_passenger_thread(num_passengers);
    start_trips(factor);

    pthread_join(car_thread, NULL);
    return 0;
}