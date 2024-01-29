# Car Ride System

This is a multithreaded C program that simulates a car ride system. The car and passengers are represented as separate threads. Semaphores are used to synchronize the actions of the car and passengers.

## Code Explanation

### Semaphores

The program uses several semaphores to control the flow of the program:

- `car_Load`: Controls when passengers can start boarding the car.
- `car_Unload`: Controls when passengers can start leaving the car.
- `seat_filled`: Signals when all seats in the car are filled.
- `seat_empty`: Signals when all seats in the car are empty.
- `Start_rides`: Signals when the car can start the ride.
- `trip_complete`: Signals when the trip is complete.
- `temp`: Used to synchronize the boarding and unboarding of passengers.
- `irreg`: Not used in the program.

### Threads

The program uses two types of threads:

- `car`: Represents the car. It waits for the ride to start, loads passengers, waits for all seats to be filled, starts the ride, and then unloads passengers.
- `passenger`: Represents a passenger. It waits for the car to load, boards the car, waits for the car to unload, and then leaves the car.

### Functions

The program includes several functions:

- `load()`: Called by the car to signal that it is ready to load passengers.
- `unload()`: Called by the car to signal that it is ready to unload passengers.
- `board(int param)`: Called by a passenger to board the car.
- `unboard(int param)`: Called by a passenger to leave the car.
- `create_passenger_thread(int a)`: Creates passenger threads.
- `start_trips(int a)`: Starts the trips.

### Main Function

The `main` function initializes the semaphores, creates the car thread, creates the passenger threads, starts the trips, and then waits for the car thread to finish.

## Usage

To use this program, you need to enter the number of passengers and the capacity of the car when prompted. The program will then simulate the car rides. Each ride lasts for 2 seconds, and it takes 2 seconds for a passenger to board the car and 1 second to leave the car. The program will continue running until all passengers have taken a ride. If the number of passengers is not a multiple of the car's capacity, the last ride will have fewer passengers. The program will print messages to the console to indicate what is happening. 

## Note

This program uses the POSIX thread and semaphore libraries, so it needs to be linked with the `-pthread` option when compiling. For example, you can compile it with the following command:

```bash
gcc -o Q2 Q2.c -pthread -lrt
```

You can then run the program with the following command:

```bash
./Q2
```

Or

```bash
make Q2 
```
if not changed.