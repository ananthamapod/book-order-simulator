
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


typedef struct BookOrder_T {
	char* title;
	float price;
	int custID;
	char* category;
} bookOrder;
