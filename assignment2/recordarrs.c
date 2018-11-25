#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "multiThreadSorter_thread.h"


int initializearrayrecord(arrayrecord* parrayrecord, int initialCapacity) {
	int outcome = FAILURE;			
	
	if (parrayrecord != NULL) {
		parrayrecord->pRaw					= NULL;					
		parrayrecord->pointerarray			= NULL;					
		parrayrecord->recDataArray		= NULL;					
		parrayrecord->iCapacity				= 0;							
		parrayrecord->iSize						= 0;							
		
		parrayrecord->sKI.index				= -1;
		
		parrayrecord->next						= NULL;
		
		if (initialCapacity>0) {
			parrayrecord->recDataArray = (Record*)malloc(sizeof(Record) * initialCapacity);
		
			if (parrayrecord->recDataArray) {
				parrayrecord->iCapacity = initialCapacity;
				outcome = SUCCESS;
			}
		}
		
	}
	
	return outcome;
}




Record* getOneRecord(arrayrecord* parrayrecord) {
	Record* pRec = NULL;

	
	if (parrayrecord == NULL) {
	}
	else {
		if (parrayrecord->iCapacity > parrayrecord->iSize) {										
		}
		else {
			int newSize = parrayrecord->iCapacity * 1.5;
			parrayrecord->recDataArray = (Record*)realloc(parrayrecord->recDataArray, sizeof(Record) * newSize);
			parrayrecord->iCapacity = newSize;
		}
		
		pRec = (parrayrecord->recDataArray + parrayrecord->iSize);
		parrayrecord->iSize++;
	}
	
	return pRec;
}



void setPtrArray(arrayrecord* parrayrecord) {
	if (parrayrecord != NULL) {
		parrayrecord->pointerarray = (Record**)malloc(sizeof(Record*) * (parrayrecord->iSize));
		int i;
		for (i = 0; i < parrayrecord->iSize; i++) {
			*(parrayrecord->pointerarray + i) = (parrayrecord->recDataArray + i);
		}
	}
}

void resizePtrArray(arrayrecord* parrayrecord, int size) {
	if (parrayrecord) {
		if (parrayrecord->pointerarray) {
			if (parrayrecord->iCapacity < size) {
				parrayrecord->pointerarray = (Record**)realloc(parrayrecord->pointerarray, sizeof(Record*) * size);
				parrayrecord->iCapacity = size;
			}
		}
		else {
			parrayrecord->pointerarray = (Record**)malloc(sizeof(Record*) * size);
			parrayrecord->iCapacity = size;
		}
	}
}


void cleanarrayrecord(arrayrecord* parrayrecord) {
	if (parrayrecord != NULL) {
		if (parrayrecord->pRaw) {
			free(parrayrecord->pRaw);
			parrayrecord->pRaw = NULL;
		}
		
		if (parrayrecord->pointerarray) {
			free(parrayrecord->pointerarray);
			parrayrecord->pointerarray = NULL;
		}
		
		if (parrayrecord->recDataArray) {
			free(parrayrecord->recDataArray);
			parrayrecord->recDataArray = NULL;
		}
	}
}
