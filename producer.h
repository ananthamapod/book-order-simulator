/*defines*/
//none

/*includes*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tknz3r.h"
#include "queue.h"

/*structs*/
typedef struct BookOrder_T {
	char* title;
	float price;
	int custID;
	char* category;
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
	string address;
	string state;
	string zip;
	//struct cust_T* next;
	OrderResult *success;
	OrderResult *failure;
} customer;

/*globals*/
Hash *custDB;	//Need to create.