# Museum
My project for OSCP

I've implemented this museum as two threads: hallA and hallB. 
I have people as objects of a structure Person. 
Values for each person is random within particular bounds. 
I execute the watching exhibition by function usleep. 
I control amount of people in each hall by counting semaphores A and B. 
I avoid deadlock situation by third semaphore Dead, which don't allow to come when hall B is full and hall A has only one place, because theoretically it's possible that everyone from hall A would like to go to B, then we will have a deadlock. 

To experiment with situation in museum, we can change macros in the begging of my code. 
For example we can change Na or Nb. 
