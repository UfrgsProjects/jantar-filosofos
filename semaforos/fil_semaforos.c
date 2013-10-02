#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
 
#define N 5
/* Number of philosphers */
#define RIGHT(i) (((i)+1) %N)
#define LEFT(i) (((i)==N) ? 0 : (i)+1)

typedef enum { THINKING, HUNGRY, EATING } phil_state;

int n;
pthread_mutex_t mutex;
sem_t semaforo[N];

int main(int argc, char *argv[]) {
     pthread_t cons;
	 n = argv[0]; // numero de filosofos
	
     pthread_exit(0);
 }

