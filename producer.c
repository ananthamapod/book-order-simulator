/*includes*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "tknz3r.h"
#include "consumerThread.h"

/*structs*/

/*globals*/
pthread_t *consumers;

/*main code*/

/*Main producer function, calls the rest of the functions that the producer thread uses
 * and manages maintenance data structures for freeing all memory allocations that the producer can safely free
*/
void *initOrderStructure(void* argPass) {
	char** argv = (char**) argPass;
	char* orders = argv[2];
	char* categories = argv[3];

	//----------------------ESSENTIALLY SETUP---------------------------//

	FILE *categoriesfile = fopen((char*)categories,"r");

	//Finds number of categories
	int size = numLines(categoriesfile);
	//Uses result to make hash table of order queues, maintenance array of categories, and array of pthread objects, each of appropriate size
	hcreate(size);
	char** categoryTable = (char**) calloc(size,sizeof(char*));
	consumers = (pthread_t *) malloc(size*sizeof(pthread_t));

	
	makeQueueTable(categoriesfile,categoryTable);
	fclose(categoriesfile);

	//------------------------------------------------------------------//

	//--------------MAIN OPERATION OF PRODUCER THREAD-------------------//
	
	//Passes in file of orders to be processed, and off we go
	parseOrders(orders);

	//------------------------------------------------------------------//

	//-----------------------CLEAN UP OPERATIONS------------------------//
	
	//Execute order 66.
	killOrder(categoryTable, size);
	return NULL;

	//------------------------------------------------------------------//
}


/*DOES NOT COMPUTE, SELF-DESTRUCT SEQUENCE INITIATED
 * Prints error message and dies. Like a pussy.
 */
void exitPrint(char* msg) {
	fprintf(stderr,"%s\n",msg);
	_Exit(EXIT_FAILURE);
}

/*Read in the order file and start sending out the orders to be processed
 * Takes in the filename
 */

void *parseOrders(void *filename) {
	
	FILE *ordersFile = fopen((char *)filename, "r");

	//Creates tokenizer for orders parsing
	TokenizerT *OrderTokenizer = TKCreate(ordersFile);
	char *tok = TKGetNextToken(OrderTokenizer);

	//Reads to the end of file
	while(tok != NULL) {
		//Creates new order
		bookOrder *order = (bookOrder *) calloc(1, sizeof(bookOrder));

		//Reads in fields of order
		for(int fieldCount = 1; fieldCount < 5; fieldCount++) {
			switch (fieldCount) {
				case 1:
					order->title = tok;
					break;
				case 2:
					order->price = strtof(tok, NULL);
					free(tok);
					break;
				case 3:
					order->custID = (int) strtol(tok, NULL, 10);
					free(tok);
					break;
				case 4:
					order->category = tok;
					break;
				default:
					break;
			}
			tok = TKGetNextToken(OrderTokenizer);
		}

		//Sends order to be processed.
		order->next = NULL;
		insertOrder(order);
	}

	//Reached EOF
	//Frees up any freeable memory, closes file
	TKDestroy(OrderTokenizer);
	fclose(ordersFile);
	return NULL;
}

/*KILL ALL OF THE THINGS, in an organized and efficient fashion.
 * Tells the consumer threads that all the orders have been processed and waits until they finish,
 * frees all allocated memory that can be safely freed by the producer,
 * and safely destroys all data structures, mutexes and mutex variables.
*/
void killOrder(char *categoryTable[], int size) {

	//Goes through each category queue and adds a dummy order that signals to the corresponding consumer thread that there are no more orders
	for(int i = 0; i < size; i++) {

		//Kill order. Think stop codon from genetics.
		bookOrder *killOrder = (bookOrder *) malloc(sizeof(bookOrder));
		killOrder->title = "KILL THREAD ORDER";
		killOrder->category = categoryTable[i];
		killOrder->next = NULL;

		//Adds the kill order to the category queue
		insertOrder(killOrder);
		//Waits for corresponding consumer thread to finish
		pthread_join(consumers[i], NULL);

		//Frees and destroys the queue and queue fields, including the associated mutex and condition variables
		ENTRY temp, *res;
		temp.key = categoryTable[i];
		res = hsearch(temp, FIND);
		queue *Q = (queue *) res->data;
		pthread_mutex_destroy(& (Q->mutex));
		pthread_cond_destroy(& (Q->processed));
		pthread_cond_destroy(& (Q->added));
		free(Q);
		free(categoryTable[i]);
	}

	//Frees and destroys all safely deallocatable memory and data structures
	free(consumers);
	free(&categoryTable[0]);
	hdestroy();
	return;
}

/*Constructs the hash table of queues and starts the consumer threads
 *
 */
void makeQueueTable(FILE* CatFile,char** categories){
	if(CatFile == NULL){
		exitPrint("Could not find category file.");
	}

	//Creates tokenizer for parsing category names
	TokenizerT *CatTok = TKCreate(CatFile);
	char* catName = TKGetNextToken(CatTok);
	int catIndex = 0;

	//Reads to the end of the file
	while(catName != NULL) {
		
		//Creates order queue for the category
		queue* Q = (queue*) malloc(sizeof(queue));
		Q->category = catName;
		Q->first = NULL;
		Q->last = NULL;

		//Initializes the mutex and condition variables for the order queue
		pthread_mutex_init(&Q->mutex,0);
		pthread_cond_init(&Q->processed, NULL);
		pthread_cond_init(&Q->added, NULL);
		
		//Adds the queue to the hash table hashed by category name
		ENTRY newEnt;
		newEnt.data = (void *)Q;
		newEnt.key = catName;
		hsearch(newEnt,ENTER);
		//Adds the category name to the maintenance array
		categories[catIndex] = catName;

		//Starts the consumer thread for that queue
		if(pthread_create(&consumers[catIndex], NULL, processQueue, (void *)catName) != 0){
			printf("pthread_attr_init failed for consumer on iteration no. %d.",catIndex);
			exitPrint("");
		}
		
		catIndex++;
		catName = TKGetNextToken(CatTok);
	}
	//Reached EOF
	//Destroys tokenizer object
	TKDestroy(CatTok);

	return;
}
