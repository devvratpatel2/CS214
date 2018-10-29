#ifndef CSV_SORTER_H
#define CSV_SORTER_H
	
typedef struct movie {
    char **rowfieldarray;
    char *rowstring;
} unsortedmovie;

void readingdirectory(char *indirectory, char *outdirectory, int dent, char *sortingfield);

int multiplesorts(char *csvfilename, char *csvfilepath, char *sortingfield, char *outdirectory);

void
outputsortedmovies(char *csvfilename, char *csvfilepath, char *sortingfield,
                  unsortedmovie **unsortedmovies,
                  unsortedmovie **intialsort,
                  int allrows, char *outdirectory);

void formatsrcdirectory(char *finalfile, char *sortingfield, char *csvfilename);

void formatcurrdirectory(char *finalfile, char *sortingfield, char *csvfilepath);

void formatoutindrr(char *finalfile, char *sortingfield, char *outdirectory, char *csvfilename);

void fileextensionremoval(char *fileName, char *noextensionfile);

void rowstorage(char *csvfilepath, unsortedmovie **unsortedmovies, int *allrows);

int rowsplitting(unsortedmovie **unsortedmovies, int allrows);

int onerowsplitting(unsortedmovie **unsortedmovies, int currRowNum);

void spaceremoval(char *oneRow);

void allquotefields(char *oneRow);

void commaremoval(char *subString);

char **rowturnsfield(char *oneRow);

void firstfieldcheck(char *tmpString);

void firstremoval(char *str, const char *toRemove);

int lookforfield(unsortedmovie **unsortedmovies, char *sortingfield, int *sorttoint);

void deletecarriage(char *str);

void freestructarr(unsortedmovie **unsortedmovies);

void initialstructignore(unsortedmovie **unsortedmovies, unsortedmovie **intialsort, int allrows);
#endif