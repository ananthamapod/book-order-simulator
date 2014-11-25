/*defines*/

/*includes*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <search.h>

/*structs*/
typedef struct queue_T {
	bookOrder* first; //1st order in queue.
	bookOrder* last;	//last order in queue. Good for O(1) inserts.
	char* category;
} queue;

/*globals*/


/*functions*/
//Function to add order to a category queue. If the category queue does not exist, it creates it.
void insertOrder(bookOrder *incoming) {	//Should only be run by the producer thread.
	ENTRY temp, *res;
	/*
		MUTEX LOCK 
		MUTEX LOCK
		MUTEX LOCK
		MUTEX LOCK
	 */
	temp.key = incoming->category;
	res = hsearch(temp,FIND);
	if(res == NULL) {	//New category = new queue. Spawn a new thread to handle the queue.
/* 		queue* newQ = (queue*) malloc(sizeof(queue));
		newQ->first = incoming;
		newQ->last = incoming;
		newQ->category = incoming->category;
		temp.data = newQ;
		temp.key = incoming->category;	//Just to be safe and try to prevent overwriting. May not work, though.
		hsearch(temp,ENTER); */
	} else {	//Seen category before. Adding to category queue.
		queue* Q = (queue*) res->data;
		Q->last->next = incoming;
		Q->last = Q->last->next;
	}
	/*
	MUTEX UNLOCK
	MUTEX UNLOCK
	MUTEX UNLOCK
	MUTEX UNLOCK
	*/
	return;
}

//Function to get and remove item from category queue. I don't know how we'll get the category to pass in, though. Any ideas?
bookOrder* getOrder(char* category) {	//Should only be run by consumer thread. Need to apply mutex for threads where necessary.
	bookOrder *ret = NULL;
	ENTRY temp, *res;
	/*
		MUTEX LOCK 
		MUTEX LOCK
		MUTEX LOCK
		MUTEX LOCK
	 */
	temp->key = category;
	res = hsearch(temp,FIND);
	if(res == NULL) {
		/*
		 *Looking for queue of category we have not encountered yet. How should this be handled?
		 *I suggest having the thread sleep for a bit of a while and then try again.
		 */
	} else {	//Queue exists.
		queue *Q = (queue*) res->data;
		do {
			ret = Q->first;
			if(ret == NULL) //Queue is empty. Have consumer thread sleep for a bit here while the processor thread populates queue.
				/*
				MUTEX UNLOCK
				MUTEX UNLOCK
				MUTEX UNLOCK
				MUTEX UNLOCK
				*/
				
				//THREAD SLEEP FOR 1~2 SECONDS
				
				/*
				MUTEX LOCK 
				MUTEX LOCK
				MUTEX LOCK
				MUTEX LOCK
				*/
		} whie(ret == NULL);
		Q->first = Q->first->next;
		ret->next = NULL;
	}
	/*
	MUTEX UNLOCK
	MUTEX UNLOCK
	MUTEX UNLOCK
	MUTEX UNLOCK
	*/
	return ret;
}

//Not sure if necessary, but this is a function to simply access the table. May be useful for incorporating mutexes.
ENTRY* accessTable(ENTRY item, ACTION action) { //ACTION is an enum defined in the search.h header file. Can be FIND or ENTER.
	//mutex lock here, I suppose.
	ENTRY* ret = hsearch(item, action);
	//mutex unlock here, I suppose
	return ret;
}
