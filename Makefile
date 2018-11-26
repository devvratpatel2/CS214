all:multiThreadSorter

sorter: multiThreadSorter_thread.h lls.c mergesort.c allrecords.c recordarrs.c sortingkeys.c tokenizer.c multiThreadSorter_thread.c extra.c simplecsvsorter.c
	gcc -Wall -Werror -g -fsanitize=address lls.c mergesort.c allrecords.c recordarrs.c sortingkeys.c tokenizer.c multiThreadSorter_thread.c extra.c simplecsvsorter.c -o multiThreadSorter_thread

clean:
	rm -rf multiThreadSorter