/*
Muhammet Bozkurt
150150142
*/
#ifndef PATH_FINDER_H
#define PATH_FINDER_H

#include<iostream>
#define INT16_MAX 32767

using namespace std;

class Path_Finder{
    private:
    
    int size;
    int** path;//[0]->path no; [1]->time; to compare
    int* pred;
    bool* explored;
    int* dist;
    int start;
    int end;
    int path_size;

    public:
    Path_Finder(int start, int end, int size);
    ~Path_Finder();
    int find_min();
    void dijkstra(int** graph);
    int path_extractor(int);//returns size of path
    void debug();
    bool is_found();//return true is path is found otherwise false
    void print_path();
    int** get_path();
    int get_path_size();
    int get_pred(int);
    void set_path(int**);
    void set_target(int s, int e){
        start = s;
        end = e;
    }
};

Path_Finder::Path_Finder(int start, int end, int size){
    this->path_size;
    this->size = size;
    this->end = end;
    this->start = start;
    path = new int*[2];
    for(int i=0; i<2 ;i++)
        path[i] = new int[size];
    pred = new int[size];
    explored = new bool[size];
    dist = new int[size];
    //initializations
    for (int i = 0; i < size; i++) 
        dist[i] = INT16_MAX, explored[i] = false, pred[i] = -1; 

    dist[start] = 0;
}

Path_Finder::~Path_Finder(){
    for(int i=0; i<2;i++)
        delete[] path[i];
    delete[] path;
    delete[] pred;
    delete[] explored;
    delete[] dist;
}


int Path_Finder::find_min(){ 
    // Initialize min value 
    int min = INT16_MAX, min_index; 
  
    for (int i = 0; i < size; i++) 
        if (!explored[i] && dist[i] <= min) 
            min = dist[i], min_index = i; 
  
    return min_index; 
} 

void Path_Finder::dijkstra(int** graph){//adj[from][to] = weight;
     
    //initializations
    for (int i = 0; i < size; i++) 
        dist[i] = INT16_MAX, explored[i] = false, pred[i] = -1; 

    dist[start] = 0;
    for (int count = 0; count < size ; count++) { 

        int min_dist_index = find_min(); 
        explored[min_dist_index] = true; 
  
        for (int v = 0; v < size; v++){
            if(min_dist_index==6 and v==2){
                //cout<<"djistra: graph[min_dist_index][v]: "<<graph[min_dist_index][v]<<" dist[min_dist_index]: "<<
                //djistra: graph[min_dist_index][v]: 0 dist[min_dist_index]: 6 !explored[v]: 1
                //dist[min_dist_index]<<" !explored[v]: "<<!explored[v]<<endl;
            }
            if (!explored[v] && graph[min_dist_index][v] && dist[min_dist_index] != INT16_MAX 
                && dist[min_dist_index] + graph[min_dist_index][v] < dist[v]) {
                dist[v] = dist[min_dist_index] + graph[min_dist_index][v]; 
                pred[v] = min_dist_index;
            }
        }
    }
}

void Path_Finder::debug(){
    int j;
    cout<<"size: "<<size<<endl;
    for(int i=0;i<size;i++){
        cout<<"\tnode: "<<i<<endl;
        cout<<"\t\tpredi: "<<pred[i]<<endl;
        cout<<"\t\texp: "<<explored[i]<<endl;
        cout<<"\t\tdist: "<<dist[i]<<endl;
    }
}

bool Path_Finder::is_found(){
    if(dist[end]>=INT16_MAX)
        return false;
    return true;
}

int Path_Finder::path_extractor(int return_path = 0){
    if(!is_found())
        return 0;
    int path_size = 0;
    int temp_index = end;
    int time_;
    
    while(temp_index != start){
        path[0][path_size] = temp_index;
        path[1][path_size++] = dist[temp_index] + return_path ;
        temp_index = pred[temp_index];
    }
    path[0][path_size] = start;
    path[1][path_size++] = 0 + return_path;
    this->path_size = path_size;
    return path_size;
}

void Path_Finder::print_path(){

    cout<<"path_size: "<<path_size<<endl;
    for (int i = path_size-1; i >= 0 ; i--){
        cout<<"node: "<<path[0][i]<<" time: "<< path[1][i] <<endl;
    }
}

int** Path_Finder::get_path(){
    int** new_path;
    new_path = new int*[2];
    for(int i=0;i<2;i++)
        new_path[i] = new int[size];
    for(int i=0; i<size; i++){
        new_path[0][i] = path[0][i];
        new_path[1][i] = path[1][i];
    }
    return new_path;
}

int Path_Finder::get_path_size(){
    return path_size;
}

int Path_Finder::get_pred(int index){
    return pred[index];
}

void Path_Finder::set_path(int** path_){
    int** new_path;
    new_path = new int*[2];
    for(int i=0;i<2;i++)
        new_path[i] = new int[size];
    for(int i=0; i<size; i++){
        new_path[0][i] = path_[0][i];
        new_path[1][i] = path_[1][i];
    }
    path = new_path;
}

#endif