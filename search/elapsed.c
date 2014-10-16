#include "elapsed.h"
#include <stdio.h>

static struct timeval tstart, tend;

/* Subtract the ‘struct timeval’ values X and Y,
   storing the result in RESULT.
   Return 1 if the difference is negative, otherwise 0. */
static int timeval_subtract (result, x, y)
     struct timeval *result, *x, *y;
{
  /* Perform the carry for the later subtraction by updating y. */
  if (x->tv_usec < y->tv_usec) {
    int nsec = (y->tv_usec - x->tv_usec) / 1000000 + 1;
    y->tv_usec -= 1000000 * nsec;
    y->tv_sec += nsec;
  }
  if (x->tv_usec - y->tv_usec > 1000000) {
    int nsec = (x->tv_usec - y->tv_usec) / 1000000;
    y->tv_usec += 1000000 * nsec;
    y->tv_sec -= nsec;
  }

  /* Compute the time remaining to wait.
     tv_usec is certainly positive. */
  result->tv_sec = x->tv_sec - y->tv_sec;
  result->tv_usec = x->tv_usec - y->tv_usec;

  /* Return 1 if result is negative. */
  return x->tv_sec < y->tv_sec;
}

void time_reset() 
{
    if(gettimeofday(&tstart, NULL) == -1)
		printf("[time] gettimeofday error.\n");
}

double time_get() 
{
	struct timeval tres;
	
    if(gettimeofday(&tend, NULL) == -1)
		printf("[time] gettimeofday error.\n");
	timeval_subtract(&tres, &tend, &tstart);

	return (double)(tres.tv_sec) + 
	       ((double)tres.tv_usec) / 1000000.0;
}

#include <unistd.h>
void time_test()
{
	time_reset();
	sleep(3.14);
	printf("%.3f sec.\n", time_get());
	sleep(2.73);
	printf("%.3f sec.\n", time_get());
	time_reset();
	sleep(1.41);
	printf("%.3f sec.\n", time_get());
}

/*
int main()
{
	time_test();
	return 0;
}
*/
