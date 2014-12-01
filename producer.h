/*defines*/
//none

/*includes*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

/*structs*/
typedef struct BookOrder_T {
	char* title;
	float price;
	int custID;
	char* category;
	struct BookOrder_T * next;
} bookOrder;

typedef struct OrderResult_T{
	char* title;
	float price;
	float remainingCredit;
	struct OrderResult_T *next;
} OrderResult;

typedef struct cust_T {
	char* name;
	int ID;
	float credit;
	char* address;
	char* state;
	char* zip;
	OrderResult *success;
	OrderResult *failure;
	pthread_mutex_t mutex;
} Customer;

typedef struct carrier_t{
	char* first;
	char* second;
} carrier;

/*globals*/

/*functions*/
void *initOrderStructure(void *); 

void *parseOrders(void *filename);

void killOrder(char* categoryTable[], int num);

void makeQueueTable(FILE* CatFile,char** categories);

void exitPrint(char* msg); 
