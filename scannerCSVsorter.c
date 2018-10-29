#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include "scannerCSVsorter.h"
#include <sys/mman.h>
#include <limits.h>
#include "mergesort.c"

static int *totalPro;

int main(int agrc, char *argv[]) {
    if (agrc < 3 || agrc > 7) {
        printf("Format of arguments is incorrect\n");
        return 1;
    }

    if (strcmp(argv[1], "-c") != 0) {
        printf("first argument is incorrect\n");
        return 1;
    }

    char *sortField = argv[2];
    char *indirectory = NULL;
    char *outdirectory = NULL;

    if (agrc == 3) {
        indirectory = ".";
        outdirectory = "srcDir";
    }
    if (agrc == 4) {
        if (strcmp(argv[3], "-o") == 0) {
            indirectory = ".";
            outdirectory = "currDir";
        } else {
            printf("The format of command line parameters is incorrect ");
            return 1;
        }
    }
    if (agrc == 5) {
        if (strcmp(argv[3], "-d") == 0 && strcmp(argv[4], "-o") != 0) {
            indirectory = argv[4];
            outdirectory = "srcDir";
        } else if (strcmp(argv[3], "-o") == 0 && strcmp(argv[4], "-d") != 0) {
            indirectory = ".";
            outdirectory = argv[4];
        } else {
            printf("second argument is incorrect\n");
            return 1;
        }
    }

    if (agrc == 6) {
        if (strcmp(argv[3], "-d") == 0 && strcmp(argv[4], "-o") != 0 && strcmp(argv[5], "-o") == 0) {
            indirectory = argv[4];
            outdirectory = "currDir";
        } else {
            printf("The format of command line parameters is incorrect\n");
            return 1;
        }
    }

    if (agrc == 7) {
        if (strcmp(argv[3], "-d") == 0 && strcmp(argv[4], "-o") != 0) {
            indirectory = argv[4];
        } else {
            printf("The format of command line parameters is incorrect\n");
            return 1;
        }
        if (strcmp(argv[5], "-o") == 0 && strcmp(argv[6], "-d") != 0) {
            outdirectory = argv[6];
        } else {
            printf("The format of command line parameters is incorrect\n");
            return 1;
        }
    }


    int parentPID = getpid();
    printf("Initial PID: %d\n", parentPID);
    printf("PIDS of all child processes: ");
    fflush(stdout);
    int dent;

    totalPro = mmap(NULL, sizeof *totalPro, PROT_READ | PROT_WRITE,
                    MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    *totalPro = 1;
    readingdirectory(indirectory, outdirectory, dent, sortField);
    printf("\ntotal process:%d\n", *totalPro);
    munmap(totalPro, sizeof *totalPro);
    return 0;
}


void readingdirectory(char *indirectory, char *outdirectory, int dent, char *sortField) {
    DIR *dir;
    struct dirent *dp;
    if (!(dir = opendir(indirectory))) {
        printf("%s directory does not exist\n", indirectory);
        return;
    }
    while ((dp = readdir(dir)) != NULL) {
        if (dp->d_name[0] == '.') {
            continue;
        }
        if (dp->d_type == DT_DIR) {
            char path[1024];
            snprintf(path, sizeof(path), "%s/%s", indirectory, dp->d_name);
            (*totalPro)++;
            pid_t pid = fork();
            if (pid > 0) {    
                printf("%d,", pid);
                fflush(stdout);
            } else if (pid == 0) {    
                readingdirectory(path, outdirectory, dent + 2, sortField);
                exit(EXIT_SUCCESS);
            }
        } else {
            char *fileExtension = strrchr(dp->d_name, '.');
            if (fileExtension == NULL) {
                continue;
            }
            if (strcmp(fileExtension + 1, "csv") == 0) {
                char path[1024];
                snprintf(path, sizeof(path), "%s/%s", indirectory, dp->d_name);
                (*totalPro)++;
                pid_t pid = fork();
                if (pid > 0) {    
                    printf("%d,", pid);
                    fflush(stdout);
                } else {
                    multiplesorts(dp->d_name, path, sortField, outdirectory);
                    exit(EXIT_SUCCESS);
                }
            }
        }
    }
    closedir(dir);
    int pid;
    while ((pid = wait(NULL)) != -1) {
        
    }
}

int multiplesorts(char *csvfilename, char *csvfilepath, char *sortField, char *outdirectory) {
    unsortedmovie **unsortedmovies = malloc(sizeof(struct unsortedmovie *) * 100000);
    int row;
    for (row = 0; row < 100000; row++) {
        unsortedmovies[row] = malloc(sizeof(unsortedmovie) * 10000);
    }
    int allrows;

    rowstorage(csvfilepath, unsortedmovies, &allrows);
    rowsplitting(unsortedmovies, allrows);


    int sorttoint;
    if (lookforfield(unsortedmovies, sortField, &sorttoint) == 1) {
        printf("\nthe field does not exist\n");
        freestructarr(unsortedmovies);
        _exit(0);
    }

    unsortedmovie **initialsort = malloc(sizeof(struct unsortedmovie *) * 100000);
    intialstructignore(unsortedmovies, initialsort, allrows);
    initializeMergeSort(initialsort, sorttoint, allrows - 1);

    outputsortedmovies(csvfilename, csvfilepath, sortField, unsortedmovies, initialsort,
                      allrows - 1, outdirectory);

    freestructarr(unsortedmovies);
    free(initialsort);
    return 0;
}

void
outputsortedmovies(char *csvfilename, char *csvfilepath, char *sortField,
                  unsortedmovie **unsortedmovies,
                  unsortedmovie **initialsort,
                  int allrows, char *outdirectory) {

    char *finalfile = malloc(sizeof(char) * 10000);
    FILE *fptr;
    if (strcmp(outdirectory, "srcDir") == 0) {
        formatsrcdirectory(finalfile, sortField, csvfilename);
        fptr = fopen(finalfile, "w");
    } else if (strcmp(outdirectory, "currDir") == 0) {
        formatcurrdirectory(finalfile, sortField, csvfilepath);
        fptr = fopen(finalfile, "w");
    } else {
        struct stat st = {0};
        if (stat(outdirectory, &st) == -1) {
            mkdir(outdirectory, 0700);
        }
        formatOutputNameinDir(finalfile, sortField, outdirectory, csvfilename);
        fptr = fopen(finalfile, "w");
    }
    int i;
    fprintf(fptr, "%s\n", unsortedmovies[0]->rowstring);
    for (i = 0; i < allrows; i++) {
        fprintf(fptr, "%s\n", initialsort[i]->rowstring);
    }
    fclose(fptr);
    free(finalfile);

}

void formatsrcdirectory(char *finalfile, char *sortField, char *csvfilename) {
    finalfile[0] = '\0';
    char *csvfilenameWithoutExtension = malloc(sizeof(char) * 100000);
    fileextensionremoval(csvfilename, csvfilenameWithoutExtension);
    strcpy(finalfile, csvfilenameWithoutExtension);
    strcat(finalfile, "-sorted-");
    strcat(finalfile, sortField);
    strcat(finalfile, ".csv");
    free(csvfilenameWithoutExtension);
}

void formatcurrdirectory(char *finalfile, char *sortField, char *csvfilepath) {
    finalfile[0] = '\0';
    char *csvfilepathWithoutExtension = malloc(sizeof(char) * 100000);
    fileextensionremoval(csvfilepath, csvfilepathWithoutExtension);
    strcpy(finalfile, csvfilepathWithoutExtension);
    strcat(finalfile, "-sorted-");
    strcat(finalfile, sortField);
    strcat(finalfile, ".csv");
    free(csvfilepathWithoutExtension);
}

void formatOutputNameinDir(char *finalfile, char *sortField, char *outdirectory, char *csvfilename) {
    finalfile[0] = '\0';
    char *csvfilenameWithoutExtension = malloc(sizeof(char) * 100000);
    fileextensionremoval(csvfilename, csvfilenameWithoutExtension);
    strcpy(finalfile, outdirectory);
    strcat(finalfile, "/");
    strcat(finalfile, csvfilenameWithoutExtension);
    strcat(finalfile, "-sorted-");
    strcat(finalfile, sortField);
    strcat(finalfile, ".csv");
    free(csvfilenameWithoutExtension);
}


void fileextensionremoval(char *fileName, char *csvfilenameWithoutExtension) {
    char *retstr;
    char *lastdot;
    if ((retstr = malloc(strlen(fileName) + 1)) == NULL)
        exit(0);
    strcpy (retstr, fileName);
    lastdot = strrchr(retstr, '.');
    if (lastdot != NULL)
        *lastdot = '\0';
    strcpy(csvfilenameWithoutExtension, retstr);
    free(retstr);
}


void rowstorage(char *csvfilepath, unsortedmovie **unsortedmovies, int *allrows) {
    FILE *fp;
    fp = fopen(csvfilepath, "r");
    if (fp == NULL) {
        printf("\nfilepath is %s", csvfilepath);
        perror("Error opening file");
        _exit(0);
    }

    *allrows = 0;
    char *currRow = malloc(sizeof(char) * 50000);
    while (fgets(currRow, 50000, fp)) {
        currRow[strcspn(currRow, "\n")] = 0;

        unsortedmovies[*allrows]->rowstring = malloc(sizeof(char) * 5000);
        strcpy(unsortedmovies[*allrows]->rowstring, currRow);
        (*allrows)++;
    }
    free(currRow);
    fclose(fp);
}

int rowsplitting(unsortedmovie **unsortedmovies, int allrows) {
    int currRowNum;
    for (currRowNum = 0; currRowNum < allrows; currRowNum++) {
        onerowsplitting(unsortedmovies, currRowNum);
    }
    return 0;
}

int onerowsplitting(unsortedmovie **unsortedmovies, int currRowNum) {
    char *oneRow = malloc(sizeof(char) * 50000);
    strcpy(oneRow, unsortedmovies[currRowNum]->rowstring);
    
    spaceremoval(oneRow);
    allquotefields(oneRow);

    unsortedmovies[currRowNum]->rowfieldarray = firstfieldcheck(oneRow);
    free(oneRow);
    return 0;
}

void spaceremoval(char *oneRow) {
    char *i = oneRow;
    char *j = oneRow;
    while (*j != 0) {
        *i = *j++;
        if (*i != ' ')
            i++;
    }
    *i = 0;
}

void allquotefields(char *oneRow) {
    char *tmpString = malloc(sizeof(char) * 50000);;
    char *token;
    char *beginStr = malloc(sizeof(char) * 50000);
    char *endStr = malloc(sizeof(char) * 50000);
    char *midStr = malloc(sizeof(char) * 50000);

    strcpy(tmpString, oneRow);
    token = strtok(tmpString, "\"");
    strcpy(beginStr, token);
    token = strtok(NULL, "\"");

    if (token) {
        commaremoval(token);
        strcpy(midStr, token);
        token = strtok(NULL, "\"");
        strcpy(endStr, token);

        tmpString[0] = '\0';
        strcat(tmpString, beginStr);
        strcat(tmpString, midStr);
        strcat(tmpString, endStr);
        strcpy(oneRow, tmpString);
    }
    
    free(tmpString);
    free(beginStr);
    free(endStr);
    free(midStr);
}

void commaremoval(char *subString) {
    char *r, *w;
    for (w = r = subString; *r; r++) {
        if (*r != ',') {
            *w++ = *r;
        }
    }
    *w = '\0';
}

char **firstfieldcheck(char *oneRow) {
    char **oneRowFields = malloc(sizeof(char *) * 100);
    int i;
    for (i = 0; i < 100; i++) {
        oneRowFields[i] = malloc(sizeof(char) * 50000);
    }
    char *tmpString = malloc(sizeof(char) * 5000);
    strcpy(tmpString, oneRow);
    checkFirstField(tmpString);

    char *tmp = malloc(sizeof(char) * 1000);
    char *comma = malloc(sizeof(char) * 100);
    strcpy(comma, ",");
    int j = 0;
    while (sscanf(tmpString, "%[^,],", tmp) != EOF) {
        
        firstremoval(tmpString, tmp);
        firstremoval(tmpString, comma);
        checkFirstField(tmpString);
        strcpy(oneRowFields[j], tmp);
        j++;
    }
    
    if (j != 28) {
        _exit(0);
    }
    free(tmp);
    free(comma);
    free(tmpString);
    return oneRowFields;
}

void checkFirstField(char *tmpString) {    
    char firstChar = tmpString[0];
    if (firstChar == ',') {
        char *newRow = malloc(sizeof(char) * 80000);
        strcpy(newRow, "!");
        strcat(newRow, tmpString);
        strcpy(tmpString, newRow);
        free(newRow);
    }

}

void firstremoval(char *str, const char *toRemove) {
    int i, j;
    int len, removeLen;
    int found = 0;

    len = strlen(str);
    removeLen = strlen(toRemove);

    for (i = 0; i < len; i++) {
        found = 1;
        for (j = 0; j < removeLen; j++) {
            if (str[i + j] != toRemove[j]) {
                found = 0;
                break;
            }
        }
        if (found == 1) {
            for (j = i; j <= len - removeLen; j++) {
                str[j] = str[j + removeLen];
            }
            break;
        }
    }
}

int lookforfield(unsortedmovie **unsortedmovies, char *sortField, int *sorttoint) {
    int fieldNum;
    *sorttoint = -1;
    int isExistence = 1;  // not exist
    for (fieldNum = 0; fieldNum < 28; fieldNum++) {
        char *currField = malloc(sizeof(char) * 200);
        strcpy(currField, unsortedmovies[0]->rowfieldarray[fieldNum]);
        deletecarriage(currField);
        if (strcmp(currField, sortField) == 0) {
            isExistence = 0;
            *sorttoint = fieldNum + 1;
            break;
        }
        free(currField);
    }

    return isExistence;
}

void deletecarriage(char *str) {
    char *src, *dst;
    for (src = dst = str; *src != '\0'; src++) {
        *dst = *src;
        if (*dst != '\r') dst++;
    }
    *dst = '\0';
}

void freestructarr(unsortedmovie **unsortedmovies) {
    int row;
    for (row = 0; row < 100000; row++) {
        free(unsortedmovies[row]->rowfieldarray);
        free(unsortedmovies[row]->rowstring);
        free(unsortedmovies[row]);
    }
    free(unsortedmovies);
}


void intialstructignore(unsortedmovie **unsortedmovies, unsortedmovie **initialsort, int allrows) {
    int i;
    for (i = 1; i < allrows; i++) {
        initialsort[i - 1] = unsortedmovies[i];
    }
}
