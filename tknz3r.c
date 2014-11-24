#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>

//MAKTOKSIZE defined so that there is a fixed maximum size for the string buffer
#define MAXTOKSIZE 1000

/*
TokenizerT struct
*/
typedef struct {
	FILE *file;
} TokenizerT;

/*
Inputs: FILE pointer file
Outputs: TokenizerT pointer or NULL if the memory could not be allocated
Dynamically allocates memory for TokenizerT tk and sets file
*/
TokenizerT *TKCreate(FILE *file) {
	TokenizerT *tk = (TokenizerT *)malloc(sizeof(TokenizerT));
	if(tk == NULL) {
		return NULL;
	}
	tk->file = file;
	return tk;
}

/*
Inputs: TokenizerT pointer tk
Outputs: next token read from file, or NULL if no more tokens are left
- Reads next token from file, uses isspace(char) to determine when to terminate token. -Changed by Imran
- Uses fgetc(FILE *) to go character by character through file.
- Converts all characters to lowercase before any token is populated.
- Starts with buffer of a dynamically allocated block of max size, and then after populating the token,
the buffer is reallocated to its proper size
*/
char *TKGetNextToken(TokenizerT *tk) {
	char *buffer = (char *)malloc((MAXTOKSIZE + 1)*sizeof(char));
	char currChar = fgetc(tk->file);
	int pos = 0;
	while(currChar != EOF && pos < MAXTOKSIZE) {
		currChar = tolower(currChar);
		if(currChar != '|' && currChar != '\n') {
			buffer[pos] = currChar;
			pos++;
		} else {
			if(pos > 0) {
				break;
			}
		}
		currChar = fgetc(tk-> file);
	}
	/*if pos is 0 at this point, it means that EOF has been reached without populating any further tokens,
	*so returns NULL*/
	if(pos == 0) {
		return NULL;
	}
	//adding the null termination
	buffer[pos] = '\0';
	buffer = (char *)realloc(buffer, (pos+1)*sizeof(char));
	return buffer;
}

/*
Inputs: TokenizerT pointer tk to be freed
Outputs: None
Frees allocated memory for TokenizerT object
*/
void TKDestroy(TokenizerT *tk) {
	free(tk);
}
/*int main(int argc, char** argv) {
	if(argc != 3) {
		printf("Not enough arguments.\n");
		return -1;
	}
	FILE *to_file = fopen(argv[1], "w+");
	FILE *from_file = fopen(argv[2], "r");
	TokenizerT *tk = TKCreate(from_file);
	char *token = TKGetNextToken(tk);
	while(token != NULL) {
		fwrite(token,sizeof(char),strlen(token),to_file);
		fwrite("\n",sizeof(char),1,to_file);
		token = TKGetNextToken(tk);
	}
	
	return 0;
}*/
