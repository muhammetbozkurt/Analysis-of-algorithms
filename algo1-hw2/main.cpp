/*
Muhammet Bozkurt
150150142
*/

#include<iostream>
#include<string>
#include<stdio.h>
#include <fstream>
#include <sstream> 
#include <queue> 
#include "event.h"

using namespace std;

int main(int argc, char const *argv[])
{   
    int size;
    Event* list;

    list = file_to_memory_handler(argv[1],size);
    build_heap(list, size);
    mortal_world(list,size);

    return 0;
}