#define AmountOfPeople 10
#define Na 3
#define Nb 5     //  (Nb < Na)!!!
#define maxA 500000  // jak długo zajmię maksymalnie oglądanie wystawy A
#define maxB 500000  // mikrosekundy, czyli 1000000 = 1 sekunda
#define addPace 500 // co ile czasu próbuje znów dodać kolejny proces

// static int A = 0; // counter of people in hall A
// static int B = 0; // counter of people in hall B

sem_t A;
sem_t B;

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
	sem_wait(&A);
	Person*x = (Person*)arg;
//	A++;

	usleep(x->watchA);
	x->watchA = 0;

	/*
	while(B > Nb)
	{
	sleep(1);
	printf("I am waiting for going to B\n");
	}


	if(x->goB == 1)
	{
		hallB();
	}
	*/
	int warto;
	sem_getvalue(&A,&warto);
	printf("Koniec procesu nr%d %d \n", x->id,warto);
//	A--;
	sem_post(&A);
	return NULL;
}

void hallB(void *arg)
{
	Person*x = (Person*)arg;
//	B++;
	usleep(x->watchB);
	x->watchB = 0;
	x->goB--;
	printf("Jestem w B %d\t",x->watchB);
//	B--;
//	return NULL;
}

int main()
{
	srand(time(NULL));

	sem_init(&A,0,Na);
	sem_init(&B,0,Nb);

	pthread_t newthread[AmountOfPeople];

	Person tab[AmountOfPeople];
//----------------------------------------Initializing visitors------------------
	for(int i=0;i<AmountOfPeople;i++)
	{
		tab[i].id = i;
		tab[i].watchA = rand()%maxA;
		int l = rand()%2;
		tab[i].goB = l;
		tab[i].watchB = rand()%maxB;
	}

//--------------------------------Calling threads--------------------
	for(int i=0;i<10;i++)
	{
		pthread_create(&newthread[i], NULL, hallA, &tab[i]);
	}


	for(int i=0;i<10;i++)
	{
		pthread_join(newthread[i], NULL);
	}
}
