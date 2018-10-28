all: scannerCSVsorter

sorter:  
	gcc -Wall -Werror -fsanitize=address -o scannerCSVsorter scannerCSVsorter.c mergesort.c
	
clean:
	rm -rf scannerCSVsorter mergesort.o