/*includes*/
#include "producer.h"

/*main code*/
void parseOrders(File* ordersFile){
	TokenizerT* OrderTokenizer = TKCreate(ordersFile); //create tokenizer for orders parsing
	
	for(tok = TKGetNextToken(OrderTokenizer); tok!=NULL; /*tok = TKGetNextToken(custMakeTknzr)*/){
		bookOrder* order = (bookOrder*)calloc(1,sizeof(bookOrder));	//Allocate and NULL out space for 1 order to make
		for(int fieldCount = 1; fieldCount <5; fieldCount++){	//get order data
			switch (fieldCount){
				case 1:
					order->title = tok;
					break;
				case 2:
					order->price = strtof(tok, NULL);
					break;
				case 3:
					order->custID = (int) strtol(tok,NULL,10);
					break;
				case 4:
					order->category = tok;
					break;
				default:
					break;
			}
			tok = TKGetNextToken(OrderTokenizer);
		}
	/*send order to appropriate category queue*/
	}

	TKDestroy(OrderTokenizer);
	return;
}
