#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include "mymalloc.h"

static char myblock[5000]; // Size of my pseudo memory 

/* Mallocs memory to my pseudo memory and returns a poinyed to the new index of the allocated memory. 
 * The metadat is one charecter long and I used a short to keep track of the free space therefore the total space is 4997.
 */
void *mymalloc(unsigned int size, char *file, int line){ 
    
    /* pointer to the first two bytes in memory that keep track of the avaliable space left in the array */
    short *  totalSpace = (short*)(myblock);
    
    
    if(*totalSpace == 0 && size <= 4997){ 
        
        totalSpace = (short*)(myblock);
        *totalSpace =(short) (4997 - size);
        myblock[2]  = '~';
        
        if(*totalSpace>=3){
            myblock[2 + size +1] = '|';
            *totalSpace -= 1;
        }
        
        void * resultAdd = ((void*)(myblock + 3));
        return resultAdd;
    }
    
    
    if((*totalSpace - size) <= 0 || size > *totalSpace  || size <=0){
        
        printf("Can't allocate %d bytes in FILE: '%s' on LINE: '%d'\n",size, file, line);
        return 0;
    }
    int i =0; 
    int index =0;
    int capacity =0;
    
    for(i=2; i<5000; i++){
        
        if(capacity == 0 && myblock[i] == '|'){
            index = i;
            capacity = 1;
            continue;
        }
        if(capacity == 1 && (myblock[i] == '~' || i == 4999)){
           
            if( (i - index -1 ) >= size){
                
                myblock[index] = '~';
                *totalSpace -= (short) size;
                if( (i - index -1 ) == size){
                    
                    void * resultAdd = ((void*)(myblock + index + 1));
                    return resultAdd;
                }
                
                if(i == 4999 && (index + size ) <= 4996){
                    
                    myblock[index + size +1] = '|';
                    *totalSpace -= 1;
                    
                }else if((i - index - 1 - size ) >=2){
                    
                    myblock[index +1 + size] = '|';
                    *totalSpace -= 1;
                    
                }else if((i - index -1 - size) < 2){
                    
                    *totalSpace -= (short)(i - index -1 - size);
                }
                
                
                void * resultAdd = ((void*)(myblock + index + 1));
                return resultAdd;
                
            }else{
                capacity = 0;
                i++;
            }
        }
    }

    printf("Can't allocate %d bytes in FILE: '%s' on LINE: '%d'\n",size, file, line);
    return 0;
}

void myfree(void *p, char *file, int line){
    
    if(p == NULL || (char*)p > (myblock + 4999) || (char*)p < myblock || ((char*)p)[-1] == 0 || ((char*)p)[-1] != '~'){
        
        printf("Error: Memory was not allocated pointer in %s on line: %d, so not freed \n",file, line);
        return;
    }
    short *  totalSpace = (short*)(myblock);
    int i = 0; 
    int j=-2; 
    char * left = (char *) p;
    char * right = (char *) p;
    int isLeftFinished =0;
    int isRightFinished=0;
    
    ((char*)p)[-1] = '|';
    while (!isLeftFinished || !isRightFinished ){
        if(isRightFinished ==0 &&  (char*)(right+i) == (char*)(myblock + 5000)){
            
            *totalSpace += i;
            isRightFinished = 1;
        }
        if(isRightFinished ==0 && (char)right[i] == '~'){
            
            *totalSpace += i;
            isRightFinished = 1;
        }
        if(isLeftFinished ==0 && ((char)left[j] == '~' || (char*)(left+j) == (char*)(myblock+1)) ){
            
            isLeftFinished = 1;
        }
        if(isRightFinished ==0 && ((char*)right)[i] == '|'){
            
            *totalSpace += (short)(i + 1) ;
            right[i] =0;
            isRightFinished =1;
        }
        if(isLeftFinished ==0 && left[j] == '|'){
            if(isRightFinished){
                *totalSpace += 1 ;
                ((char*)p)[-1] = 0;
                isLeftFinished =1;
                p = NULL;
                
            }else{
                
                j++;
            }
        }
        
        i++;
        j--;
        
    }
    // Frees the totalSpace
    if(*totalSpace == 4997){
        myblock[1] = 0;
        myblock[2] = 0;
        myblock[3] = 0;
        myblock[4] = 0;
        *totalSpace = 0;
    }
    
    return;
}
