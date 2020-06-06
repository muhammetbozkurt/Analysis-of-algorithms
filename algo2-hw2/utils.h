/*
Muhammet Bozkurt
150150142
*/
#ifndef UTILS_H
#define UTILS_H

#include<iostream>
#include<list>
#include"graph.h"
#include"Path_Finder.h"

/*
I am not proud of codes in this library :(
*/

using namespace std;

list<int> cmp_paths(Path_Finder& josepf, Path_Finder& lucy);
void calc_alter_paths(Path_Finder& josepf, Path_Finder& lucy, list<int> intersection, int** graph, int f);
void procedure(const char* file_name);
void print_path(int** path, int size);


class Output_Handler{//  to use print out results to terminal properly

public:
    int size_lucy_to_dest;
    int size_joseph_to_dest;
    int size_lucy_to_home;
    int size_joseph_to_home;
    int** lucy_to_dest;
    int** joseph_to_dest;
    int** lucy_to_home;
    int** joseph_to_home;
    bool is_there_solution;
    void print_out();
};

void Output_Handler::print_out(){
    if(!is_there_solution){
        cout<<"No solution!"<<endl;
        return;
    }
    cout<<"Joseph's Path, duration: "<<joseph_to_home[1][0]<<endl;
    print_path(joseph_to_dest, size_joseph_to_dest);
    cout<<"-- return --"<<endl;
    print_path(joseph_to_home, size_joseph_to_home);
    cout<<endl;
    cout<<"Lucy's Path, duration: "<<lucy_to_home[1][0]<<endl;
    print_path(lucy_to_dest, size_lucy_to_dest);
    cout<<"-- return --"<<endl;
    print_path(lucy_to_home, size_lucy_to_home);
}


list<int> cmp_paths(Path_Finder& josepf, Path_Finder& lucy);
void calc_alter_paths(Path_Finder& josepf, Path_Finder& lucy, list<int> intersection, int** graph, int f);
void procedure(const char* file_name);

void print_path(int** path, int size){
    for(int i = size-1; i >= 0 ; i--){
        cout<<"Node: "<<path[0][i]<<" Time: "<< path[1][i] <<endl;
    }
}

list<int>  cmp_paths(Path_Finder& joseph, Path_Finder& lucy){
    list<int> output;//order in list int_node, lucy_int_node_pred, joseph_int_node_pred   
    int** j_path = joseph.get_path();
    int** l_path = lucy.get_path();
    int j_path_size = joseph.get_path_size(), l_path_size = lucy.get_path_size();
    for(int j=j_path_size-1; j >= 0; j--){
        for(int l=l_path_size-1; l >= 0; l--){
            if(j_path[0][j] == l_path[0][l] && j_path[1][j] == l_path[1][l]){
                output.push_back(l_path[0][l]);
                output.push_back(lucy.get_pred(l_path[0][l]));
                output.push_back(joseph.get_pred(j_path[0][j]));
                return output;
            }
        }
    }
    return output;
}

void calc_alter_paths(Path_Finder& josepf, Path_Finder& lucy, list<int> intersection, int** graph, int remind_l = 0,int remind_j = 0){
    //finding alternative path for lucy

    list<int>::iterator i = intersection.begin();

    int node = intersection.front(), memory; //node holds  intersection node no
    intersection.pop_front();
    int node_pred = intersection.front(); //node_pred holds pred node for lucy
    intersection.pop_front();

    memory = graph[node_pred][node];

    graph[node_pred][node] = 0;

    lucy.dijkstra(graph);
    lucy.path_extractor(remind_l); //now lucy's path is updated

    //finding alternative path for josef
    graph[node_pred][node] = memory;

    node_pred = intersection.front();//node_pred holds pred node for josef
    intersection.pop_front();
    memory = graph[node_pred][node];
    graph[node_pred][node] = 0;
    josepf.dijkstra(graph);
    josepf.path_extractor(remind_j);//now josepf's path is updated
    graph[node_pred][node] = memory;
}


