#ifndef MULTITHREADSORTER_H
#define MULTITHREADSORTER_H


#define nullindex				1				
#define intindex			2				
#define floatindex			3				
#define stringindex			4				


#define arraysize				128
#define arraysize_ADD		arraysize + 3
#define tempindex								arraysize
#define tempindex1							arraysize + 1
#define outputindex								arraysize + 2


#define TRUE							1
#define FALSE							0

#define SUCCESS						0
#define FAILURE						1

#define comma 					','
#define quote1 					'\"'
#define DOT								'.'

#define initialsize				1024

#define keylength		128

typedef struct BUFFER {
	size_t	length;
	char*	data;
} BUFFER;

char* initBuffer(BUFFER* pBuf, int initSize);



char* sizebuffer(BUFFER* pBuf, int size);


char* getbuffer(BUFFER* pBuf, int delta);

void freeBuffer(BUFFER* pBuf);


int stripnewline(char* pBuf);


int strcicmp(char const *input1, char const *input2);

typedef struct sortingparam {
	char filename[3 * maxpath + maxname + 1];	
	char sortingKeys[keylength];						
	int 	isfile;												
} sortingparam;


void setsortingparam(const char* parentpath, const char* inPath, const char* inName, const char* sortinginput, sortingparam* psortingparam);



arrayrecord* sort(sortingparam* psortingparam);


void cleanUparrayrecord(Parrayrecord* pparrayrecord);


int insertOne(Parrayrecord* pparrayrecord, Parrayrecord ppOnearrayrecord);

void swap(Parrayrecord* ppInarrayrecord, Parrayrecord* ppOutarrayrecord);

Parrayrecord removeOne(Parrayrecord* pparrayrecord);




typedef struct Record {
	char*		datarecord;
	char* 		pkey;					
	char*		pkeyTerm;			
	char			chHold;					
	long 			lKey;					
	float			fKey;					
	int 			idxType;				
} Record;



Record* setdatarecord(Record* pRec, char* store, char* buffer, skeyindex* psk);



int compare(void* pData1, void* pData2);


typedef struct arrayrecord {
	char*						pRaw;
	Record** 					pointerarray;					
	Record* 					recDataArray;				
	int 		 					iCapacity;						
	int		 					iSize;							
	
	skeyindex 				sKI;
	
	struct arrayrecord*	next;
} arrayrecord;


typedef arrayrecord *Parrayrecord;



int initializearrayrecord(arrayrecord* parrayrecord, int initialCapacity);



Record* getOneRecord(arrayrecord* parrayrecord);


void resizePtrArray(arrayrecord* parrayrecord, int size);



void cleanarrayrecord(arrayrecord* parrayrecord);

typedef int (*comparedata)(void* pData1, void* pData2);


void merge(void* arr1[], int n1, void* arr2[], int n2, void* arr[], comparedata pFuncCompare);


void mergeSort(void* arr[], int l, int r, comparedata pFuncCompare);






arrayrecord* fileordirec(void* input);



void* threadFunc(void* input);



void sortAndMergeFiles(Parrayrecord pHead, arrayrecord** pparrayrecord, int count);



void mergeTwo(Parrayrecord p1, Parrayrecord p2, Parrayrecord pMerged);



int writearrayrecordToFile(char* header, arrayrecord* parrayrecord, FILE *file);


typedef struct skeyindex {
	int index;
} skeyindex;



int getsortkey(char* input, char* key, skeyindex* psk);


typedef struct Tokenizer {
	char*			pToken;		
	char*			currptr;								
	int			comma;										
	int			quote;											
	int 			isEnded;											
} Tokenizer;

void initTokenizer(Tokenizer* pTokenizer, char* _currptr, int _comma, int _quote);


char* getNextToken(Tokenizer* pTokenizer);

#endif