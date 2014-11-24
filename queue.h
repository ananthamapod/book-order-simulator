/*defines*/

/*includes*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <search.h>

/*structs*/
typedef struct queue_T{
	bookOrder* first; //1st order in queue.
	bookOrder* last;	//last order in queue. Good for O(1) inserts.
	char* category;
} queue;

/*globals*/

/*functions*/

//Function to add order to a category queue. If the category queue does not exist, it creates it.
void insertOrder(bookOrder *);	//Should only be run by the producer thread.

//Function to get and remove item from category queue. I don't know how we'll get the category to pass in, though. Any ideas?
bookOrder* getOrder(char*);	//Should only be run by consumer thread. Need to apply mutex for threads where necessary.

//Not sure if necessary, but this is a function to simply access the table. May be useful for incorporating mutexes.
ENTRY* accessTable(ENTRY, ACTION); //ACTION is an enum defined in the search.h header file. Can be FIND or ENTER.