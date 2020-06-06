/*
Muhammet Bozkurt
150150142
*/
#include<iostream>
#include<string>
#include <fstream>
#include <sstream> 
#include <queue> 
#include "event.h"

using namespace std;


Event::Event(){
    this->event = "";
    this->start_or_end = "";
    this->time = 0;
}

Event::Event(string event,int time, string s_o_e){
    this->event = event;
    this->time = time;
    this->start_or_end = s_o_e;
}

Event::~Event(){
    event.clear();
}

int Event::get_time(){
    return time;
}

void Event::operator=(Event &x){
        event = x.event;
        time = x.time;
        start_or_end = x.start_or_end;
    }

string Event::get_event(){
    return event;
}

string Event::get_start_or_end(){
    return start_or_end;
}

void swap_Event(Event &first, Event &second){
    Event temp = first;
    first = second;
    second = temp;
}


void heapify(Event* list, int n, int i) 
{ 
    int smallest = i; //smallest ← i
    int l = 2 * i + 1; //l ← LEFT(i)
    int r = 2 * i + 2; //r ← RIGHT(i)
    // checking childs 
    if (l < n && list[l].get_time() < list[smallest].get_time()) 
        smallest = l; 
  
    if (r < n && list[r].get_time() < list[smallest].get_time()) 
        smallest = r; 
  
    // if smallest ≠ i 
    if (smallest != i) { 
        swap_Event(list[i], list[smallest]); 
  
        // Recursively heapify the affected sub-tree 
        heapify(list, n, smallest); 
    } 
} 
  
void build_heap(Event* arr, int n) 
{ 
    // Building heap  
    for (int i = n / 2 - 1; i >= 0; i--) 
        heapify(arr, n, i); 
    //debug(arr, n); 
 
} 

//for debug :D
void debug(Event* arr, int n) 
{ 
    for (int i = 0; i < n; i++) 
        cout<<arr[i].get_event() <<" "<<arr[i].get_time() <<endl; 
    cout << "\n"; 
} 

void kill_the_weakest(Event* arr, int& n){ 
    // Get the last element and put it into root
    Event last_element = arr[n - 1]; 
    
    arr[0] = last_element; 
    n = n - 1; 
    // heapify again 
    heapify(arr, n, 0); 
} 


Event* file_to_memory_handler(char const * file_name, int &list_size){
    //for getting size first push objects to queue then put them into array  
    queue <Event> que;
    string event_name;
    ifstream src_file(file_name);
    int size=0, start,end;
    //it is assumed there is no problem in source file
    while(src_file>>event_name){
        src_file>>start;
        src_file>>end;
        //creating nodes
        Event temp_start(event_name,start,"STARTED");
        Event temp_end(event_name,end,"ENDED");
        que.push(temp_start);
        que.push(temp_end);
        //because every line has two node
        size+=2;
    }
    src_file.close();
    list_size = size;
    Event* list = new Event[size];

    for(int i=0;!que.empty(); i++) { 
        //queue to array
        list[i] = que.front();
        que.pop(); 
    } 
    return list;
}

void mortal_world(Event* list,int size){
    //main part of assignment
    for(int time_counter=1;size>0;time_counter++){
        if(list[0].get_time() == time_counter){
            while (list[0].get_time() == time_counter){
                cout<<"TIME "<<time_counter<<": "<<list[0].get_event()<<" "<<list[0].get_start_or_end()<<endl;
                kill_the_weakest(list,size);
                if(size == 0){
                    cout<<"TIME "<<time_counter<<": NO MORE EVENTS, SCHEDULER EXITS"<<endl;
                    break;
                }
            }
        }
        else    
            cout<<"TIME "<<time_counter<<": NO EVENT"<<endl;
    }
}