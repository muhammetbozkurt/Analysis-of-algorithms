#include <iostream>
#include <fstream>
#include <sstream> 
#include <time.h>

using namespace std;


void swap(int* array,int first, int second);

class Sort{
private:
    int N;
    int* array;
    string fileName;
    void merge(int,int,int);
public:
    Sort(int, char const *);
    ~Sort();
    void bubbleSort();
    void merge_sort(int,int);
    void print();
};

int main(int argc, char const *argv[])
{
    int len;
    string a;
    clock_t t;
    stringstream converter(argv[2]),conv(argv[3]);
    converter>>len;
    conv>>a;
    Sort obj(len,argv[3]);
    if(*(argv[1]) == 'm'){
        t =clock();
        obj.merge_sort(0,len-1);
        t = clock() - t;
    }
    else if(*(argv[1]) == 'b'){
        t = clock();        
        obj.bubbleSort();
        t = clock() - t;

    }
    else{
        cerr<<"invalid sort operation type"<<endl;
        return -1;
    }
    obj.print();
    cout<<"sorting N="<<len<<" array with "<<*(argv[1])<<" take "<<t<<" clock"<<endl<<endl;
    return 0;
}

Sort::Sort(int N, char const * fileName){
    int a,counter=0;
    array = new int[N];
    this->fileName = fileName;
    this->N = N;
    ifstream unsortedFile(fileName);
    while(counter<N && unsortedFile>>a){
        array[counter] = a;
        //cout<<a<<endl;
        counter++;
    }
    unsortedFile.close();
}

Sort::~Sort(){
    delete[] array;
}

void Sort::bubbleSort(){
    int i = N;
    bool sorted = false;
    while(i>1 && !sorted){
        sorted = true;
        for(int j=1;j<=i-1;j++){
            if(array[j] < array[j-1]){
                swap(array,j,j-1);
                sorted = false;
            }
        }
    i--;
    }
}

void Sort::merge(int low, int mid, int high){
    int k=low,i=0,j=mid+1;
    int* temp;
    temp = new int[high-low+1];
    while(k<=mid && j<=high){
        if(array[k]<=array[j]){
            temp[i] = array[k];
            k++;
        }
        else{
            temp[i]=array[j];
            j++;
        }
        i++;
    }
    if(k>mid){
        for(int h=j;h<=high;h++){
            temp[i]=array[h];
            i++;
        }
    }
    else{
        for(int h=k;h<=mid;h++){
            temp[i] = array[h];
            i++;
        }
    }
    for(int h=low;h<=high;h++)
        array[h] = temp[h-low];
}

void Sort::merge_sort(int low,int high){
    if(low<high){
        int mid = int((low+high)/2);
        merge_sort(low,mid);
        merge_sort(mid+1,high);
        merge(low,mid,high);
    }
}

void Sort::print(){
    ofstream out("output.txt");
    for(int i=0;i<N;i++){
        out<<array[i]<<endl;;
    }
    out.close();
}

void swap(int* array,int first, int second){
    int temp = array[first];
    array[first] = array[second];
    array[second] = temp;
}
