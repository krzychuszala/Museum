#include <stdio.h>
#include <sys/resource.h> // getpriority
#include <sched.h> // scheduling functions
#include <semaphore.h> // sem_t
#include <time.h> // rand()
#include <stdlib.h>  // srand()
#include <pthread.h> // pthread_join()
#include <unistd.h> // sleep, nice

// These variables are supposed to be change

#define AmountOfPeople 30
#define Na 5
#define Nb 3     //  (Nb < Na)!!!
#define maxA 5000000  // max time for watching exhibition A
#define maxB 500000  // microseconds, 1000000 = 1 second


sem_t A;
sem_t B; // counter of people in hall
sem_t Dead; // to prevent deadlock situation

typedef struct Person
{
	unsigned int id; // threads id
	unsigned int watchA; // time for watching A
	int goB; // does he want to go to hall B or not
	unsigned int watchB; // time for hall B
}Person;


void* hallB(void*arg);

void* hallA(void*arg)
{
	Person*x = (Person*)arg;
	x->watchA = rand()%maxA;
	int l = rand()%2;
	x->goB = l;

	if(x->goB == 1)
	{
		int inc = 19;
		int check = setpriority(PRIO_PROCESS, 0, inc);
	}

	sem_wait(&Dead);
	sem_wait(&A);

	printf("Zwiedzanie A nr%d \n", x->id);

	usleep(x->watchA);// sightseeing hall A
	x->watchA = 0;

	if(x->goB == 1)
	{
		sem_wait(&B);
		sem_post(&A);
		pthread_t threadB;
		pthread_create(&threadB, NULL, hallB, arg);
		pthread_join(threadB,NULL);
	}

	sem_post(&A);
	sem_post(&Dead);
	printf("Wychodze z A nr%d \n", x->id);
	return NULL;
}

void*hallB(void *arg)
{
	Person*x = (Person*)arg;
	x->watchB = rand()%maxB;

	printf("Zwiedzanie B nr%d \n", x->id);
	usleep(x->watchB);// sightseeing hall B
	x->watchB = 0;
	x->goB--;

		int a,b;
		sem_getvalue(&A,&a);
		sem_getvalue(&B,&b);
		if(a == 0 && b == 0)
		{
			printf("Deadlock\n");
		}

	sem_wait(&A);
	sem_post(&B);
	printf("Wychodze z B nr%d \n", x->id);
	return NULL;
}

int main()
{
	srand(time(NULL));

	if(Nb >= Na)
	{ printf("Na has to be set greater than Nb\n");}

	sem_init(&A,0,Na);
	sem_init(&B,0,Nb);
	sem_init(&Dead,0,(Na+Nb-1));

	pthread_t newthread[AmountOfPeople];

	Person tab[AmountOfPeople];
//----------------------------------------Initializing visitors------------------
	for(int i=0;i<AmountOfPeople;i++)
	{
		tab[i].id = i;
	}

//--------------------------------Calling threads--------------------
	for(int i=0;i<AmountOfPeople;i++)
	{
		pthread_create(&newthread[i], NULL, hallA, &tab[i]);
	}


	for(int i=0;i<AmountOfPeople;i++) // Assuring that everybody left museum
	{
		pthread_join(newthread[i], NULL);
	}

	printf("\n");
}
