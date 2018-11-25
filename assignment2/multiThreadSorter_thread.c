#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <linux/limits.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <dirent.h>
#include <errno.h>
#include <pthread.h>

#include "multiThreadSorter_thread.h"
//#include "helper.h"
//#include "sortingkey.h"
//#include "record.h"
//#include "arrayrecord.h"
//#include "sorter_thread.h"
//#include "mergesort.h"
//#include "csvsorter.h"
//#include "tokenizer.h"
//#include "linked_list.h"


volatile int 		runningThreadCount 	= 0;							
volatile int 		totalThreadCount 		= 0;							
Parrayrecord 	parrayrecord 			= NULL;			



comparedata g_pFuncCompare = compare;



pthread_mutex_t 		data_mutex 	= PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t 		data_cond 	= PTHREAD_COND_INITIALIZER;
pthread_attr_t 			attr;




int main(int argc, char* argv[]) {
	if (argc == 3 || argc == 5 || argc == 7) {	
	}
	else {
		fprintf(stderr, "Usage Sorting: sorter -c sorting_key_field.\n");
		fprintf(stderr, "Usage Searching Directory: sorter -c sorting_key_field -d thisdir.\n");
		fprintf(stderr, "Usage output to directory: sorter -c sorting_key_field -d thisdir -o thatdir.\n");
		fprintf(stderr, "Invalid number of parameters: %d!\n", argc);
		return 1;
	}

	char* pSortingKey = NULL;		
	char* pInDir			=	NULL;		
	char* pOutDir		= NULL;		

	int i;
	for (i = 1; i < argc; i = i + 2) {
		char* pOption = argv[i];
		//
		if (strcmp(pOption, "-c") == 0) {																						
			if (pSortingKey == NULL) {
				pSortingKey = argv[i + 1];
			}
			else {
				fprintf(stderr, "Usage output to directory: sorter -c sorting_key_field -d thisdir -o thatdir.\n");
				return 1;
			}
		}
		else if (strcmp(pOption, "-d") == 0) {
			if (pInDir == NULL) {
				pInDir = argv[i + 1];
			}
			else {
				fprintf(stderr, "Usage output to directory: sorter -c sorting_key_field -d thisdir -o thatdir.\n");
				return 1;
			}
		}
		else if (strcmp(pOption, "-o") == 0) {
			if (pOutDir == NULL) {
				pOutDir = argv[i + 1];
			}
			else {
				fprintf(stderr, "Usage output to directory: sorter -c sorting_key_field -d thisdir -o thatdir.\n");
				return 1;
			}
		}
		else {
			fprintf(stderr, "Usage Sorting: sorter -c sorting_key_field.\n");
			fprintf(stderr, "Usage Searching Directory: sorter -c sorting_key_field -d thisdir.\n");
			fprintf(stderr, "Usage output to directory: sorter -c sorting_key_field -d thisdir -o thatdir.\n");
			fprintf(stderr, "Invalid parameter options %s!\n", pOption);
			return 1;
		}
	}

	if (pSortingKey == NULL) {
		fprintf(stderr, "Usage Sorting: sorter -c sorting_key_field.\n");
		fprintf(stderr, "Usage Searching Directory: sorter -c sorting_key_field -d thisdir.\n");
		fprintf(stderr, "Usage output to directory: sorter -c sorting_key_field -d thisdir -o thatdir.\n");
		fprintf(stderr, "Mandatory Parameters: sorting_key_field not present!\n");
		return 1;
	}
	
	
	if (pInDir == NULL) {
		pInDir = ".";
	}

	
	if (pOutDir != NULL && strcmp(pOutDir, ".") != 0) {
	   mkdir(pOutDir, 0700);
	}
	
	
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

	
	sortingparam* psortingparam = (sortingparam*)malloc(sizeof(sortingparam));
	setsortingparam(NULL, pInDir, NULL, pSortingKey, psortingparam);
	
	
	fileordirec(psortingparam);

	
	Parrayrecord 	parrayrecordOutput 	= NULL;				
	char* pHeader = NULL;
	
	arrayrecord arrayrecords[arraysize_ADD];
	arrayrecord* pTemparrayrecords[arraysize_ADD];
	{	int idx;
		for (idx=0; idx<arraysize_ADD; idx++) {
			initializearrayrecord(&arrayrecords[idx], 0);
			pTemparrayrecords[idx] = &arrayrecords[idx];
		}
	}

	
	int runningThreadCountNow = 0;
	while (TRUE) {
		Parrayrecord parrayrecordNow = NULL;
		//
		pthread_mutex_lock(&data_mutex);										
		//
		while (!(parrayrecord || runningThreadCount == 0)) {				
			pthread_cond_wait(&data_cond, &data_mutex);
		}
		//
		runningThreadCountNow = runningThreadCount;						
		if (parrayrecord) {
			swap(&parrayrecord, &parrayrecordNow); 							
		}
		else {
			parrayrecordNow = NULL;
		}
		//
		pthread_mutex_unlock(&data_mutex);										
		
		if (parrayrecordNow) {
			int count = 0;
			Parrayrecord pOne = removeOne(&parrayrecordNow);
			while (pOne) {
				if (insertOne(&parrayrecordOutput, pOne)==SUCCESS) {
					if (!pHeader) {
						pHeader = pOne->pRaw;
					}
					//
					count++;
				};
				//
				pOne = removeOne(&parrayrecordNow);
			}
			//
			sortAndMergeFiles(parrayrecordOutput, pTemparrayrecords, count);
		}
		//
		if (runningThreadCountNow == 0) {
			break;
		}
	}
	
	char outFullFileName[maxpath + maxname + 1];
	strcpy(outFullFileName, "");
	if (pOutDir != NULL) {
		strcat(outFullFileName, pOutDir);
		strcat(outFullFileName, "/");
	}
	strcat(outFullFileName, "AllFiles-sorted-");
	strcat(outFullFileName, pSortingKey);
	strcat(outFullFileName, ".csv");
	//
	FILE *file = fopen(outFullFileName, "w");
	if (file != NULL) {
		writearrayrecordToFile(pHeader, pTemparrayrecords[outputindex], file);
		fclose(file);
	}
	else {
		fprintf(stderr, "Could not open file for output %s\n", strerror(errno));
	}
	//
	cleanUparrayrecord(&parrayrecordOutput);
	//
	{	int idx;
		for (idx=0; idx<arraysize_ADD; idx++) {
			cleanarrayrecord(&arrayrecords[idx]);
		}
	}
	//
	return 0;
}


