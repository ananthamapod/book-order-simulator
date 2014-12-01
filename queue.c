/*defines*/
#ifndef BLUE
  #define BLUE "\x1B[12;34m"
#endif
#ifndef GREEN
  #define GREEN "\x1B[12;32m"
#endif
#ifndef NORMAL
  #define NORMAL "\033[0m"
#endif

/*includes*/
#include "producer.h"
#include <search.h>

/*enums*/

/*structs*/
typedef struct queue_T {
	bookOrder* first; //1st order in queue.
	bookOrder* last;	//last order in queue. Good for O(1) inserts.
	char* category;
	pthread_mutex_t mutex;
	pthread_cond_t processed;	//condition variable signifying that no orders are in the queue
	pthread_cond_t added;	//condition variable signifying that there are orders to be processed
} queue;

/*globals*/


/*functions*/

/*Adds order to a category queue. Should only be used by producer thread.
 * Uses mutexes and condition variables
*/
void insertOrder(bookOrder *incoming) {
	ENTRY temp, *res;
	temp.key = incoming->category;
	
	res = hsearch(temp,FIND);
	if(res == NULL) {
		//Nonexisting category. Return without doing anything with it. 
		return;
	}

	//Finds queue corresponding to category
	queue* Q = (queue*) res->data;

	/*	MUTEX LOCK HERE		*/
	pthread_mutex_lock(& (Q->mutex));
	
	//Signals the consumer thread corresponding to queue that there is still an order to be processed in the queue
	//Reliquishes the mutex lock and waits for the consumer to process the items in the queue
	//Then reacquires the mutex and continues
	while(Q->last != NULL) {
		printf(GREEN "Producer waits because queue corresponding to category %s is full.\n" NORMAL, res->key);
		pthread_cond_signal(& (Q->added));
		pthread_cond_wait(& (Q->processed), & (Q->mutex));
	}
	printf(GREEN "Producer resumes because queue corresponding to category %s has space available.\n" NORMAL, res->key);
	
	//Adds the order to be processed onto the queue
	Q->last = incoming;
	Q->first = incoming;

	//Sends signal to consumer thread that there are orders to be processed in the queue
	pthread_cond_signal(& (Q->added));

	/*	MUTEX UNLOCK HERE	*/
	pthread_mutex_unlock(& (Q->mutex));
	return;
}

/*Gets latest order and removes it from category queue. Should only be run by consumer thread.
 * Uses mutexes and condition variables
*/
bookOrder* getOrder(char* category) {
	bookOrder *ret = NULL;
	ENTRY temp, *res;

	temp.key = category;
	res = hsearch(temp,FIND);
	//If no such category exists, return nothing
	if(res == NULL) {
		return NULL;
	}
	//Finds queue corresponding to category
	queue *Q = (queue*) res->data;

	/*	MUTEX LOCK HERE		*/
	pthread_mutex_lock(& (Q->mutex));

	//Until queue has orders in it, consumer signals the producer thread that the queue is empty and relinquishes the mutex
	//When the producer signals that the queue is no longer empty, consumer reacquires mutex
	while(Q->first == NULL) {
		printf(BLUE "Consumer waits because queue corresponding to category %s is empty.\n" NORMAL, category);
		pthread_cond_signal(& (Q->processed));
		pthread_cond_wait(& (Q->added), & (Q->mutex));
	}
	printf(BLUE "Consumer resumes because queue corresponding to category %s has orders to be processed.\n" NORMAL, category);

	//Stores the first order in the queue
	ret = Q->first;

	//Removes the order from the queue
	Q->first = Q->first->next;
	if(Q->first == NULL) {
		Q->last = NULL;
	}

	ret->next = NULL;
	
	//Signals the producer thread that all the orders in the queue have been processed
	pthread_cond_signal(& (Q->processed));

	/*	MUTEX UNLOCK HERE	*/
	pthread_mutex_unlock(& (Q->mutex));
	return ret;
}
