/*
Muhammet Bozkurt
150150142
*/
#ifndef EVENT_H
#define EVENT_H

using namespace std;

class Event{

private:
    int time;
    string event;
    string start_or_end;

public:
    Event();
    Event(string, int time,string);
    ~Event();
    int get_time();
    string get_event();
    string get_start_or_end();
    void operator=(Event &x);
};

void swap_Event(Event &first, Event &second);
void heapify(Event arr[], int n, int i);
void build_heap(Event arr[], int n);
void kill_the_weakest(Event arr[], int& n);
void mortal_world(Event* list,int size);
void debug(Event arr[], int n);
Event* file_to_memory_handler(char const * file_name, int &list_size);



#endif