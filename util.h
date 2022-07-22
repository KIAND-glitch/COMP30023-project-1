#ifndef UTIL_H
#define UTIL_H

// counts the number of distinct elements in an array
int countDistinctElement(int arr[], int size);

// returns the most repeated elements count by check the number of times
// each element is present i.e count, and finding the max count
// by comparing all counts
int MostRepeatingElementCount(int arr[], int size);

// checks if an element is present in the array and returns its index, else returns -1 
int isElementPresent(int arr[], int size, int el);

#endif
