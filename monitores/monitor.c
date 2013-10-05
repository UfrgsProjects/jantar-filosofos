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

	int left = id;				// garfo da esquerda
	int right = (1+id)%n;			// garfo da direita
  
	philosopher[id] = HUNGRY;
	pthread_mutex_lock(&mutex);
		
		//printf("id: %d Sessao critica take \n", id);
		//printf("H");
		printPhil(id);

		while(forks[right] == IN_USE || forks[left] == IN_USE){
				pthread_cond_wait(&cond[id], &mutex);
		}


		
		philosopher[id] = EATING;
  		forks[left]		= IN_USE;
  		forks[right]	= IN_USE;
		//printf("id: %d Comendo \n", id);
		//printPhil(id);
  	pthread_mutex_unlock(&mutex);
  	printPhil(id);
//printf("id: %d take fim \n", id);
}

void drop_forks(int id){
	
  int left = id;		// garfo da esquerda
  int right = (1+id)%n;	// garfo da direita
  

	pthread_mutex_lock(&mutex);
		//printf("id: %d Sessao critica drop \n", id);
		forks[left]		= FREE;
		forks[right]	= FREE;
		printPhil(id);
		philosopher[id] = THINKING;
		//printf("T");
		//printf("id: %d Acordando %d e %d \n", id, left, right);
		/*Acordando os filosofos da esquerda e direita*/
		pthread_cond_signal(&cond[left]);
    	pthread_cond_signal(&cond[right]);
    	printPhil(id);
    	//printf("id: %d Acordando %d e %d sucesso \n", id, left, right);
	pthread_mutex_unlock(&mutex);
//printf("id: %d drop fim \n", id);
printPhil(id);
}


void *philosopherAction(void *arg){
	
	int id = (int) arg;
	while(1){

		sleep((rand() % 20 + 1));
		//printPhil(id);
		take_forks(id);
		sleep((rand() % 20 + 1));
		//printPhil(id);
		drop_forks(id);
		printPhil(id);
		//printPhil(id);
		//printf("um laco \n");
		//
	}
	pthread_exit(0);
}

void printPhil(int id){
	int i;
	for (i=0;i<n;i++){
			if(philosopher[i] == THINKING)
			printf(" T ");
			if(philosopher[i] == HUNGRY)
			printf(" H ");
			if(philosopher[i] == EATING)
			printf(" E ");
	}
	printf("\n");
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

	printf("**** %d THREADS **** \n", n);
	/* Inicializações */
	threads		= (pthread_t*) malloc(n * sizeof(pthread_t*));
	philosopher	= (phil_states *) malloc(n * sizeof(phil_states));
	cond		= (pthread_cond_t *) malloc(n * sizeof(pthread_cond_t));
	forks		= (int*) malloc(n * sizeof(int*));
	pthread_mutex_init(&mutex, NULL);
	
	for(i=0;i<n;i++){
			philosopher[i] = THINKING;
			forks[i] = FREE;
	}
	
	for	(i=0;i<n;i++){
			pthread_cond_init(&cond[i], NULL);
			pthread_create(&threads[i], NULL, philosopherAction, (void *)(i));;
	}
	
	pthread_exit(0);
return 0;
}
