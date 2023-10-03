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