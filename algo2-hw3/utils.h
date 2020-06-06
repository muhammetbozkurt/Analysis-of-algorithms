/*
Muhammet Bozkurt
150150142
*/
#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <vector> 
#include <bits/stdc++.h>

using namespace std;


void insertionSort(vector<int> &arr, vector<int> &sequence ){ 
    /*
    sort both freqs and sequences
    */
    int i, key, j, temp, n = arr.size();  
    for (i = 1; i < n; i++) 
    {  
        key = arr[i];
        temp = sequence[i]; 
        j = i - 1;  
  
        while (j >= 0 && arr[j] > key) 
        {  
            arr[j + 1] = arr[j];  
            sequence[j + 1] = sequence[j];  
            j = j - 1;  
        }  
        arr[j + 1] = key;
        sequence[j + 1] = temp;
    }  
}  


int max(int a, int b){ 
    return (a > b)? a : b;
} 


int min(int x, int y, int z) 
{ 
    return min(min(x, y), z); 
} 

int levelsteinDP(vector<int> &arr1, vector<int>& arr2, int m, int n, int dp[][1000]) 
{ 
    if (m == 0) //base case
        return n; 
  
    if (n == 0)  //base case
        return m; 
  
    if (dp[m - 1][n - 1] != -1) 
        return dp[m - 1][n - 1]; 
 
    if (arr1[m - 1] == arr2[n - 1]) 
        return dp[m - 1][n - 1] = levelsteinDP(arr1, arr2, m - 1, n - 1, dp); 
  
    return dp[m - 1][n - 1] = 1 + min(levelsteinDP(arr1, arr2, m, n - 1, dp), // Insert 
                                      levelsteinDP(arr1, arr2, m - 1, n, dp), // Remove 
                                      levelsteinDP(arr1, arr2, m - 1, n - 1, dp) // Replace 
                                      ); 
}

int levelstein_caller(vector<int> &arr1, vector<int> &arr2){ //to make call levelsteinDP create memoization array
    
    int dp[arr1.size()][1000]; 

    memset(dp, -1, sizeof dp); 
    return levelsteinDP(arr1, arr2, arr1.size(), arr1.size(), dp);
}
  
 

int covarage(vector<int> x){ // to detect covareage of test cases
    int res = 0, size = x.size();
    for(int i=0; i<size; i++)
        if(x[i] != 0)
            res++;
    return res;
}

#endif