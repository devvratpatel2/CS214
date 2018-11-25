#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "multiThreadSorter_thread.h"




void initTokenizer(Tokenizer* pTokenizer, char* _currptr, int _comma, int _quote) {
	pTokenizer->currptr = _currptr;
	pTokenizer->pToken = NULL;
	pTokenizer->comma = _comma;
	pTokenizer->quote = _quote;
	pTokenizer->isEnded = FALSE;
}

char* getNextToken(Tokenizer* pTokenizer) {
	char* pOutput;
	if (!pTokenizer) {																									
		return NULL;
	}
	else if (pTokenizer->isEnded) {																					
		return NULL;
	}

	if (*(pTokenizer->currptr) == pTokenizer->quote ) {												
		pTokenizer->currptr++;																						
		char* pQuote = strchr(pTokenizer->currptr, pTokenizer->quote);							
		if (pQuote != NULL) {
			int length = pQuote - pTokenizer->currptr;													
			pTokenizer->pToken = pTokenizer->currptr;													
			*(pTokenizer->pToken + length) = '\0';																	
			//
			pTokenizer->currptr = pQuote + 1;												
			if (pTokenizer->currptr != '\0') {
				char* pcomma = strchr(pTokenizer->currptr, pTokenizer->comma);		
				if (pcomma) {
					pTokenizer->currptr = pcomma + 1;												
				}
				else {
					pTokenizer->isEnded = TRUE;																	
				}
			}
			else {
				pTokenizer->isEnded = TRUE;																		
			}
		}
		else {																									
			pTokenizer->pToken = pTokenizer->currptr;													
			pTokenizer->isEnded = TRUE;
		}
	}
	
	else if (*(pTokenizer->currptr) == '\0') {													
			pTokenizer->pToken = pTokenizer->currptr;;
			pTokenizer->isEnded = TRUE;
	}

	else {																							
		char* pcomma = strchr(pTokenizer->currptr, pTokenizer->comma);				
		if (pcomma) {
			int length = pcomma - pTokenizer->currptr;
			
			
			pTokenizer->pToken = pTokenizer->currptr;													
			*(pTokenizer->pToken + length) = '\0';																
			pTokenizer->currptr = pcomma + 1;														
		}
		else {
			pTokenizer->pToken = pTokenizer->currptr;													
			pTokenizer->isEnded = TRUE;
		}
	}

	
	pTokenizer->pToken = pOutput;

	
	return pOutput;
}



