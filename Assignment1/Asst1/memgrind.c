#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <sys/time.h>
#include "mymalloc.h"

int main(int argc, char ** argv){
    
    printf("\n");
    
    // keeps track of the iterations 
    double a = 0;
    double b = 0;
    double c = 0;
    double d = 0;
    double e = 0;
    double f = 0;
    

    srand(time(NULL));
    
    // ERRORS 
    
    // Error for freeing memory that was not allocated by malloc
    printf("\nerror check one\n");
    
    struct timeval begin, end;
    gettimeofday(&begin, NULL);
    
    
    char * p = (char *)malloc( 200 );
    
    free( p + 10 );
    
    gettimeofday(&end, NULL);
    double elapsed = (end.tv_sec - begin.tv_sec) + ((end.tv_usec - begin.tv_usec)/1000000.0);
    printf("error check one time: %.15lf\n", elapsed);
    
    // Freeing a pointer twice 
    printf("\nerror check two\n");
    
    gettimeofday(&begin, NULL);
    
    free(p);
    free(p);
    
    gettimeofday(&end, NULL);
    elapsed = (end.tv_sec - begin.tv_sec) + ((end.tv_usec - begin.tv_usec)/1000000.0);
    printf("error check two time: %.15lf\n", elapsed);
    
    // Saturation of dynamic memory
    printf("\nerror check three\n");
    
    gettimeofday(&begin, NULL);
    
    p = malloc (5000);
    
    p = malloc (4999);
    
    
    gettimeofday(&end, NULL);
    elapsed = (end.tv_sec - begin.tv_sec) + ((end.tv_usec - begin.tv_usec)/1000000.0);
    printf("error check three time: %.15lf\n", elapsed);
    
    // Performs the test cases 100 times and gets the average time 
    int count =0;
    while(count !=100){
        
        srand(time(NULL));
        
        // TEST A
        
        gettimeofday(&begin, NULL);
        
        int i =0;
        
        for(i = 0; i < 150; i++){
            
            p = malloc(1);
            free(p);
        }
        
        gettimeofday(&end, NULL);
        elapsed = (end.tv_sec - begin.tv_sec) + ((end.tv_usec - begin.tv_usec)/1000000.0);
        a+= elapsed;
        
        
        // TEST B

        gettimeofday(&begin, NULL);
        
        void * array [150];
        
        for(i = 0; i < 150; i++){
            
            array[i] = malloc(1);
        }
        for(i = 0; i < 150; i++){
            
            free(array[i]);
        }
        
        gettimeofday(&end, NULL);
        elapsed = (end.tv_sec - begin.tv_sec) + ((end.tv_usec - begin.tv_usec)/1000000.0);
        b+= elapsed;
        
        
        // TEST C 

        int garbage = 8;
        for(i=0;i<150;i++){
            
            array[i] = &garbage;
            
        }
        
        gettimeofday(&begin, NULL);
        
        int malloced = 0;
        int occupied = 0;
        int done =0;
        
        while(!done){
            
            int r = rand()% 20;
            
            if(malloced != 150 && r > 15){
                int index = rand() % 150;
                
                if(array[index] == &garbage){
                    array[index] = malloc(1);
                    malloced ++;
                    occupied++;
                }
                
            }else{
                int index = rand() % (150);
                
                if(array[index] != &garbage){
                    free(array[index]);
                    array[index] = &garbage;
                    occupied--;
                }
            }
            if(malloced == 150 && occupied == 0){
                
                done =1 ;
                
            }
        }
        
        gettimeofday(&end, NULL);
        elapsed = (end.tv_sec - begin.tv_sec) + ((end.tv_usec - begin.tv_usec)/1000000.0);
        c+= elapsed;



      /* E,F: Two more workloads of your choosing

      Both the test cases will run for 100 iterations and the average time will be shown at the end.
        
  E.  In the first  test case, pick any random number of bytes ranging from 2 to 64. Now, malloc until you can and 
      once we are done with that, free every other pointer so : 0, 2, 4 ......
      Now pick another number that will fit in the free space without splitting to occur and re malloc the same freed 
      pointers. Once that is done, free all the nodes. 
      The range is only 2-64 as if we get 1, then 1-0 will 0 and that will cause an error.

        
  F.  In this test case, pick any random number of bytes ranging from 10-64. Now, malloc until you can and 
      once we are done with that, free every other pointer so : 0, 2, 4 ....
      Now pick another number that will fit in the free space without splitting to occur and re malloc
      the same freed pointers. Once that is dine, free all the nodes.
      */
        
        //TEST D

        gettimeofday(&begin, NULL);
        
        malloced = 0;
        occupied = 0;
        done =0;
        
        while(!done){
            
            int r = rand()% 20;
            
            if(malloced != 150 && r > 15){
                int index = rand() % 150;
                
                if(array[index] == &garbage){
                    
                    int size = (rand() % 64) + 1;
                    array[index] = malloc(size);
                    malloced ++;
                    occupied++;
                }
                
            }else{
                int index = rand() % (150);
                
                if(array[index] != &garbage){
                    free(array[index]);
                    array[index] = &garbage;
                    occupied--;
                }
            }
            if(malloced == 150 && occupied == 0){
                
                done =1 ;
                
            }
        }
        
        gettimeofday(&end, NULL);
        elapsed = (end.tv_sec - begin.tv_sec) + ((end.tv_usec - begin.tv_usec)/1000000.0);
        d+= elapsed;
        

        // TEST E (Description in testplan.txt)
        
        gettimeofday(&begin, NULL);
        
        int  size = (rand() % 63) +2;
        
        int capacity = (4997)/ (size +1) -1;
        
        void * pointers[capacity];
        
        for(i = 0; i<capacity; i++){
            
            pointers[i] = malloc(size);
        }
        
        for(i = 0; i<capacity; i+=2){
            
            free(pointers[i]);
        }
        int freeChunks = size - 1;
        
        for(i = 0; i<capacity; i+=2){
            
            pointers[i] = malloc (freeChunks);
        }
        for(i = 0; i<capacity; i++){
            
            free(pointers[i]);
        }
        
        gettimeofday(&end, NULL);
        elapsed = (end.tv_sec - begin.tv_sec) + ((end.tv_usec - begin.tv_usec)/1000000.0);
        e+= elapsed;

        // TEST F (Description in test plan.txt)

        size = (rand() % 55) +10;
        
        capacity = (4997)/ (size +1) -1;
        
        void * pointers2[capacity];
        
        for(i = 0; i<capacity; i++){
            
            pointers2[i] = malloc(size);
        }
        
        for(i = 0; i<capacity; i+=2){
            
            free(pointers2[i]);
        }
        freeChunks = size -5;
        
        for(i = 0; i<capacity; i+=2){
            
            pointers2[i] = malloc (freeChunks);
        }
        for(i = 0; i<capacity; i++){
            
            free(pointers2[i]);
        }
        gettimeofday(&end, NULL);
        elapsed = (end.tv_sec - begin.tv_sec) + ((end.tv_usec - begin.tv_usec)/1000000.0);
        f += elapsed;
        
        count++;
    }
    
    printf("\nAll test cases are done\n"); // Displays the average time for all the tests which will be shown in the readme.txt
    printf("Average time for A: %.10lf sec\n", a/100);
    printf("Average time for B : %.10lf sec\n", b/100);
    printf("Average time for E : %.10lf sec\n", c/100);
    printf("Average time for D : %.10lf sec\n", d/100);
    printf("Average time for E : %.10lf sec\n", e/100);
    printf("Average time for F : %.10lf sec\n", f/100);
    
    char * endMessage = (char*) malloc(sizeof(char) * 40);
    
    endMessage = "The total time for all the test cases was: ";
    
    double totalTime = a + b + c + d + e + f;
    
    char * seconds = (char*) malloc(sizeof(char) * 9);
    
    seconds = " sec";
    printf("\n%s%.10lf%s\n\n", endMessage, totalTime, seconds);
    
    return 0;
}