void procedure(const char* file_name){
    int** lucy_first, **joseph_first, **lucy_second, **joseph_second; // to hold their first paths  
    int lucy_first_size, joseph_first_size;
    list<int> intersection;
    Graph graph(file_name);
    Path_Finder lucy(graph.lh,graph.ld,graph.get_size());
    Path_Finder joseph(graph.jh,graph.jd,graph.get_size());
    Output_Handler terminal;
    terminal.is_there_solution = true;
    
    lucy.dijkstra(graph.adj);
    joseph.dijkstra(graph.adj);

    lucy.path_extractor();
    joseph.path_extractor();

    lucy_first = lucy.get_path();
    joseph_first = joseph.get_path();
    lucy_first_size = lucy.get_path_size();
    joseph_first_size = joseph.get_path_size();
    intersection = cmp_paths(joseph, lucy);

    terminal.size_joseph_to_dest = joseph_first_size;
    terminal.size_lucy_to_dest = lucy_first_size;
    terminal.lucy_to_dest = lucy_first;
    terminal.joseph_to_dest = joseph_first;

    if(intersection.empty());
    else{
        int lucy_time = -1, joseph_time = -1, lucy_second_size = 0, joseph_second_size = 0;
        calc_alter_paths(joseph,lucy,intersection,graph.adj);
        joseph_second = joseph.get_path();
        lucy_second = lucy.get_path();
        lucy_second_size = lucy.get_path_size();
        joseph_second_size = joseph.get_path_size();
        //first check lucy's alternative
        joseph.set_path(joseph_first);
        intersection = cmp_paths(joseph, lucy);//we only care about will intersect be empty?
        if(intersection.empty())
            lucy_time = lucy_second[1][0];

        //check joseph's alternative
        joseph.set_path(joseph_second);
        lucy.set_path(lucy_first);
        intersection = cmp_paths(joseph, lucy);//we only care about will intersect be empty?
        if(intersection.empty())
            joseph_time = joseph_second[1][0];
        //time ı -1 olan cakısıyordur        


        if(!joseph.is_found() and !lucy.is_found()){
            //there is no solution
            terminal.is_there_solution = false;
        }
        else if(!joseph.is_found()){
            //take directly lucy_secand path
            terminal.lucy_to_dest = lucy_second;
            terminal.size_lucy_to_dest = lucy_second_size;
        }
        else if(!lucy.is_found()){
            // take directly joseph
            terminal.joseph_to_dest = joseph_second;
            terminal.size_joseph_to_dest = joseph_second_size;
        }
        else if(joseph.is_found() and lucy.is_found()){//for debug
            //choose better one
            if(joseph_time > lucy_time){
                //choose lucy
                terminal.lucy_to_dest = lucy_second;
                terminal.size_lucy_to_dest = lucy_second_size;
            }
            else{
                terminal.joseph_to_dest = joseph_second;
                terminal.size_joseph_to_dest = joseph_second_size;
            }
        }
        else{//unecassary else just for debugging
            cout<<"I should have not be selected there is absulotaly a problem!"<<endl;
        }
    }

    intersection.clear();
    int remind_l = lucy.get_path()[1][0]+30;
    int remind_j = joseph.get_path()[1][0] + 30;
    lucy.set_target(graph.ld, graph.lh);
    joseph.set_target(graph.jd, graph.jh);
    lucy.dijkstra(graph.adj);
    joseph.dijkstra(graph.adj);
    
    lucy.path_extractor(remind_l);
    joseph.path_extractor(remind_j);


    lucy_first = lucy.get_path();
    joseph_first = joseph.get_path();
    lucy_first_size = lucy.get_path_size();
    joseph_first_size = joseph.get_path_size();

    intersection = cmp_paths(joseph, lucy);

    terminal.joseph_to_home = joseph_first;
    terminal.size_joseph_to_home = joseph_first_size;
    terminal.lucy_to_home = lucy_first;
    terminal.size_lucy_to_home = lucy_first_size;

    if(intersection.empty());
    else{
        int lucy_time = -1, joseph_time = -1, lucy_second_size, joseph_second_size;
        calc_alter_paths(joseph,lucy,intersection,graph.adj, remind_l, remind_j);
        joseph_second = joseph.get_path();
        lucy_second = lucy.get_path();
        lucy_second_size = lucy.get_path_size();
        joseph_second_size = joseph.get_path_size();
        //first check lucy's alternative
        joseph.set_path(joseph_first);
        intersection = cmp_paths(joseph, lucy);//we only care about will intersect be empty?
        if(intersection.empty())
            lucy_time = lucy_second[1][0];

        //check joseph's alternative
        joseph.set_path(joseph_second);
        lucy.set_path(lucy_first);
        intersection = cmp_paths(joseph, lucy);//we only care about will intersect be empty?
        if(intersection.empty())
            joseph_time = joseph_second[1][0];
        //time ı -1 olan cakısıyordur        



        if(!joseph.is_found() and !lucy.is_found()){
            //there is no solution
            terminal.is_there_solution = false;
        }
        else if(!joseph.is_found()){
            //take directly lucy_secand path
            terminal.lucy_to_home = lucy_second;
            terminal.size_lucy_to_home = lucy_second_size;
        }
        else if(!lucy.is_found()){
            // take directly joseph
            terminal.joseph_to_home = joseph_second;
            terminal.size_joseph_to_home = joseph_second_size;
        }
        else if(joseph.is_found() and lucy.is_found()){//for debug
            //choose better one
            if(joseph_time > lucy_time){
                //choose lucy
                terminal.lucy_to_home = lucy_second;
                terminal.size_lucy_to_home = lucy_second_size;
            }
            else{
                terminal.joseph_to_home = joseph_second;
                terminal.size_joseph_to_home = joseph_second_size;
            }
        }
        else{//unecassary else just for debugging
            cout<<"I should have not be selected there is absulotaly a problem!"<<endl;
        }
    }
    terminal.print_out();
}

#endif