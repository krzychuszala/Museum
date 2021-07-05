#include <stdio.h>
#include <time.h> // rand()
#include <stdlib.h>  // srand()
#include <pthread.h> // pthread_join()
#include <unistd.h> // sleep
#define AmountOfPeople 10
#define Na 3
#define Nb 5     //  (Nb < Na)!!!
#define maxA 500000  // jak długo zajmię maksymalnie oglądanie wystawy A
#define maxB 500000  // mikrosekundy, czyli 1000000 = 1 sekunda
#define addPace 500 // co ile czasu próbuje znów dodać kolejny proces

static int A = 0; // counter of people in hall A
static int B = 0; // counter of people in hall B

typedef struct Person
{
	unsigned int id; // nr procesu w programie
	unsigned int watchA; // jak długo będzie oglądał hale A
	int goB; // czy chce isć do hali B
	unsigned int watchB; // jak długo B
}Person;

void hallB();

void* hallA(void*arg)
{
	Person*x = (Person*)arg;
	A++;
	printf("%d\t",x->watchA);
//	usleep(x->watchA);
//	printf("%d oto jest goB\t",x->goB);

	while(B > Nb)
	{
	sleep(1);
	printf("I am waiting for going to B\n");
	}

	if(x->goB == 1)
	{
		hallB();
	}


	printf("Koniec procesu nr%d\n", x->id);
	A--;
	return NULL;
}

void hallB(void *arg)
{
	Person*x = (Person*)arg;
	B++;
	usleep(x->watchB);
	x->goB--;
	printf("Jestem w B %d\t",x->watchB);
	B--;
//	return NULL;
}

int main()
{
	srand(time(NULL));

	pthread_t newthread[AmountOfPeople];

	Person tab[AmountOfPeople];
	for(int i=0;i<AmountOfPeople;i++)
	{
		tab[i].id = i;
		tab[i].watchA = rand()%maxA;
		int l = rand()%2;
		tab[i].goB = l;
		tab[i].watchB = rand()%maxB;
	}

	for(int i=0;i<10;i++)
	{
		while(A > Na)
		{
			printf("waiting for A %d\n",A);
			sleep(1);
		}
		pthread_create(&newthread[i], NULL, hallA, &tab[i]);
	}


	for(int i=0;i<10;i++)
	{
		pthread_join(newthread[i], NULL);
	}
}
