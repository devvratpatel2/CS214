#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>



void initializeMergeSort(unsortedmovie **initialsort, int sorttoint, int allrows);

void mergeSort(unsortedmovie **initialsort, int allrows, int originalRows, int sorttoint);

void merge(unsortedmovie **initialsort, unsortedmovie **leftunsortedmovies, int leftCount, unsortedmovie **rightunsortedmovies,
           int rightCount, int sorttoint);

void initializeMergeSort(unsortedmovie **initialsort, int sorttoint, int allrows) {
    mergeSort(initialsort, allrows, allrows, sorttoint);

}

void mergeSort(unsortedmovie **initialsort, int allrows, int originalRows, int sorttoint) {
    if (allrows < 2) return;
    int middle = allrows / 2;

    int row;
    unsortedmovie **leftArray = malloc(sizeof(struct unsortedmovie *) * middle);
    unsortedmovie **rightArray = malloc(sizeof(struct unsortedmovie *) * (allrows - middle));

    int i;
    for (i = 0; i < middle; i++) {
        leftArray[i] = initialsort[i];
    }

    for (i = middle; i < allrows; i++) {
        rightArray[i - middle] = initialsort[i];
    }

    mergeSort(leftArray, middle, originalRows, sorttoint);
    mergeSort(rightArray, allrows - middle, originalRows, sorttoint);
    merge(initialsort, leftArray, middle, rightArray, allrows - middle, sorttoint);
    
    free(leftArray);
    free(rightArray);

}

void merge(unsortedmovie **initialsort, unsortedmovie **leftunsortedmovies, int leftCount, unsortedmovie **rightunsortedmovies,
           int rightCount, int sorttoint) {
    int leftIndex, rightIndex, mergedIndex;
    leftIndex = 0;
    rightIndex = 0;
    mergedIndex = 0;

    if (sorttoint == 1 || sorttoint == 2 || sorttoint == 7 || sorttoint == 10 ||
        sorttoint == 11 || sorttoint == 12 || sorttoint == 15 || sorttoint == 17 ||
        sorttoint == 18 || sorttoint == 20 || sorttoint == 21 || sorttoint == 22) {
        while (leftIndex < leftCount && rightIndex < rightCount) {

            int leftLength = strlen(leftunsortedmovies[leftIndex]->rowfieldarray[sorttoint - 1]);
            int rightLength = strlen(rightunsortedmovies[rightIndex]->rowfieldarray[sorttoint - 1]);


            if (strcmp(leftunsortedmovies[leftIndex]->rowfieldarray[sorttoint - 1],
                       rightunsortedmovies[rightIndex]->rowfieldarray[sorttoint - 1]) == 0) {
                initialsort[mergedIndex++] = rightunsortedmovies[rightIndex++];
                continue;
            }

            int strLength = leftLength;
            if (leftLength > rightLength)
                strLength = rightLength;
            int j = 0;
            for (j = 0; j < strLength; j++) {
                char leftChar = leftunsortedmovies[leftIndex]->rowfieldarray[sorttoint - 1][j];
                char rightChar = rightunsortedmovies[rightIndex]->rowfieldarray[sorttoint - 1][j];
                int leftCharToInt = leftChar - '0';
                int rightCharToInt = rightChar - '0';

                int leftIsUpper = isupper((int) leftChar);
                int rightIsUpper = isupper((int) rightChar);

                if (leftIsUpper != 0 && rightIsUpper != 0) {   //both Upper
                    if (leftCharToInt < rightCharToInt) {
                        initialsort[mergedIndex++] = leftunsortedmovies[leftIndex++];
                        break;
                    } else if (leftCharToInt > rightCharToInt) {
                        initialsort[mergedIndex++] = rightunsortedmovies[rightIndex++];
                        break;
                    } else {
                        if (j + 1 == strLength) {
                            if (leftLength < rightLength) {
                                initialsort[mergedIndex++] = leftunsortedmovies[leftIndex++];
                            }else{
                                initialsort[mergedIndex++] = rightunsortedmovies[rightIndex++];
                            }
                        }
                        continue;
                    }
                } else if (leftIsUpper == 0 && rightIsUpper == 0) {   //both Lower
                    if (leftCharToInt < rightCharToInt) {
                        initialsort[mergedIndex++] = leftunsortedmovies[leftIndex++];
                        break;
                    } else if (leftCharToInt > rightCharToInt) {
                        initialsort[mergedIndex++] = rightunsortedmovies[rightIndex++];
                        break;
                    } else {
                        if (j + 1 == strLength) {
                            if (leftLength < rightLength) {
                                initialsort[mergedIndex++] = leftunsortedmovies[leftIndex++];
                            }else{
                                initialsort[mergedIndex++] = rightunsortedmovies[rightIndex++];
                            }
                        }
                        continue;
                    }
                } else if (leftIsUpper != 0 && rightIsUpper == 0) {   // left Upper, right Lower
                    if (leftCharToInt + 32 == rightCharToInt) {
                        initialsort[mergedIndex++] = leftunsortedmovies[leftIndex++];
                        break;
                    } else if (leftCharToInt + 32 < rightCharToInt) {
                        initialsort[mergedIndex++] = leftunsortedmovies[leftIndex++];
                        break;
                    } else {
                        initialsort[mergedIndex++] = rightunsortedmovies[rightIndex++];
                        break;
                    }
                } else if (leftIsUpper == 0 && rightIsUpper != 0) {    // left Lower, right Upper
                    if (rightCharToInt + 32 == leftCharToInt) {
                        initialsort[mergedIndex++] = rightunsortedmovies[rightIndex++];
                        break;
                    } else if (rightCharToInt + 32 < leftCharToInt) {
                        initialsort[mergedIndex++] = rightunsortedmovies[rightIndex++];
                        break;
                    } else {
                        initialsort[mergedIndex++] = leftunsortedmovies[leftIndex++];
                        break;
                    }
                }
            }
        }
    } else {
        while (leftIndex < leftCount && rightIndex < rightCount) {
            if (strtod(leftunsortedmovies[leftIndex]->rowfieldarray[sorttoint - 1], NULL) <
                strtod(rightunsortedmovies[rightIndex]->rowfieldarray[sorttoint - 1], NULL)) {
                initialsort[mergedIndex++] = leftunsortedmovies[leftIndex++];
            } else {
                initialsort[mergedIndex++] = rightunsortedmovies[rightIndex++];
            }
        }
    }

    while (leftIndex < leftCount) {
        initialsort[mergedIndex++] = leftunsortedmovies[leftIndex++];
    }
    while (rightIndex < rightCount) {
        initialsort[mergedIndex++] = rightunsortedmovies[rightIndex++];
    }
}