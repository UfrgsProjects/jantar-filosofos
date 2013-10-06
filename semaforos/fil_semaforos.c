/****
*
* Algoritmo que implementa o algoritmo dos filósofos aplicado a semaforos.
	PRONTO E FUNCIONANDO
*
*****/
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <unistd.h>
 
#define RIGHT(i) (( (i) == 0 ) ? (i)+1 : (i) == numero_filosofos - 1 ? 0 : (i)+1)
#define LEFT(i) (( (i) == 0 ) ? numero_filosofos - 1 : (i) == numero_filosofos - 1 ? (i)-1 : (i)-1 )
#define TRUE 1

typedef enum Filosofo_Estado { THINKING, HUNGRY, EATING } filosofo_estado;

filosofo_estado *estado;
int numero_filosofos;
sem_t *semaforos;
sem_t mutex;
sem_t mutex_starvation;	

void imprime_estado(int id_filosofo){
	switch(estado[id_filosofo]){
		case 0: 
			printf("T");
		break;
		
		case 1: 
			printf("H");
		break;

		case 2: 
			printf("E");
		break;
	}
}


void show_estado(){
	int i = 0;
	//sem_wait(&mutex);		
	for(i = 0; i < numero_filosofos; i++)
		imprime_estado(i);
	printf("\n");
	//sem_post(&mutex);		
}

void think(int id_filosofo){
	show_estado(id_filosofo);
	sleep( (rand() % 10 + 1) );
}

void eat(int id_filosofo){
	show_estado(id_filosofo);
	sleep( (rand() % 10 + 1) );
}


void test(int id){
	if ( estado[id] == HUNGRY && 
         estado[LEFT(id)] != EATING &&
	     estado[RIGHT(id)] != EATING ){
			estado[id] = EATING;
			sem_post(&semaforos[id]);	
	}
}

// FILOSOFO TOMA GARFOS
void take_forks(int id_filosofo){
		
	//while( estado[id_filosofo] == HUNGRY){		
	sem_wait(&mutex);
	estado[id_filosofo] = HUNGRY;	
		/*		
		if( (estado[id_filosofo] == HUNGRY) && 
		    (estado[LEFT(id_filosofo)] != EATING) &&
		    (estado[RIGHT(id_filosofo)] != EATING)	){		
				estado[id_filosofo] = EATING;				
				sem_post(&semaforos[id_filosofo]);
		}
		*/
	show_estado(id_filosofo);
	test(id_filosofo);	
    sem_post(&mutex);			
	sem_wait(&semaforos[id_filosofo]);
	//}
	
}

// FILOSOFO SOLTA GARFOS
void drop_forks(int id_filosofo){
	sem_wait(&mutex);
		/*		
		if( estado[LEFT(id_filosofo - 1)] == HUNGRY ) 
			sem_post(&semaforos[LEFT(id_filosofo)]);
		if( estado[RIGHT(id_filosofo - 1)] == HUNGRY ) 
			sem_post(&semaforos[RIGHT(id_filosofo)]);		
		*/	
		estado[id_filosofo] = THINKING;
		test(LEFT(id_filosofo));
		test(RIGHT(id_filosofo));
	sem_post(&mutex);		
}


//	IMPLEMENTAÇÃO DOS FILOSOFOS
void *filosofo_work(void* arg){
	int id = (int) arg;
		
	while(TRUE){
		think(id);
		//sem_wait(&mutex_starvation);
			take_forks(id);
			eat(id);
			drop_forks(id);
		sem_post(&mutex_starvation);
	}

	return (NULL);
}

//Iniciliza semaforos: Mutex e de sincronização
void init_semaforo(int num){
	int i = 0;
	sem_init(&mutex, 0, 1); // mutex
	sem_init(&mutex_starvation,0,1);
	semaforos = (sem_t *) malloc( num * sizeof(sem_t) );	
	estado = (filosofo_estado *) malloc(num * sizeof(filosofo_estado));
	for(i = 0; i < num; i++)
		sem_init(&semaforos[i],0,0); // Sincronização de Condição
	for(i = 0; i < num; i++)
		estado[i] = THINKING;

}

int main(int argc, char *argv[]) {
	int i = 0;     
	pthread_t *threads;
		
	// Verifica se foi passado Parametro
	if (argc != 2) {
	   printf ("\n**************************************************\n\n");	
       printf ("Ops!! Informe um número inteiro, Ex: fil_semaforos 2\n");
	   printf ("\n**************************************************\n\n");	
       exit(1);
    }      

	numero_filosofos =  atoi(argv[1]); // numero de filosofos
    init_semaforo(numero_filosofos);
	threads = (pthread_t *) malloc(numero_filosofos * sizeof(pthread_t));

	// Cria threads de Filósofos
	for(i=0; i<numero_filosofos; i++)
		pthread_create(&threads[i], NULL, filosofo_work, (void *)(i));
	
    pthread_exit(0);


return 0;
}


