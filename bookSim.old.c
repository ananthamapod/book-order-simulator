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
#include <pthread.h>

/*structs*/
typedef struct cust_T{
	char* name;
	int ID;
	float credit;
	string address;
	string state;
	string zip;
	struct cust_T* next;
} customer;

/*gobal variables*/

/*prototypes*/ 
void testCustomerPrint(customer*);
void exitPrint(char*);

/*main code*/
int main(int argc, char** argv){
	if(argc != 4){
		exitPrint("Incorrect number of arguments. Exiting program.");
	}

	FILE* custDB = fopen(argv[1],"r");
	if(custDB == NULL){
		exitPrint("Could not find customer database. Exiting program.");
	}
	
	//Set-up for customer linked list
	customer* first = NULL;
	customer* curr = NULL;
	TokenizerT* custMakeTknzr = TKCreate(custDB);
	
	//Create customer linked list
	string tok;
	for(tok = TKGetNextToken(custMakeTknzr); tok!=NULL; /*tok = TKGetNextToken(custMakeTknzr)*/){
		customer* make = (customer*)calloc(1,sizeof(customer));	//Allocate and NULL out space for customer
		if(first == NULL){	//1st customer case
			first = make;
			curr = first;
		}else{			//otherwise
			curr->next = make;
			curr = curr->next;
		}
		for(int fieldCount = 1; fieldCount <7; fieldCount++){	//get customer data
			switch (fieldCount){
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
			tok = TKGetNextToken(custMakeTknzr);
		}
	}

	testCustomerPrint(first);
	printf("Success\n");
	return 0;
}

//Test the customer linked list
void testCustomerPrint(customer* print){
	if(print == NULL)
		exitPrint("Empty customer DB. Exiting program");

	for(; print != NULL; print = print->next){
		printf("%s\n",print->name);
		printf("%d\n",print->ID);
		printf("%f\n",print->credit);
		printf("%s\n",print->address);
		printf("%s\n",print->state);
		printf("%s\n",print->zip);
		printf("%x\n",print->next);
	}
	return;
}

//Error print & exit function
void exitPrint(char* msg){
	fprintf(stderr,"%s\n",msg);
	_Exit(EXIT_FAILURE);
}
