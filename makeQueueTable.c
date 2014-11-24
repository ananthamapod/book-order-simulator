/*defines*/
//none

/*includes*/
#include "makeQueueTable.h"

/*structs*/
//none

/*globals*/
//none

/*code*/

//Makes a POSIX hash table of category queues.
void makeQueueTable(char* CatFileName){
	FILE* CatFile = fopen(CatFileName,"r");
	if(CatFile == NULL){
		printExit("Could not fine category file.");
	}
	int CatCount = CountLines(CatFile);
	hcreate(CatCount);
	
	fseek(CatFile,0,SEEK_SET);
	TokenizerT *CatTok = TKCreate(CatFile);
	char* catName = TKGetNextToken(CatTok);
	
	while(catName != NULL){
		queue* Q = (queue*) malloc(sizeof(queue));
		Q->category = catName;
		ENTRY newEnt;
		newEnt.data = (void*)Q;
		newEnt.key = catName;
		hsearch(newEnt,ENTER);
		catName = TKGetNextToken(CatTok);
	}
	fclose(CatFile);
	return;
}

/*copied from
 *https://stackoverflow.com/questions/4278845/what-is-the-easiest-way-to-count-the-newlines-in-an-ascii-file
 */
 //Finds the number of lines in a file.
int CountLines(File *CatFile) {
    int line_count = 0;

    while ( fgetline(CatFile) )
        line_count++;
    return line_count;
}