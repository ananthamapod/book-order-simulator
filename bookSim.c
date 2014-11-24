/*TODO:
 *implement threading
 *make category queues
 *sending orders to cat. Qs.
 *spawn threads
 *make consumer threads to deal with cat. Qs.
 */

/*defines*/
#define string char*

/*includes*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tknz3r.h"
#include "query.h"
#include <pthread.h>

/*structs*/
typedef struct cust_T {
	char* name;
	int ID;
	float credit;
	string address;
	string state;
	string zip;
	struct cust_T* next;
} customer;

/*global variables*/

/*prototypes*/ 
void *testCustomerPrint(void*);
void exitPrint(char*);

/*main code*/
int buildCustomerDB(customer **first, FILE *custDB) {

	customer* curr = NULL;
	TokenizerT* tk = TKCreate(custDB);
	
	//Create customer linked list
	string tok = TKGetNextToken(tk);
	while(tok!=NULL) {
		customer* make;
		if((make = (customer*)calloc(1,sizeof(customer))) == NULL) {//Allocate and NULL out space for customer
			return -1;
		}
		if(*first == NULL) {	//1st customer case
			*first = make;
			curr = *first;
		}else {			//otherwise
			curr->next = make;
			curr = curr->next;
		}
		for(int fieldCount = 1; fieldCount <7; fieldCount++) {	//get customer data
			switch (fieldCount) {
				case 1:
					make->name = tok;
					break;
				case 2:
					make->ID = (int) strtol(tok,NULL,10);
					break;
				case 3:
					make->credit = strtof(tok, NULL);
					break;
				case 4:
					make->address = tok;
					break;
				case 5:
					make->state = tok;
					break;
				case 6:
					make->zip = tok;
					break;
				default:
					break;
			}
			tok = TKGetNextToken(tk);
		}
	}
	TKDestroy(tk);
	return 0;
}

int main(int argc, char** argv) {
	if(argc != 4) {
		exitPrint("Incorrect number of arguments. Exiting program.");
	}

	FILE* custDB = fopen(argv[1],"r");
	if(custDB == NULL) {
		exitPrint("Could not find customer database. Exiting program.");
	}
	
	//Set-up for customer linked list
	customer* first = NULL;
	if(buildCustomerDB(&first, custDB) == -1) {
		exitPrint("Could not construct customer database");
	}

	pthread_t producer;
	pthread_attr_t producer_attr;

	if(pthread_attr_init(&producer_attr) != 0) {
		exitPrint("pthread_attr_init failed for producer");
	}
	int scope = 0;
	if(pthread_create(&producer, &producer_attr, testCustomerPrint, first) != 0) {
		exitPrint("pthread_create failed for producer");
	}
	if(pthread_join(producer, NULL) != 0) {
		exitPrint("thread failed to return properly");
	}

	printf("Success\n");
	return 0;
}

//Test the customer linked list
void *testCustomerPrint(void* input) {
	customer *print = (customer *) input;
	if(print == NULL) {
		exitPrint("Empty customer DB. Exiting program");
	}

	for(; print != NULL; print = print->next) {
		printf("%s\n",print->name);
		printf("%d\n",print->ID);
		printf("%f\n",print->credit);
		printf("%s\n",print->address);
		printf("%s\n",print->state);
		printf("%s\n",print->zip);
		printf("%x\n",print->next);
	}
	return NULL;
}

//Error print & exit function
void exitPrint(char* msg) {
	fprintf(stderr,"%s\n",msg);
	_Exit(EXIT_FAILURE);
}
