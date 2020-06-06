/*
Muhammet Bozkurt
150150142
*/
#ifndef GRAPH_H
#define GRAPH_H

#include<iostream>
#include<fstream>
#include<list>
#include<iterator>


using namespace std;
class Graph{

    private:
    int size;
    

    public:
    //to make manipulations easily
    int** adj;//adj[from][to] = weight;
    int jh;
    int jd;
    int lh;
    int ld;

    Graph(char const *);
    ~Graph();
    void add_edge(int source, int node, int weight);
    void print();

    int get_size(){//size is important
        return size;
    }

};


Graph::Graph(char const * file_name){
    ifstream file(file_name);
    int source, weight, node;
    //first read number of node
    file>>size;

    //initilize neccessary arrays
    adj = new int*[size];

    for(int i=0;i<size;i++){
        adj[i] = new int[size]();
    }
    
    //read start points and destinations
    file>>jh>>jd>>lh>>ld;
    while(!file.eof()){
        file>>source>>node>>weight;
        add_edge(source,node,weight);
    }
    
    file.close();
}

Graph::~Graph(){
    //empty allocated memory
    for(int i=0;i<size;i++)
        delete[] adj[i];
    delete[] adj;
}

void Graph::add_edge(int source, int node, int weight){
    //adj[from][to] = weight;
    adj[source][node] = weight;
}


void Graph::print(){
    for(int i=0; i<size; i++){
        for(int j=0;j<size;j++){
            if(adj[i][j] != 0){
                cout<<"from: "<<i<<" to: "<<j<<" with weight: "<<adj[i][j]<<endl;
            }
        }
    }
}

#endif