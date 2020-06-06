/*
Muhammet Bozkurt
150150142
*/
#ifndef TESTSUITE_H
#define TESTSUITE_H

#include <fstream>
#include <sstream>
#include "utils.h"

using namespace std;


class TestSuite{
private:
    string testId;
    int bugDetected;
    int time;

public:
    vector<vector<int> > ordered_sequences;
    vector<vector<int> > freqs;
    TestSuite(string id, int bugDetected, int time, vector<string> freqs);
    TestSuite();
    void print();
    int get_time();
    int get_bug();
    string get_testId();
    vector<vector<int> > get_freqs();
    void freq_converter(vector<string> old);
    void prioritize_most_significant(); //find test case that has highest coverage and put it into 
    void new_order(); // after prioritize_most_significant,  
    void ordered_sequence_cretor();
};


TestSuite::TestSuite(string id, int bugs, int time, vector<string> freqs){
    testId = id;
    bugDetected = bugs;
    this->time = time;
    freq_converter(freqs);
    
}

void TestSuite::print(){
    cout<<"----------------------------------"<<endl;
    cout<<"id: "<<testId<<"\nbugDetected: "<<bugDetected<<"\ntime: "<<time<<endl;
    for(int i=0;i<freqs.size();i++){
        cout<<i<<"th :"<<endl;
        for(int j=0;j<freqs[i].size();j++){
            cout<<freqs[i][j]<<" ";
        }
        cout<<endl;
    }
    cout<<"----------------------------------"<<endl;
}

int TestSuite::get_time(){
    return time;
}

int TestSuite::get_bug(){
    return bugDetected;
}

string TestSuite::get_testId(){
    return testId;
}

vector<vector<int> > TestSuite::get_freqs(){
    return freqs;
}

void TestSuite::prioritize_most_significant(){
    //replace test case with highest coverage at 0th index
    int max = covarage(freqs[0]), max_index = 0, temp;//covarege from utils.h
    vector<int> buffer;
    //find coverages
    for(int index = 1; index < freqs.size(); index++){
        temp = covarage(freqs[index]);
        if(temp > max){
            max_index = index;
            max = temp;
        }
    }

    if(max_index != 0){
        buffer = freqs[max_index];
        freqs[max_index] = freqs[0]; 
        freqs[0] = buffer;
    }
}

void TestSuite::freq_converter(vector<string> old){
    //because freqs are taken as string, they needs to preprocess
    int size = old.size(), unit;
    string temp;
    for(int i=0;i<size;i++){
        stringstream in(old[i]);
        vector<int> collector;
        while(getline(in,temp,'-')){
            unit = atoi(temp.c_str());
            collector.push_back(unit);
        }
        freqs.push_back(collector);
    }
}

void TestSuite::ordered_sequence_cretor(){
    /*because when ordering difference, ordered sequences are required*/
    int size_of_seq = freqs[0].size();
    vector<int> dummy;
    for(int i=0;i<freqs.size();i++){
        vector<int> sequence;
        dummy = freqs[i]; //to protect original frequences
        //initialize sequence
        for(int j=1;j<=size_of_seq;j++) 
            sequence.push_back(j);
        //sort
        insertionSort(dummy, sequence);//sort both freqs and sequences
        ordered_sequences.push_back(sequence);
    }
}

void TestSuite::new_order(){
    prioritize_most_significant();
    ordered_sequence_cretor();
    int* lev_vals;
    int last_index=1, max_val, index=2, temp;
    vector<int> temp_freq;
    lev_vals = new int[ordered_sequences.size()];
    //calculate differences rellative to first test case
    //and store it to reorder them
    for(int i=1; i<ordered_sequences.size(); i++)
        lev_vals[i] = levelstein_caller(ordered_sequences[0],ordered_sequences[i]); 
    max_val = lev_vals[1];
    while(index<ordered_sequences.size()){
        if(lev_vals[index]>max_val){
            //swaps       
            temp_freq = ordered_sequences[last_index];
            ordered_sequences[last_index] = ordered_sequences[index];
            ordered_sequences[index] = temp_freq;
            temp = lev_vals[index];
            lev_vals[index] = lev_vals[last_index];
            lev_vals[last_index] = index;
            last_index++;
        }
        index++;
    }
}

#endif