/*defines*/

/*includes*/

/*structs*/
typedef struct ENTRY_T {
	int key;
	void *data;
} ENTRY;

typedef struct hash {
	int size;
	ENTRY *customerList;
	void (*) () destroy;
} Hash;

/*enums*/
typedef enum ACTION_T {FIND,ENTER} ACTION;
/*globals*/

/*main code*/

/*Function that initializes hash table and entries, also takes in function responsible for freeing data*/
Hash *HCreate (int size, void (*)() destroy) {
	Hash *newHash = (Hash *) malloc(sizeof(Hash));
	newHash->size = size;
	newHash->customerList = (ENTRY *) malloc(size * sizeof(ENTRY));
	newHash->destroy = destroy;
	ENTRY *customers = newHash->customerList;
	int i;
	for(i = 0; i < size; i++) {
		customers[i] = NULL;
	}
	return newHash;
}

/*Function that finds entry in hash table corresponding to entry's key
If action is ENTER, adds entry to hash table
If action is FIND, returns entry in hash table corresponding to entry's key*/
ENTRY *HSearch (Hash *hash, ENTRY *entry, ACTION action) {
	if(hash == NULL || entry == NULL) {
		return NULL;
	}
	if((entry->key < hash->size) && (entry->key >= 0)) {
		if(action == ENTER) {
			hash->customerList[entry->key]->data = entry->data;
		}
		return hash->customerList[key];
	}

	return NULL;
}

/*Function that frees all memory allocated by hash table, entries, and entry data*/
void HDestroy(Hash *hash) {
	if(hash == NULL) {
		return;
	}

	int i;
	ENTRY *customers = hash->customerList;
	void (*)() destroy = hash->destroy;
	for(i = 0; i < size; i++) {
		destroy(customers[i]->data);
	}
	free(customers);
	free(hash);
}
