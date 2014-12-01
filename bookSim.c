/*TODO:
 *implement threading
 *spawn threads
 *make consumer threads to deal with cat. Qs.
 */

/*defines*/
#define string char*

#ifndef RED
  #define RED "\x1B[31m"
#endif
#ifndef NORMAL
  #define NORMAL "\033[0m"
#endif

/*includes*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "producer.h"
#include "hash.h"
#include <pthread.h>
#include "tknz3r.h"

/*structs*/


/*global variables*/

/*prototypes*/ 
void printReports(Hash*);
void exitPrint(char*);
OrderResult *reverse(OrderResult *head);

/*main code*/

/*EXTERMINATE.
 *Everything. Related to a Customer_T struct.
 */
void destroyTheCustomer(Customer *customer) {
	OrderResult *curr, *prev;
	free(customer->name);
	free(customer->address);
	free(customer->state);
	free(customer->zip);

	curr = customer->success;
	while(curr != NULL) {
		prev = curr;
		curr = curr->next;

		free(prev->title);
		free(prev);
	}
	curr = customer->failure;
	while(curr != NULL) {
		prev = curr;
		curr = curr->next;

		free(prev->title);
		free(prev);
	}
	
	pthread_mutex_destroy(& (customer->mutex));
	free(customer);
}

/*Reads in the file containing the customer and creates the customer database as a hash table
 *
 */
int buildCustDB(Hash **hash, char *filename) {
	FILE *file = fopen(filename, "r");
	//Finds number of customer by finding number of lines, since each customer's data is on a different line
	int size = numLines(file);
	TokenizerT *tk = NULL;

	Entry temp;

	if((*hash = HCreate(size, destroyTheCustomer)) == NULL) {
		return -1;
	}
	
	if((tk = TKCreate(file)) == NULL) {
		return -1;
	}
	string tok = TKGetNextToken(tk);

	//Reads to the end of the file
	while(tok!=NULL) {
		Customer* make;

		//Allocates and space for new customer
		if((make = (Customer*)calloc(1,sizeof(Customer))) == NULL) {
			return -1;
		}
		
		//Populates data for each customer field
		for(int fieldCount = 1; fieldCount <7; fieldCount++) {	
			switch (fieldCount) {
				case 1:
					make->name = tok;
					break;
				case 2:
					make->ID = (int) strtol(tok,NULL,10);
					free(tok);
					break;
				case 3:
					make->credit = strtof(tok, NULL);
					free(tok);
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

		//Adds the new customer to the database (hash table)
		temp.key = make->ID;
		temp.data = make;

		if(HSearch(temp, ADD) ==NULL) {
			return -1;
		}
	}

	//Destroys the tokenizer and closes the file
	TKDestroy(tk);
	fclose(file);
	return 0;
}

int main(int argc, char** argv) {
	if(argc != 4) {
		exitPrint("Incorrect number of arguments. Exiting program.");
	}

	//Set-up for customer hash table
	Hash *custDB = NULL;
	if(buildCustDB(&custDB, argv[1]) == -1) {
		exitPrint("Could not construct customer database");
	}

	//Starts the producer thread
	pthread_t producer;
	pthread_attr_t producer_attr;
	void *thread_result;

	if(pthread_attr_init(&producer_attr) != 0) {
		exitPrint("pthread_attr_init failed for producer");
	}
	
	
	if(pthread_create(&producer,&producer_attr, initOrderStructure, argv) != 0) {
		exitPrint("pthread_create failed for producer");
	}
	if(pthread_join(producer, &thread_result) != 0) {
		exitPrint("thread failed to return properly");
	}
	//Prints reports and frees leftover unallocated memory
	printReports(custDB);
	HDestroy();

	printf("Success\n");
	return 0;
}

/*Prints final reports for all of the customers
 *
 */
void printReports(Hash *hash) {
	if(hash == NULL) {
		return;
	}
	Entry *entry = hash->customerList;
	int i = 0;
	float revenue = 0;
	if(entry == NULL) {
		exitPrint("Empty customer DB. Exiting program");
	}

	for(i = 0; i < hash->size; i++) {
		int empty = 1;
		Customer *print = (Customer *)(entry[i].data);
		printf("============================\n");
		printf("CUSTOMER: %s\nID: %d\n", print->name, print->ID);
		printf("FINAL BALANCE: %.2f\n",print->credit);
		printf("============================\n");

		//Have to reverse the results list, since it stores most recent first and final report is printed from oldest to newest
		OrderResult *temp = reverse(print->success);
		printf("Successful Orders: \n--------------------\n");
		while(temp != NULL) {
			printf("%s\t$%.2f\nRemaining credit: $%.2f\n\n", temp->title, temp->price, temp->remainingCredit);
			revenue += temp->price;
			temp = temp->next;
			empty = 0;
		}
		if(empty == 1) {
			printf("NONE\n\n");
		}

		//As with the list of successes, have to reverse the list before printing final report
		temp = reverse(print->failure);
		empty = 1;
		printf(RED "Unsuccessful Orders: \n--------------------\n");
		while(temp != NULL) {
			printf("%s\t$%.2f\n\n", temp->title, temp->price);
			temp = temp->next;
			empty = 0;
		}
		if(empty == 1) {
			printf("NONE\n\n");
		}
		printf(NORMAL "============================\n\n");
	}
	printf("\n\nTOTAL REVENUE: $%.2f\n", revenue);
}

/*------- UTILITY FOR REVERSING LINKED LIST OF PROCESSED ORDERS -------*/

OrderResult *reverse(OrderResult *head) {
	OrderResult *ptr1, *ptr2, *ptr3;
	if(head == NULL) {
		return NULL;
	}

	ptr1 = head;
	ptr2 = ptr1->next;
	if(ptr2  == NULL) {
		return ptr1;
	}

	ptr3 = ptr2->next;

	while(ptr3 != NULL) {
		ptr2->next = ptr1;
		ptr1 = ptr2;
		ptr2 = ptr3;
		ptr3 = ptr2->next;
	}
	ptr2->next = ptr1;
	head->next = NULL;

	return ptr2;
}

/*---------------------------------------------------------------------*/
