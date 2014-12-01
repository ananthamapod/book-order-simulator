/*defines*/
#include <stdio.h>
#include <stdlib.h>
/*includes*/

/*structs*/
typedef struct Entry_T {
	int key;
	void *data;
} Entry;

typedef struct hash {
	int size;
	Entry *customerList;
	void (*destroy)();
} Hash;

/*enums*/
typedef enum Action_T {SEARCH,ADD} Action;
/*globals*/
Hash *custDB;

/*main code*/

/*Function that initializes hash table and entries, also takes in function responsible for freeing data
 *
*/
Hash *HCreate (int size, void (*destroy) ()) {
	custDB = (Hash *) malloc(sizeof(Hash));
	custDB->size = size;
	custDB->customerList = (Entry *) malloc(size * sizeof(Entry));
	custDB->destroy = destroy;
	Entry *customers = custDB->customerList;
	int i;
	for(i = 0; i < size; i++) {
		customers[i].key = i+1;
		customers[i].data = NULL;
	}
	return custDB;
}

/*Function that finds entry in hash table corresponding to entry's key
* If action is ENTER, adds entry to hash table
* If action is FIND, returns entry in hash table corresponding to entry's key
*/
Entry *HSearch (Entry entry, Action action) {
	if(custDB == NULL) {
		return NULL;
	}
	if((entry.key <= custDB->size) && (entry.key > 0)) {
		if(action == ADD) {
			custDB->customerList[entry.key-1].data = entry.data;
		}
		return &(custDB->customerList[entry.key-1]);
	}

	return NULL;
}

/*Function that frees all memory allocated by hash table, entries, and entry data
 *
*/
void HDestroy() {
	if(custDB == NULL) {
		return;
	}

	int i;
	Entry *customers = custDB->customerList;
	void (*destroy)() = custDB->destroy;
	for(i = 0; i < custDB->size; i++) {
		destroy(customers[i].data);
	}
	free(customers);
	free(custDB);
}
