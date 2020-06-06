/*
Muhammet Bozkurt
150150142
*/
#ifndef SUITESCHEDULER_H
#define SUITESCHEDULER_H

#include"TestSuite.h"

using namespace std;

class SuiteScheduler{
private:
    vector<TestSuite> array;
    int totalTime;
    int* memoization;
    int size;
    int res_buggy;
public:
    vector<TestSuite> result;
    void suite_reader(const char*  file_name);
    SuiteScheduler(const char*  path_to_data);
    vector<TestSuite> opt();
    void printRes();
    int get_res_bug();
};

SuiteScheduler::SuiteScheduler(const char*  path_to_data){
   
    suite_reader(path_to_data);
    size = array.size();
    memoization = new int[size];
    
}

void SuiteScheduler::printRes(){
    int bug=0 , time=0 ;
    cout<<"Id\tBug\tTime"<<endl;
    for(int i = 0; i <result.size(); i++){
        cout<<result[i].get_testId()<<"\t"<<result[i].get_bug()<<"\t"<<result[i].get_time()<<endl;
        bug += result[i].get_bug();
        time += result[i].get_time();
    }
    cout<<endl;
    cout<<"Total consumed time: "<<time<<endl;
    cout<<"Total collected bug: "<<bug<<endl<<endl;
}


void SuiteScheduler::suite_reader(const char*  file_name){
    /*
    reading file in proper manner and put them 
    */
    vector<string> freqs;
    string buffer(file_name), id, bugs, time, freq;
    ifstream file(file_name);
    
    getline(file, buffer);
    stringstream s(buffer);
    s>>id>>id>>id>>id>>totalTime; 
    getline(file, buffer);
    getline(file, buffer);
    while (getline(file, buffer)){
        freqs.clear();
        stringstream in(buffer);
        getline(in, id, ' ');
        getline(in, bugs, ' ');
        getline(in, time, ' ');

        while(getline(in, freq, ' '))
            freqs.push_back(freq);
        array.push_back(TestSuite(id, atoi(bugs.c_str()), atoi(time.c_str()), freqs));
    }
}

vector<TestSuite> SuiteScheduler::opt(){
    //knapsack algorithm 
    int i, w; 
    int M[size+1][totalTime+1]; 
    // Build table M[][] in bottom up manner 
    for (i = 0; i <= size; i++) 
    { 
        for (w = 0; w <= totalTime; w++){ 
            if (i==0 || w==0) 
                M[i][w] = 0; 
            else if (array[i-1].get_time() <= w) {
                M[i][w] = max(array[i-1].get_bug() + M[i-1][w-array[i-1].get_time()],  M[i-1][w]);
            }
            else{
                M[i][w] = M[i-1][w];
            }
        } 
    }

    // stores the results
    res_buggy = M[size][totalTime];     
      
    w = totalTime; 
    for (i = size; i > 0 && res_buggy > 0; i--) { 
          
        if (res_buggy == M[i - 1][w])  
            continue;         
        else { 
  
            result.push_back(array[i - 1]);
            res_buggy = res_buggy - array[i - 1].get_bug(); 
            w = w - array[i - 1].get_time(); 
        } 
    }
    res_buggy = M[size][totalTime];
    return result;
}

int SuiteScheduler::get_res_bug(){
    return res_buggy;
}

#endif