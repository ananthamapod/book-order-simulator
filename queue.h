/*defines*/

/*includes*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <search.h>
#include "producer.h"

/*structs*/
typedef struct queue_T {
	bookOrder* first; //1st order in queue.
	bookOrder* last;	//last order in queue. Good for O(1) inserts.
	char* category;
	pthread_mutex_t mutex;
	pthread_cond_t processed;	//Condition variable signifying that all orders in queue have been processed, used by consumer thread
	pthread_cond_t added;	//Condition variable signifying that there are orders to be processed in the queue, used by producer thread 
} queue;

/*globals*/

/*functions*/

/*Adds order to a category queue. Should only be used by producer thread.
 * Uses mutexes and condition variables
*/
void insertOrder(bookOrder *incoming);

/*Gets latest order and removes it from category queue. Should only be run by consumer thread.
 * Uses mutexes and condition variables
*/
bookOrder* getOrder(char* category);


