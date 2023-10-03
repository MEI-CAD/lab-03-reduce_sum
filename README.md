# lab-03-reduce_sum

# Lab 03: OpenMP / CUDA reduction <!-- omit in toc -->

Computação de Alto Desempenho 2023-24

João Lourenço <joao.lourenco@fct.unl.pt>

Alex Davidson <a.davidson@fct.unl.pt>

October 3, 2023

<!-- ## Assignment link --> <!-- omit in toc -->

<!-- ### <https://classroom.github.com/a/Yk3GawQV> -->

## Table of Contents <!-- omit in toc -->

- [Overall goal](#overall-goal)
- [Setup](#setup)
- [Assignment](#assignment)
        - [Task 01: Testing the sequential code](#task-01)
        - [Task 02: Test the compiler optimizer](#task-02)
        - [Task 03: Parallelize with OpenMP and test](#task-03)
        - [Task 04: Parallelize with CUDA and test](#task-04)
        - [Task 05: Discussion](#task-05)


## Overall goal

In this laboratory, you will start from a simple C program that implements a reduction over a very large array (counting the number of 1 in the array), try the compiler's optimizations, and implement parallel versions in OpenMP and CUDA.

## Setup

You need to use a laptop with support for CUDA (either native or using the docker contained given in a past lab).  For performance analysis and evaluation, you need real CUDA hardware, so either run your program in your own CUDA-enabled hardware, or run it in the provided/allowed cluster nodes.

## Assignment

Consider the C program below.

```c
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <assert.h>
#include <time.h>
#include <sys/time.h>

#define SIZE 1024*1024L

// returns current (wall-clock) time in milliseconds, as a double
double wtime (void) {
  struct timeval timecheck;
  gettimeofday(&timecheck, NULL);
  return (double)timecheck.tv_sec * 1000 + (double)timecheck.tv_usec / 1000;
}

// initializes a vector with 0 in even positions and 1 in odd positions
// so the number of 1s will be "n/2"
void init_vector (int * const v, long n) {
  for (long i = 0; i < n; i++)
    v[i] = i % 2;
}

// makes v[0] = sum(v[i]) (0<=i<n)
void reduce_sum (int * const v, long n) {
  for (long i = 0; i < n; i++)
    v[0] = v[0] + v[i];
}

// Get size of the array from command line (size is in millions of items)
// Initialize the vector of the given size and report execution time
//       (both for wall clock time and CPU time)
// Count the number of 1s in the array and report execution time
//       (both for wall clock time and CPU time)
int main (int argc, const char **argv) {
  long size;
  if (argc > 1)
    size = SIZE * atol(argv[1]);
  else
    size = SIZE;  
  int *vect = (int *) calloc (size, sizeof (int));
  assert (vect != NULL);

  printf ("Initializing vector of size %ld\n", size);
  double t1 = wtime();
  clock_t t2 = clock();
  init_vector (vect, size);
  t2 = clock() - t2;
  t1 = wtime() - t1;
  double tf = ((double)t2/(double)CLOCKS_PER_SEC)*1000.0;
  printf("init time:   wall=%f ms    cpu=%f ms\n", t1, tf);
  
  printf ("\nRunning reduction\n");
  t1 = wtime();
  t2 = clock();
  reduce_sum (vect, size);
  t2 = clock() - t2;
  t1 = wtime() - t1;
  tf = ((double)t2/(double)CLOCKS_PER_SEC)*1000.0;
  printf(" ref time:   wall=%f ms    cpu=%f ms\n", t1, tf);
  
  if (vect[0] == size/2)
    printf("\n   OK sum: %i\n", vect[0]);
  else
    printf("ERROR sum: expected %li, counter %i\n", size/2, vect[0]);
}
```

### Task 1

Compile and test the given code for multiple values of size.  Find a size that takes more than 10 seconds and less than 30 to run.

To compile use the command below, that turns off code optimization, i.e., uses optimization level 0 (`-O0`):
`gcc -O0 -o sum sum.c`

### Task 2

Compile the same given code increasing the optimization level setp-by-step from 0 to 2.  For each optimization level register the execution time.

### Task 3

Parallelize the given code using OpenMP.  Compile it with `gcc -OX -o sum sum.c` increasing the optimization level (`X`) setp-by-step from 0 to 2.  For each optimization level register the execution time.

### Task 4

Parallelize the original given code  (sequential)  using CUDA.  Please try a basic parallelization, and then improve it is small steps.  For each optimization step register the execution time.

### Task 5

Discuss your observations on the code complexity and execution time for the multiples approaches.
