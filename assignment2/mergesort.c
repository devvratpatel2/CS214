#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "multiThreadSorter_thread.h"



void merge(void* arr1[], int n1, void* arr2[], int n2, void* arr[], comparedata pFuncCompare) {
	int i = 0; 																
	int j = 0; 																
	int k = 0;																
	while (i < n1 && j < n2) {
		if (pFuncCompare(arr1[i], arr2[j]) <= 0) {
			arr[k] = arr1[i];
			i++;
		}
		else {
			arr[k] = arr2[j];
			j++;
		}
		k++;
	}
	
	while (i < n1) {
		arr[k] = arr1[i];
		i++;
		k++;
	}

	
	while (j < n2) {
		arr[k] = arr2[j];
		j++;
		k++;
	}
}



void mergeSort(void* arr[], int l, int r, comparedata pFuncCompare) {
	if (l < r) {
		
		int m = l + (r - l) / 2;
		
		mergeSort(arr, l, m, pFuncCompare);
		mergeSort(arr, m + 1, r, pFuncCompare);
		
		int i, j;
		int n1 = m - l + 1;										
		int n2 = r - m;											
		
		void* tempArr1[n1];
		void* tempArr2[n2];
		
		for (i = 0; i < n1; i++) {
			tempArr1[i] = arr[l + i];
		}
		for (j = 0; j < n2; j++) {
			tempArr2[j] = arr[m + 1 + j];
		}
		
		merge(tempArr1, n1, tempArr2, n2, arr + l, pFuncCompare);
	}
}
