#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "multiThreadSorter_thread.h"


char* initBuffer(BUFFER* pBuf, int initSize) {
	if (initSize == 0) {
		pBuf->data = NULL;
		pBuf->length = 0;
	}
	else {
		pBuf->data = (char*)malloc(initSize * sizeof(char));
		if (pBuf->data == NULL) {
			pBuf->length = 0;
		}
		else {
			pBuf->length = initSize;
		}
	}
	
	return pBuf->data;
}



char* sizebuffer(BUFFER* pBuf, int size) {
	if (pBuf->length >= size) {
	
	}
	else {
		pBuf->data = realloc(pBuf->data, size);
		if (pBuf->data == NULL) {
			pBuf->length = 0;
		}
		else {
			pBuf->length = size;
		}
	}
	//
	return pBuf->data;
}
char* getbuffer(BUFFER* pBuf, int delta) {
	int len = strlen(pBuf->data);
	int size = len + 1 + delta;
	
	if (pBuf->length >= size) {
	}
	else {
		int newsize = size * 2;
		
		pBuf->data = realloc(pBuf->data, newsize);
		if (pBuf->data == NULL) {
			pBuf->length = 0;
		}
		else {
			pBuf->length = newsize;
		}
	}
	
	return pBuf->data + len;
}
void freeBuffer(BUFFER* pBuf) {
	if (pBuf->data) {
		free(pBuf->data);
		
		pBuf->data = NULL;
		pBuf->length = 0;
	}
}


int stripnewline(char* pBuf) {
	int len = strlen(pBuf);
	if ( *(pBuf + len - 1) == '\n' ) {
		*(pBuf + len - 1)  = '\0';
		len--;
	}
	
	return len;
}

int strcicmp(char const *input1, char const *input2) {
	int i;
    for (;; input1++, input2++) {
        i = tolower(*input1) - tolower(*input2);
        if (i != 0 || !*input1) {
        	return i;
		}
    }
}