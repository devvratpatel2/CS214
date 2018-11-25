#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "multiThreadSorter_thread.h"




int getsortkey(char* input, char* key, skeyindex* psk) {
	int outcome = 1;

	
	if (psk == NULL || input == NULL ||key == NULL) {
		return outcome;
	}

	
	psk->index = -1;

	if (input[0] != '\0' && key[0] != '\0') {													
		char* pToken;
		Tokenizer tokenizer;
		initTokenizer(&tokenizer, input, comma, quote1);
		pToken = getNextToken(&tokenizer);
		int index = 0;
		
		while (pToken != NULL) {
			if (strcicmp(pToken, key) == 0) {		
				
				psk->index = index;
				outcome = 0;
				
				break;
			}
			else {
				
				pToken = getNextToken(&tokenizer);
				index++;
			}
		}
	}
	else {
		
	}
	
	return outcome;
}