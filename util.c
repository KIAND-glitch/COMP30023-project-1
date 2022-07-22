#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

// counts the number of distinct elements in an array
int countDistinctElement(int arr[], int size)     
{
    // count stores the number of distinct elements
    int j, count = 1;
    //Traverse the array
    for (int i = 1; i < size; i++)      
    {
       for (j = 0; j < i; j++)   
       {
         // check for duplicate elements 
         if (arr[i] == arr[j])    
         {
            break;             
         }
       }
       if (i == j)
       {
         count++;     
       }
    }
    return count;      
}

// returns the most repeated element by check the number of times
// each element is present i.e count, and finding the max count
// by comparing all counts
int MostRepeatingElementCount(int arr[], int size) 
{
    int i, j, count;
    int maxCount = 0;

    for(i = 0; i< size; i++)   
    {
        count = 1;
        // check for duplicate elements
        for(j = i+1; j < size; j++)  
        {
            if(arr[j] == arr[i])
            {
                count++;     

                if(count > maxCount)
                {
                    maxCount = count;
                }
            }
        }
    }
    if(maxCount == 0){
        maxCount++;
    }
    return maxCount;
}

// checks if an element is present in the array and returns its index, else returns -1 
int isElementPresent(int arr[], int size, int el){
  for(int i = 0; i < size; i++){
    if(arr[i]==el){
      return i;
    }
  }
  return -1;
}