void* threadFunc(void* input) {
	arrayrecord* pRcdArray = fileordirec(input);				
	pthread_mutex_lock(&data_mutex);								
	runningThreadCount--;														
	if (pRcdArray) {																
		insertOne(&parrayrecord, pRcdArray);
	}
	//
	pthread_cond_signal(&data_cond);								
	//
	pthread_mutex_unlock(&data_mutex);							
	//
	return NULL;
}



arrayrecord* fileordirec(void* input) {
	int  childThreadCount		= 0;
	BUFFER buf;																																	
	initBuffer(&(buf), 4096);
	
	sprintf(buf.data, "Initial TID: %lu\nTIDS of all child threads: ", pthread_self());										
	
	sortingparam* psortingparam = (sortingparam*)input;
	//
	arrayrecord* pRcdArray = NULL;
	if (psortingparam->isfile) {																														
		pRcdArray = sort(psortingparam);																								
	}
	else {																																						
		
		DIR *dir = NULL;
		struct dirent *entry;
		//
		if (!(dir = opendir(psortingparam->filename))) {																			
			fprintf(stderr, "Could not open directory %s: %s\n", psortingparam->filename, strerror(errno));
		}
		else {
			while ((entry = readdir(dir)) != NULL) {
				if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {						
				}
				else if (entry->d_type == DT_DIR) {																					
					sortingparam* psortingparamChild = (sortingparam*)malloc(sizeof(sortingparam));		
					setsortingparam(psortingparam->filename, entry->d_name, NULL, psortingparam->sortingKeys, psortingparamChild);
					//
					pthread_mutex_lock(&data_mutex);
					runningThreadCount++;
					totalThreadCount++;
					pthread_mutex_unlock(&data_mutex);
					//
					pthread_t thread_id;
					pthread_create(&thread_id, &attr, threadFunc, psortingparamChild);
					childThreadCount++;
					sprintf(getbuffer(&buf, 32), "%lu,", thread_id);
				}
				else {																																	
					int lenName = strlen(entry->d_name);
					if (lenName > 4 && strcicmp(entry->d_name + lenName - 4, ".csv") == 0) {					
						sortingparam* psortingparamChild = (sortingparam*)malloc(sizeof(sortingparam));
						setsortingparam(psortingparam->filename, NULL, entry->d_name, psortingparam->sortingKeys, psortingparamChild);
						//
						pthread_mutex_lock(&data_mutex);
						runningThreadCount++;
						totalThreadCount++;
						pthread_mutex_unlock(&data_mutex);
						//
						pthread_t thread_id;
						pthread_create(&thread_id, &attr, threadFunc, psortingparamChild);
						childThreadCount++;
						sprintf(getbuffer(&buf, 32), "%lu,", thread_id);
					}
					else {
						
					}
				}
			}
		}
		//
		if (dir) {
			closedir(dir);
		}
	}
	//
	sprintf(getbuffer(&buf, 64), "\nTotal number of threads: %d\n\n", childThreadCount);
	//
	printf(buf.data, "%s", buf.data);
	//
	freeBuffer(&buf);																															
	free(psortingparam);																													
	//
	return pRcdArray;
}





