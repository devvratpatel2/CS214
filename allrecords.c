#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

#include "multiThreadSorter_thread.h"




Record* setdatarecord(Record* pRec, char* store, char* buffer, skeyindex* psk) {
	Record* pOut = NULL;
	if (store == NULL || *store == '\0' || buffer == NULL || *buffer == '\0' || psk == NULL || pRec==NULL) {
		if (pRec) {
			pRec->datarecord = NULL;
		
		}
	}
	else {
		pRec->datarecord = store;
		
		pOut = pRec;
	}
	
	
	if (pOut != NULL) {
		Tokenizer tokenizer;
		initTokenizer(&tokenizer, buffer, comma, quote1);

		char* bufferStart = buffer;												
		char* pToken;

		pToken = getNextToken(&tokenizer);

		int index = 0;
		
		while(psk->index > index) {				

			pToken = getNextToken(&tokenizer);
			index++;
		}

		int offset;
		int length;
		if (pToken == NULL) {
			offset = strlen(pRec->datarecord);					
			length = 0;
		}
		else {
			offset = pToken - bufferStart;
			length = strlen(pToken);
		}
		//
		pRec->pkey 		= store + offset;
		pRec->pkeyTerm 	= store + offset + length;
		pRec->chHold 		= *pRec->pkeyTerm;										
		*pRec->pkeyTerm 	= '\0';														
		//
		pRec->lKey = 0L;																	
		pRec->fKey = 0.0f;															
		//
		char	cTemp;
		if (length > 0) {
			char* pDot = strchr(pRec->pkey, DOT);									
			
			if (!pDot && sscanf(pRec->pkey, "%ld%c", &(pRec->lKey), &cTemp) == 1) {					
				pRec->fKey = pRec->lKey;
				pRec->idxType =intindex;
			}
			else if (pDot && sscanf(pRec->pkey, "%f%c", &(pRec->fKey), &cTemp) == 1) {			
				pRec->idxType = floatindex;												
			}
			else {
				pRec->idxType = stringindex;												
			}
		}
		else {
			pRec->idxType = nullindex;
		}
	}
	//
	return pOut;
}



int compare(void* pData1, void* pData2) {
	int outcome;
	// 
	Record* pRecord1 = (Record*)pData1;
	Record* pRecord2 = (Record*)pData2;
	//
	int idxType1 = pRecord1->idxType;
	int idxType2 = pRecord2->idxType;
	//
	if (idxType1==nullindex && idxType2==nullindex) {
		outcome = 0;
	}
	else if (idxType1==nullindex && idxType2!=nullindex) {
		outcome = -1;
	}
	else if (idxType1!=nullindex && idxType2==nullindex) {
		outcome = 1;
	}
	else {
		if (idxType1== stringindex && idxType2== stringindex) {
			outcome = strcicmp(pRecord1->pkey, pRecord2->pkey);			
		else if (idxType1== stringindex && idxType2!= stringindex) {
			outcome = 1;
		}
		else if (idxType1!= stringindex && idxType2== stringindex) {
			outcome = -1;
		}
		else {
			if (idxType1==intindex && idxType2==intindex) {
				long 	ldelta = pRecord1->lKey - pRecord2->lKey;
				if (ldelta == 0L) {
					outcome = 0;
				}
				else if (ldelta > 0L) {
					outcome = 1;
				}
				else {
					outcome = -1;
				}
			}
			else {
				float fdelta;
				fdelta = pRecord1->fKey - pRecord2->fKey;
				if (fdelta == 0.0) {
					outcome = 0;
				}
				else if (fdelta > 0.0) {
					outcome = 1;
				}
				else {
					outcome = -1;
				}
			}
		}
	}
	
	return outcome;
}
