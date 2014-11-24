#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>

typedef struct {
	FILE *file;
} TokenizerT;

/*

*/
TokenizerT *TKCreate(FILE *file);
/*
Goes character by character through file in tk until it encounters alphanumeric characters,
then populates the token buffer until non-alphanumeric characters or EOF is reached
Returns: char pointer pointing to token
*/
char *TKGetNextToken(TokenizerT *tk);

void TKDestroy(TokenizerT *tk);
