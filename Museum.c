#include <stdio.h>
#include <sched.h> // scheduling functions
#include <semaphore.h> // sem_t
#include <time.h> // rand()
#include <stdlib.h>  // srand()
#include <pthread.h> // pthread_join()
#include <unistd.h> // sleep
#define AmountOfPeople 30
#define Na 4
#define Nb 3     //  (Nb < Na)!!!
#define maxA 5000000  // jak długo zajmię maksymalnie oglądanie wystawy A
#define maxB 500000  // mikrosekundy, czyli 1000000 = 1 sekunda
#define addPace 500 // co ile czasu próbuje znów dodać kolejny proces


sem_t A;
sem_t B; // counter of people in hall

typedef struct Person
{
	unsigned int id; // nr procesu w programie
	unsigned int watchA; // jak długo będzie oglądał hale A
	int goB; // czy chce isć do hali B
	unsigned int watchB; // jak długo B
}Person;

void* hallB(void*arg);

void* hallA(void*arg)
{
	sem_wait(&A);
	Person*x = (Person*)arg;

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
	printf("Wychodze z A nr%d \t", x->id);
	return NULL;
}

void*hallB(void *arg)
{
	Person*x = (Person*)arg;

	printf("Zwiedzanie B nr%d \n", x->id);
	usleep(x->watchB);// sightseeing hall B
	x->watchB = 0;

	int w;
	sem_getvalue(&B,&w);
	x->goB--;


		int a,b;
		sem_getvalue(&A,&a);
		sem_getvalue(&B,&b);
		printf("%d %d \n",a,b);
		if(a == 0 && b == 0)
		{
			printf("Deadlock\n");
		}

	sem_wait(&A);
	sem_post(&B);
	printf("Wychodze z B nr%d \t", x->id);
	return NULL;
}

int main()
{
	srand(time(NULL));

	sem_init(&A,0,Na);
	sem_init(&B,0,Nb);

//	sched_setscheduler(

	pthread_t newthread[AmountOfPeople];

	Person tab[AmountOfPeople];
//----------------------------------------Initializing visitors------------------
	for(int i=0;i<AmountOfPeople;i++)
	{
		tab[i].id = i;
		tab[i].watchA = rand()%maxA;
		int l = 1; // rand()%2;
		tab[i].goB = l;
		tab[i].watchB = rand()%maxB;
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
