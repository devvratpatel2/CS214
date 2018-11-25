#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>

#include "multiThreadSorter_thread.h"


arrayrecord* sort(sortingparam* psortingparam) {
	comparedata pFuncCompare = compare;
	
	struct stat file_stats;

	arrayrecord* pRcdArray = (arrayrecord*)malloc(sizeof(arrayrecord));

	if (!stat(psortingparam->filename, &file_stats)) {
	}
	else {
		fprintf(stderr, "Error: %s\n", strerror(errno));
	}

	FILE *file = fopen(psortingparam->filename, "r");

	BUFFER buf;										
	initBuffer(&(buf), 0);						

	if (file == NULL) {
		fprintf(stderr, "Error: %s\n", strerror(errno));
	}
	else if (initializearrayrecord(pRcdArray, initialsize) != SUCCESS) {
		cleanarrayrecord(pRcdArray);
		free(pRcdArray);
		pRcdArray = NULL;
		//
		fprintf(stderr, "Could not parse sorting key\n");
	}
	else {
		unsigned int lenRemain = file_stats.st_size + 1 + 2;										
		pRcdArray->pRaw = (char*)malloc(sizeof(char) * (lenRemain + 1 + 2));
		char* pRemain = pRcdArray->pRaw;

		if (fgets(pRemain, lenRemain, file) > 0 ) {
			int len = stripnewline(pRemain);
			//
			sizebuffer(&buf, len + 1);
			strcpy(buf.data, pRemain);						
			//
			pRemain = pRemain + len + 1;
			lenRemain = lenRemain - len - 1;
			//
			if (getsortkey(buf.data, psortingparam->sortingKeys, &(pRcdArray->sKI))) {
				cleanarrayrecord(pRcdArray);
				free(pRcdArray);
				pRcdArray = NULL;
				//
				fprintf(stderr, "Could not parse sorting key\n");
			}
			else {
				while (fgets(pRemain, lenRemain, file) > 0) {
					int len = stripnewline(pRemain);
					//
					sizebuffer(&buf, len + 1);
					strcpy(buf.data, pRemain);						

					Record* pRecord = getOneRecord(pRcdArray);

					Record* pOut = setdatarecord(pRecord, pRemain, buf.data, &(pRcdArray->sKI));

					if (!pOut) {
						fprintf(stderr, "Could not create record\n");
					}
					//
					pRemain = pRemain + len + 1;
					lenRemain = lenRemain - len - 1;
				}
				//
				setPtrArray(pRcdArray);
				//
				mergeSort((void**)(pRcdArray->pointerarray), 0, pRcdArray->iSize - 1, pFuncCompare);
			}
		}
	}

	
	if (file != NULL) {
		fclose(file);
	}

	
	freeBuffer(&buf);

	
	return pRcdArray;
}



void setsortingparam(const char* parentpath, const char* inPath, const char* inName, const char* sortinginput, sortingparam* psortingparam) {
	strcpy(psortingparam->sortingKeys, sortinginput);
	//
	strcpy(psortingparam->filename, "");												
	//
	int emptyParentPath 	= parentpath == NULL || strlen(parentpath) == 0;
	int emptyPath 			= inPath == NULL || strlen(inPath) == 0;
	int emptyName 			= inName == NULL || strlen(inName) == 0;
	//
	if (emptyParentPath && emptyPath && emptyName) {
		strcat(psortingparam->filename, ".");
		psortingparam->isfile = FALSE;
	}
	else if (emptyParentPath && emptyPath && !emptyName) {
		strcat(psortingparam->filename, inName);
		psortingparam->isfile = TRUE;
	}
	else if (emptyParentPath && !emptyPath && emptyName) {
		strcat(psortingparam->filename, inPath);
		psortingparam->isfile = FALSE;
	}
	else if (emptyParentPath && !emptyPath && !emptyName) {
		strcat(psortingparam->filename, inPath);
		strcat(psortingparam->filename, "/");
		strcat(psortingparam->filename, inName);
		psortingparam->isfile = TRUE;
	}
	else if (!emptyParentPath && emptyPath && emptyName) {
		strcat(psortingparam->filename, parentpath);
		psortingparam->isfile = FALSE;
	}
	else if (!emptyParentPath && emptyPath && !emptyName) {
		strcat(psortingparam->filename, parentpath);
		strcat(psortingparam->filename, "/");
		strcat(psortingparam->filename, inName);
		psortingparam->isfile = TRUE;
	}
	else if (!emptyParentPath && !emptyPath && emptyName) {
		strcat(psortingparam->filename, parentpath);
		strcat(psortingparam->filename, "/");
		strcat(psortingparam->filename, inPath);
		psortingparam->isfile = FALSE;
	}
	else if (!emptyParentPath && !emptyPath && !emptyName) {
		strcat(psortingparam->filename, parentpath);
		strcat(psortingparam->filename, "/");
		strcat(psortingparam->filename, inPath);
		strcat(psortingparam->filename, "/");
		strcat(psortingparam->filename, inName);
		psortingparam->isfile = TRUE;
	}
}