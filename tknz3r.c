#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>


/*------------------- TOKENIZER FOR FILES ------------------------------------*/


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
		currChar = toupper(currChar);
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
		free(buffer);
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

/*---------------------------------------------------------------------*/

/*----------- UTILITY FOR FINDING NUMBER OF LINES IN A FILE -----------*/

int numLines(FILE *file) {
	int count = 0;
	char buffer[1024];
	int curr = ftell(file);
	fseek(file, 0, SEEK_SET);
	while(fgets (buffer, 1024, file) != NULL) {
		count++;
	}
	fseek(file, curr, SEEK_SET);
	return count;
}

/*---------------------------------------------------------------------*/
