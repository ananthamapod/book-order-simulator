/*defines*/
#ifndef RED
  #define RED "\x1B[31m"
#endif
#ifndef NORMAL
  #define NORMAL "\033[0m"
#endif

//none

/*includes*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hash.h"
#include "queue.h"

/*structs*/

/*globals*/

/*main code*/

/*Main function called by consumer thread. Obtains order from queue and processes it.
 *
 */
void *processQueue(void *catName) {

	bookOrder *order;

	while(1) {
		//Get order in queue.
		order = getOrder(catName);
		//Mutex un/locks on the orders are in the getOrder function in queue.c file.
		
		//Stop signal. Think stop codon from genetics.
		if(strcmp(order->title, "KILL THREAD ORDER") == 0) {
			free(order);
			break;
		}
		 
		//Gets customer data.
		Entry *res, temp = {order->custID, NULL};
		res = HSearch(temp, SEARCH);

		Customer *cust = (Customer *) res->data;

		
		/*	MUTEX LOCK HERE		*/
		pthread_mutex_lock(& (cust->mutex));
		
		//Stores price and other values for reuse.
		float cred = cust->credit;
		float cost = order->price;
		float result = cred - cost;
		
		//Prepares result data
		OrderResult *add = (OrderResult *) malloc(sizeof(OrderResult));
		add->title = order->title;
		add->price = cost;
		
		if(result >= 0) {
			//Good purchase. Add it to customer's success list
			cust->credit = result;
			add->remainingCredit = result;
			add->next = cust->success;
			cust->success = add;

			printf("ORDER CONFIRMATION\n----------------------\n%s\t$%.2f\n-----------------------\n%s\n%s\n%s %s\n",
				order->title, order->price, cust->name, cust->address, cust->state, cust->zip);
			printf("Remaining balance: $%.2f\n\n\n", cust->credit);

		}else {
			//Bad purchase. Add to customer's failure list.
			add->next = cust->failure;
			cust->failure = add;
			
			printf(RED "ORDER REJECTION\n-----------------------\n%s\n-----------------------\n%s\t$%.2f\nRemaining credit: $%.2f\n\n\n" NORMAL,
				cust->name, order->title, order->price, cust->credit);
		}

		/*	MUTEX UNLOCK HERE	*/
		pthread_mutex_unlock(& (cust->mutex));

		//Frees all the data associated with the order that is no longer relevant
		free(order->category);
		free(order);
	}

	return NULL;
}
