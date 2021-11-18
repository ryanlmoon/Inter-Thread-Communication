# Inter-Thread Communication
Copyright (c) 2021 Ryan Moon

## About
This is a simple C program meant to demonstrate the following:
- Using _pipe_ and _pthread_ system functions
- Creating multiple threads for different tasks
- Sharing data between threads using the pipe

## Usage
This program accepts two integers as command-line arguments (for
example, X and Y). 

Simply compile the program and run it with two integers to observe the following:

- The parent process will read X and Y from the command line. 
- The parent process will create three threads with corresponding thread ID 100, 101, and 102, respectively. 
- The parent process will write X and Y to the shared memory using pipe. 
- The first thread (i.e., thread 100) will read X and Y from the pipe and perform the sum, S = X+Y, and then the result S will be written to the pipe.
- Next, the second thread (i.e., thread 101) will read the S from the pipe and determine whether S is an even or odd number, and then S will be written again to the pipe by the second thread.
- Finally, the third thread (i.e., thread 102) will read S from the pipe and count the total number of digits in S.