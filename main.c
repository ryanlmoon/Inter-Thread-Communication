/**
 * CS3305 Assignment 3
 * Ryan Moon 251126575 22/Oct/21
 * 
 * This program takes two numbers as command line arguments and uses pipe()
 *  to pass the values between multiple threads
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>

int port[2];        // Our pipe
size_t PIPE_SIZE;   // Used for reading/writing 'X,Y' to/from pipe

/**
 * Takes X and Y from the command line and sums them into S
 */
void *sum(void *thread_id){
    int *id = (int*) thread_id;
    
    char fromPipe[PIPE_SIZE];   // Read string from pipe
    read(port[0], &fromPipe, PIPE_SIZE);
    
    char *token;    // Split string at delimiter
    token = strtok(fromPipe, ",");
    char *strx, *stry;
    strx = token;
    token = strtok(NULL, ",");
    stry = token;
    

    int x, y;   // Convert to integers
    x = atoi(strx);
    y = atoi(stry);
    printf("Thread (TID %d) reads X = %d and Y = %d from pipe\n", *id, x, y);

    int sum = (x + y);  // Sum and send onwards
    write(port[1], &sum, PIPE_SIZE);
    printf("Thread (TID %d) writes X + Y = %d to the pipe\n", *id, sum);
}

/**
 * Determines whether sum S is even or odd
 */
void *odd_even(void *thread_id){
    int *id = (int*) thread_id;
    
    int fromPipe;
    read(port[0], &fromPipe, PIPE_SIZE);
    printf("Thread (TID %d) reads X + Y = %d from the pipe\n", *id, fromPipe);
    
    if (fromPipe % 2 == 0){
        printf("Thread (TID %d) identifies X + Y = %d as an even number\n", *id, fromPipe);
    }
    else {
        printf("Thread (TID %d) identifies X + Y = %d as an odd number\n", *id, fromPipe);
    }

    write(port[1], &fromPipe, PIPE_SIZE);
}

/**
 * Counts the number of digits in sum S
 */
void *digit_count(void *thread_id){
    int *id = (int*) thread_id;

    int fromPipe;
    read(port[0], &fromPipe, PIPE_SIZE);
    printf("Thread (TID %d) reads X + Y = %d from the pipe\n", *id, fromPipe);
    
    // To find number of digits, we divide by 10 until the result is zero, while keeping the
    //  original value intact 
    int alsoFromPipe = fromPipe;
    int digits = 0;
    while (alsoFromPipe != 0) {
        alsoFromPipe /= 10;
        digits++;
    } 

    printf("Thread (TID %d) identifies X + Y = %d as a %d digit number\n", *id, fromPipe, digits);
}


int main (int argc, char *argv[]){
    
    pid_t pid = getpid();

     /* Ensure correct number of arguments supplied */
    if (argc > 3){
        printf("Too many arguments supplied. (2 Required)\n");
        exit(0);
    }
    else if (argc < 3){
        printf("Additional arguments expected. (2 Required)\n");
        exit(0);
    }
    
    /* Create pipe */
    if (pipe(port) < 0){
        printf("Error creating pipe\n");
        exit(0);
    }    

    /* Retrieve command line args */
    const char *argX, *argY;
    char *argXY;
    argX = argv[1];
    argY = argv[2];
    // Pipe size = size of command line args (+1)
    PIPE_SIZE = ((sizeof(char)*strlen(argX)) + (sizeof(char)*strlen(argY)) + 1);
    argXY = (char*)malloc(PIPE_SIZE);
    printf("Parent (PID %d) recieves X = %s and Y = %s from the user\n", pid, argX, argY);

    argXY = strcpy(argXY, argX);    // Concatenate X and Y values with ',' delimiter
    argXY = strcat(argXY, ","); 
    argXY = strcat(argXY, argY);
    write(port[1], argXY, PIPE_SIZE);   // "X,Y"
    printf("Parent (PID %d) writes X = %s and Y = %s to the pipe\n", pid, argX, argY);


    pthread_t thread0, thread1, thread2;
    int id0 = 100, id1 = 101, id2 = 102;

    pthread_create(&thread0, NULL, sum, &id0);
    pthread_join(thread0, NULL);
    
    pthread_create(&thread1, NULL, odd_even, &id1);
    pthread_join(thread1, NULL);

    pthread_create(&thread2, NULL, digit_count, &id2);
    pthread_join(thread2, NULL);

    free(argXY);
}
