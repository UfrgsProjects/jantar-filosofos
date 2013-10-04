#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#define IN_USE 1
#define FREE 0

typedef enum philosopher_states { THINKING, HUNGRY, EATING } phil_states;

int alive = 1;
int n; //numero de filosofos
pthread_mutex_t mutex;
pthread_cond_t *cond;
int *forks;

phil_states *philosopher; // array de filosofos

void take_forks(int id){

  int left = id; 				// garfo da esquerda
  int right = (1+id)%5; // garfo da direita

	pthread_mutex_lock(&mutex);
		philosopher[id] = HUNGRY;
		printf("id: %d H \n", id);
		while(forks[right] == IN_USE || forks[left] == IN_USE)
				pthread_cond_wait(&cond[id], &mutex);

		philosopher[id] = EATING;
  	forks[left]		  = IN_USE;
  	forks[right]    = IN_USE;
		printf("E \n");

	
  pthread_mutex_unlock(&mutex);

}

void drop_forks(int id){
	
  int left = id; 				// garfo da esquerda
  int right = (1+id)%5; // garfo da direita
	
	pthread_mutex_lock(&mutex);
		forks[left] 	= FREE;
		forks[right]  = FREE;
	
		philosopher[id] = THINKING;

		/*Acordando os filosofos da esquerda e direita*/
		pthread_cond_signal(&cond[left]);
    pthread_cond_signal(&cond[right]);
	pthread_mutex_unlock(&mutex);
}


void *philosopherAction(void *arg){
	
	int id = (int) arg;

	pthread_exit(0);
}



int main (int argc, const char * argv[]){

	pthread_t* threads;
	int i,j;

		if (argc != 2) {
					printf ("\n**************************************************\n\n");	
       		printf ("Ops!! Informe um número inteiro, Ex: fil_semaforos 2\n");
					printf ("\n**************************************************\n\n");	
   	exit(1);
    } 

	n = atoi(argv[1]);

	/* Inicializações */
	threads		= (pthread_t*) malloc(n * sizeof(pthread_t*));
	philosopher	= (phil_states *) malloc(n * sizeof(phil_states));
	cond		= (pthread_cond_t *) malloc(n * sizeof(pthread_cond_t));
	forks		= (int*) malloc(n * sizeof(int*));
	pthread_mutex_init(&mutex, NULL);
	

	for	(i=0;i<n;i++){
			philosopher[i] = THINKING;
			forks[i] = FREE;
			pthread_cond_init(&cond[i], NULL);
			pthread_create(&threads[i], NULL, philosopherAction, (void *)(i));;
	}

return 0;
}
