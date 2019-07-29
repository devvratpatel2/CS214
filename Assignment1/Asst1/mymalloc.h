#ifndef mymalloc_h
#define mymalloc_h

#define malloc(x) mymalloc((x), __FILE__, __LINE__)
#define free(x) myfree((x), __FILE__, __LINE__)
// My version of malloc which will malloc memory in my pseudo memory of size 5000
void *mymalloc(unsigned int size, char *file, int line);
// My version of free which will free memory from my pseudo memory of size 5000
void myfree(void *p, char *file, int line);

#endif