void sortAndMergeFiles(Parrayrecord pHead, arrayrecord** pparrayrecord, int count) {
	Parrayrecord pOne = NULL;
	Parrayrecord pTwo = NULL;
	if (count == 1) {																													
		pOne = pHead;
		
		mergeTwo(pOne, pparrayrecord[outputindex], pparrayrecord[tempindex]);
		arrayrecord* pTemp = pparrayrecord[outputindex];
		pparrayrecord[outputindex] = pparrayrecord[tempindex];
		pparrayrecord[tempindex] = pTemp;
	}
	else {																																	
		int i, j;
		for (i = 0; i < count; i = i+2) {																							
			j = (i/2) % arraysize;
			//
			if (i==0) {
				pOne = pHead;
			}
			else {
				pOne = pTwo->next;
			}
			pTwo = pOne->next;
			//
			if (pTwo != NULL) {																										
				if (pparrayrecord[j]->iSize==0) {																			
					mergeTwo(pOne, pTwo, pparrayrecord[j]);
				}
				else {																														
					mergeTwo(pOne, pTwo, pparrayrecord[tempindex]);
					
					mergeTwo(pparrayrecord[tempindex], pparrayrecord[j], pparrayrecord[tempindex1]);
					
					arrayrecord* pTemp = pparrayrecord[j];
					pparrayrecord[j] = pparrayrecord[tempindex1];
					pparrayrecord[tempindex1] = pTemp;
				}
			}
			else {																															
				j = 0;																														
				if (pparrayrecord[j]->iSize==0) {																			
					
					printf("Never here\n");
					
					mergeTwo(pOne, pparrayrecord[j], pparrayrecord[tempindex]);							
					
					arrayrecord* pTemp = pparrayrecord[j];
					pparrayrecord[j] = pparrayrecord[tempindex];
					pparrayrecord[tempindex] = pTemp;
				}
				else {																														
					
					mergeTwo(pOne, pparrayrecord[j], pparrayrecord[tempindex]);
					
					arrayrecord* pTemp = pparrayrecord[j];
					pparrayrecord[j] = pparrayrecord[tempindex];
					pparrayrecord[tempindex] = pTemp;
				}
			}
		}
		
		while (TRUE) {
			int 					changed 	= 0;
			Parrayrecord 	p1 			= NULL;
			Parrayrecord 	p2 			= NULL;
			for (i=0; i<arraysize; i=i+2) {
				j = i/2;
				//
				p1 = pparrayrecord[i];
				p2 = pparrayrecord[i+1];
				//
				if (p1->iSize>0 && p2->iSize>0) {
					mergeTwo(p1, p2, pparrayrecord[tempindex]);
					arrayrecord* pTemp = pparrayrecord[j];
					pparrayrecord[j] = pparrayrecord[tempindex];
					pparrayrecord[tempindex] = pTemp;
					//
					//
					changed = 1;
				}
				else if (p1->iSize>0 && p2->iSize==0) {
					if (i==0) {
					}
					else {
						arrayrecord* pTemp = pparrayrecord[j];
						pparrayrecord[j] = pparrayrecord[i];
						pparrayrecord[i] = pTemp;
						//
						changed = 1;
					}
				}
				else {
					break;
				}
			}
			//
			if (changed==0) {
				break;
			}
		}
		mergeTwo(pparrayrecord[outputindex], pparrayrecord[0], pparrayrecord[tempindex]);
		arrayrecord* pTemp = pparrayrecord[outputindex];
		pparrayrecord[outputindex] = pparrayrecord[tempindex];
		pparrayrecord[tempindex] = pTemp;
	}
}

void mergeTwo(Parrayrecord p1, Parrayrecord p2, Parrayrecord pMerged) {
	resizePtrArray(pMerged, p1->iSize + p2->iSize);
	merge((void**)p1->pointerarray, p1->iSize, (void**)p2->pointerarray, p2->iSize, (void**)pMerged->pointerarray, g_pFuncCompare);
	pMerged->iSize = p1->iSize + p2->iSize;
	p1->iSize = 0;
	p2->iSize = 0;
}

 



int writearrayrecordToFile(char* header, arrayrecord* parrayrecord, FILE *file) {
	int outcome = FAILURE;
	//
	if (file != NULL) {
		fprintf(file, "%s\n", header);																													
		
		int i;
		for (i = 0; i < parrayrecord->iSize; i++) {
			*((*(parrayrecord->pointerarray + i))->keyTerm) = (*(parrayrecord->pointerarray + i))->chHold;	
			//
			fprintf(file, "%s\n", (*(parrayrecord->pointerarray + i))->datarecord);													
		}
		//
		outcome = SUCCESS;
	}
	else {
		fprintf(stderr, "Error: %s\n", strerror(errno));																					
	}
	//
	return outcome;
}
