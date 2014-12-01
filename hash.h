/*structs*/
typedef struct Entry_T {
	int key;
	void *data;
} Entry;

typedef struct hash {
	int size;
	Entry *customerList;
	void (*destroy) ();
} Hash;

Hash *custDB;

/*enums*/
typedef enum Action_T {SEARCH,ADD} Action;

/*main code*/

/*Function that initializes hash table and entries, also takes in function responsible for freeing data*/
Hash *HCreate (int size, void (*destroy) ());

/*Function that finds entry in hash table corresponding to entry's key
If action is ENTER, adds entry to hash table
If action is FIND, returns entry in hash table corresponding to entry's key*/
Entry *HSearch (Entry entry, Action action);

/*Function that frees all memory allocated by hash table, entries, and entry data*/
void HDestroy();
