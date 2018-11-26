#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <dirent.h>

#include "multiThreadSorter_thread.h"


void cleanUparrayrecord(Parrayrecord* pparrayrecord) {
	if (pparrayrecord) {
		Parrayrecord pListHead = *pparrayrecord;
		//
		if (pListHead) {
			Parrayrecord pOneInList = pListHead;
			while (pOneInList) {
				Parrayrecord pToBeFree = pOneInList;
				pOneInList = pOneInList->next;
				//
				cleanarrayrecord(pToBeFree);
				free(pToBeFree);
			}
			pListHead = NULL;
		}
		//
		*pparrayrecord = NULL;
	}
}


int insertOne(Parrayrecord* pparrayrecord, Parrayrecord ppOnearrayrecord) {
	int outcome = FAILURE;
	//
	if (pparrayrecord) {
		if (ppOnearrayrecord != NULL) {
			if (*pparrayrecord == NULL) {
				*pparrayrecord = ppOnearrayrecord;
				ppOnearrayrecord->next = NULL;
			}
			else {
				ppOnearrayrecord->next = *pparrayrecord;
				*pparrayrecord = ppOnearrayrecord;
			}
			//
			outcome = SUCCESS;
		}
	}
	//
	return outcome;
}


void swap(Parrayrecord* ppInarrayrecord, Parrayrecord* ppOutarrayrecord) {
	*ppOutarrayrecord = *ppInarrayrecord;
	*ppInarrayrecord = NULL;
}


Parrayrecord removeOne(Parrayrecord* pparrayrecord) {
	Parrayrecord pTemp = NULL;
	//
	if (pparrayrecord) {
		pTemp = *pparrayrecord;
		
		if (pTemp) {
			*pparrayrecord = pTemp->next;
			pTemp->next = NULL;
		}
	}
	//
	return pTemp;
}
