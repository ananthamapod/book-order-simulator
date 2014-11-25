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

/*main code*/

/*Function that initializes hash table and entries, also takes in function responsible for freeing data*/
Hash *HCreate (int size, void (*)() destroy);

/*Function that finds entry in hash table corresponding to entry's key
If action is ENTER, adds entry to hash table
If action is FIND, returns entry in hash table corresponding to entry's key*/
ENTRY *HSearch (Hash *hash, ENTRY *entry, ACTION action);

/*Function that frees all memory allocated by hash table, entries, and entry data*/
void HDestroy(Hash *hash);
