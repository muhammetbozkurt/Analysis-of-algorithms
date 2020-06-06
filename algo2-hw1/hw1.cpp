#include <iostream> 
#include <list>
#include <stack>

#include <fstream>
#include <string.h>
#include <stdio.h>

#include <iterator>

#include <bits/stdc++.h> 

using namespace std;

class parent_child{
    public:
    int parent;
    int child;
    parent_child();
    parent_child(int parent, int child){
        this->parent = parent;
        this->child = child;
    };
};

class Attack{
    public:
    string name;
    int pp_value;
    int acc_value;
    int damege_value;
    int first_usage_value;
    Attack(string name, int pp_value, int acc_value, int damege_value, int first_usage_value);
};

class Pokemon{
    public:
    int number_of_attacks;
    list<Attack> attacks;
    Pokemon();
    Pokemon(char const* file_name);
    ~Pokemon();
    
};

class Node{
    public:
    string att_name;
    string is_eff;
    int parent;
    int no;
    //0 for first pokemon -> pikachu; 1 for second one 
    int hp[2];
    int pp[2];
    int isleaf;

    double prob;

    char turn;

    Node();
    Node(int parent, int no, int first_pokemon_hp, int first_pokemon_pp, int second_pokemon_hp, int second_pokemon_pp, int isleaf, double prob, char turn);
    //what a shame
    void rearrange(int parent, int no, int first_pokemon_hp, int first_pokemon_pp, int second_pokemon_hp, int second_pokemon_pp, int isleaf, double prob, char turn, string att, int effec);
    void print();
};

class Graph{ 

    int size;    // approx. number of vertices 
    int level;
    list<int> *adjency;    // Pointer to an array containing adjacency lists 
    list<Node> last_level,backup; //backup will be used if last_level is empty

    Node* all_nodes;

    Pokemon* pokemons;

    public: 
    int node_counter; // to keep track of actual number of nodes
    void  level_creation_utils(list<Attack>::iterator it, Node& current_node, int level_num, int attack_num, int reverse=1);
    Graph(char const* first_pokemon_file_name, char const* second_pokemon_file_name, int level);   // Constructor
    ~Graph();
    void add_edge(int v, int w);   // function to add an edge to graph 
    void creation();
    void level_creation(Node& current_node, int level_num);
    void print_last_level();
    void BFS();
    void DFS();
    void path_finder(int who);
    int death_finder(int who); 
};

int main(int argc, char** argv){
    
    if(argc < 2)
        cout<<"Unexpected Command Line Input!"<<endl;
    else if(strcmp("part1", argv[1])==0){
        Graph test("pikachu.txt","blastoise.txt",atoi(argv[2]));
        test.print_last_level();
    }
    else if(strcmp("part2", argv[1])==0){
        Graph test("pikachu.txt","blastoise.txt",atoi(argv[2]));
        if(strcmp("dfs", argv[3])==0)
            test.DFS();
        else if(strcmp("bfs", argv[3])==0)
            test.BFS();
        else
            cout<<"Unexpected Command Line Input!"<<endl;
    }
    else if(strcmp("part3", argv[1])==0){
        Graph test("pikachu.txt","blastoise.txt",12);
        if(strcmp("pikachu", argv[2])==0)
            test.path_finder(1);
        else if(strcmp("blastoise", argv[2])==0)
            test.path_finder(0);
        else
            cout<<"Unexpected Command Line Input!"<<endl;
    }
    else
        cout<<"Unexpected Command Line Input!"<<endl;
    return 0;
}
  
Graph::Graph(char const * first_pokemon_file_name, char const* second_pokemon_file_name, int level){
    
    pokemons = new Pokemon[2]{first_pokemon_file_name,second_pokemon_file_name};
    this->level = level;
    //calculate size approximetly
    size = 1;
    for(int i=0;i<=level ;i++){
        if(i%2==0)
            size *= pokemons[1].number_of_attacks;
        else
            size *= pokemons[0].number_of_attacks;
    }
    size++;
    if(level==12)
        size = 15867969;

    adjency = new list<int>[size];
    all_nodes = new Node[size];
    node_counter = 0;
    creation();
    delete[] pokemons;
}

void Graph::creation(){
    int level_counter = 0;
    Node root(-1,0,273,100,361,100,0,1.0,'P');
    all_nodes[node_counter++] = root;
    last_level.push_back(root);

    list<Node> temp;
    while(level_counter<level){
        if(!last_level.empty()) backup = last_level;
        temp = last_level;
        last_level.clear();
        while(!temp.empty()){
            level_creation(temp.front(),level_counter);
            temp.pop_front();
        }
        level_counter++;
    }
}

void Graph::level_creation_utils(list<Attack>::iterator it, Node& current_node, int level_num, int attack_num, int not_reverse){
    int new_hp, new_pp;
    double prob = (not_reverse==1) ? (it->acc_value / 100.0) * (current_node.prob/attack_num) : (1-it->acc_value / 100.0) * (current_node.prob/attack_num);
    int who = (level_num)%2;//if zero that means first pokemon will play
    Node temp;    
    if(who == 0){
        //first pokemon is attacking therefore its pp will decrease
        new_hp = current_node.hp[1] - it->damege_value * not_reverse;
        new_pp = current_node.pp[0] + it->pp_value;
    }
    else{
        //second pokemon is attacking
        new_hp = current_node.hp[0] - it->damege_value * not_reverse;
        new_pp = current_node.pp[1] + it->pp_value;
    }
    
    int isleaf = (level-1 == level_num || new_hp == 0)? 1 : 0;
    if(who==0)
        temp.rearrange(current_node.no, node_counter, current_node.hp[0],new_pp,new_hp,current_node.pp[1],isleaf,prob,'B',it->name,not_reverse);
    else
        temp.rearrange(current_node.no, node_counter, new_hp, current_node.pp[0], current_node.hp[1], new_pp, isleaf, prob, 'P',it->name,not_reverse);
    
    add_edge(current_node.no, node_counter);
    last_level.push_back(temp);
    all_nodes[node_counter] = temp;
    node_counter++;
}

void Graph::level_creation(Node& current_node, int level_num){
    int attack_num = 0;    
    int who = (level_num)%2;
    //to calculate prob, first how many attacks is needed to be learnt
    for(list<Attack>::iterator it = pokemons[who].attacks.begin();it != pokemons[who].attacks.end(); it++)
        if(it->first_usage_value<=level_num)
            attack_num++;
    for(list<Attack>::iterator it = pokemons[who].attacks.begin();it != pokemons[who].attacks.end(); it++){
        if(it->first_usage_value <= level_num && it->acc_value>0 && current_node.pp[0] > it->pp_value && current_node.isleaf == 0){
            level_creation_utils(it,current_node,level_num,attack_num);
            if(it->acc_value<100)
                level_creation_utils(it,current_node,level_num,attack_num,0);
        }
    }
}
  
void Graph::add_edge(int v, int w){ 
    adjency[v].push_back(w); // Add w to v’s list. 
}

void Graph::BFS(){
    time_t start, end;
    start = clock();
    int index_memory;
    bool *discovered = new bool[node_counter+1]; 
    for(int i = 0; i < node_counter+1; i++) 
        discovered[i] = false; 
  
    list<int> queue;
    queue.push_back(0);
    discovered[0] = true;
    //cout<<"BFS: "<<endl;
    while(!queue.empty()){

        index_memory = queue.front();
        //cout<<index_memory<<" ";
        queue.pop_front();
        
        for (list<int>::iterator i = adjency[index_memory].begin(); i != adjency[index_memory].end(); ++i){ 
            if (!discovered[*i]){
                discovered[*i] = true;
                queue.push_back(*i);
            } 
        } 
    }
    end = clock();
    double time_taken = double(end - start) / double(CLOCKS_PER_SEC); 
    cout << "Time : " << fixed << time_taken << setprecision(5); 
    cout << " sec " << endl; 
    cout<<"Total number of node: "<<node_counter<<endl;
} 

void Graph::DFS(){ 
    time_t start, end;
    start = clock();
    bool *discovered = new bool[node_counter]; 
    for (int i = 0; i < node_counter; i++) 
        discovered[i] = false; 
    //cout<<"DFS:"<<endl;
    stack<parent_child> keeper;//to keep p(u),u
    parent_child temp();
    int index;
    keeper.push(parent_child(-1,0));
    while(!keeper.empty()){
        index = keeper.top().child;
        keeper.pop();
        if(!discovered[index]){
            discovered[index] = true;
            for(list<int>::iterator i = adjency[index].begin();i != adjency[index].end();i++)
                keeper.push(parent_child(index,*i));
        }
    }
    end = clock();
    double time_taken = double(end - start) / double(CLOCKS_PER_SEC); 
    cout << "Time : " << fixed << time_taken << setprecision(5); 
    cout << " sec " << endl; 
    cout<<"Total number of node: "<<node_counter<<endl;
}

void Graph::print_last_level(){
    if(!last_level.empty()){
        for(list<Node>::iterator i = last_level.begin();i != last_level.end();++i){
            i->print();
        }
    }
    else{
        for(list<Node>::iterator i = last_level.begin();i != last_level.end();++i){
            i->print();
        }
    }
}

int Graph::death_finder(int who){//pokemon i want to find dead
    int ret_val = 0;//index value that will return if this method can't find a death
    for(int i = 0; i< node_counter; i++){
        if(all_nodes[i].hp[who] <= 0)
            return i;
        else if(all_nodes[i].isleaf == 1 && all_nodes[ret_val].hp[who]>all_nodes[i].hp[who])
            ret_val = i;
    }
    return ret_val;
}

void Graph::path_finder(int who){//who refers to pokemon we want to kill
    //we don't need use bfs again because while creating graph which is actually tree numbers were given in apropraite order for bfs
    int index = death_finder(who);
    int buf = index;
    list<int> order;
    while(all_nodes[index].parent != -1){//this while loop will collect every node in path except root
        order.push_front(index);
        index = all_nodes[index].parent;
    }
    for(list<int>::iterator i = order.begin();i!=order.end();i++){
        if(all_nodes[*i].turn == 'P')
            cout<<"Blastoise used ";
        else
            cout<<"Pikachu used ";
        cout<<all_nodes[*i].att_name<<". It's "<<all_nodes[*i].is_eff<<endl;
    }
    cout<<"Level count: "<<order.size()<<endl;
    cout<<"Probability: "<<all_nodes[buf].prob<<endl;
}

Graph::~Graph(){
    last_level.clear();
    backup.clear();
    delete[] all_nodes;
    for(int i=0;i<size;i++)
        adjency[i].clear();
    delete[] adjency;
}

Node::Node(){
    this->hp[0] = -1;
    this->pp[0] = -1;

    this->hp[1] = -1;
    this->pp[1] = -1;
    this->isleaf = -1;

    this->prob = -1.0;

    this->turn = '\0';

    this->no = 0;
    parent = -1;
}

Node::Node(int parent, int no, int first_pokemon_hp, int first_pokemon_pp, int second_pokemon_hp, int second_pokemon_pp, int isleaf, double prob, char turn){
    this->hp[0] = first_pokemon_hp;
    this->pp[0] = first_pokemon_pp;

    this->hp[1] = second_pokemon_hp;
    this->pp[1] = second_pokemon_pp;
    this->isleaf = isleaf;

    this->prob = prob;

    this->turn = turn;

    this->no = no;
    this->parent = parent;
}

void Node::rearrange(int parent, int no, int first_pokemon_hp, int first_pokemon_pp, int second_pokemon_hp, int second_pokemon_pp, int isleaf, double prob, char turn, string att, int effec){
    this->hp[0] = first_pokemon_hp;
    this->pp[0] = first_pokemon_pp;

    this->hp[1] = second_pokemon_hp;
    this->pp[1] = second_pokemon_pp;
    this->isleaf = isleaf;

    this->prob = prob;

    this->turn = turn;

    this->no = no;
    this->parent = parent;

    att_name = att;
    is_eff = (effec)? "effective" : "noneffective";
}

Pokemon::Pokemon(){
    number_of_attacks = 0;
}

Pokemon::Pokemon(char const* file_name){
    fstream pokemon_file(file_name);
    string line;
    string name;
    int pp_value, acc_value, damege_value, first_usage_value, collector=0;
    number_of_attacks = 0;

    getline(pokemon_file, line);//to get rid of first line

    while(!pokemon_file.eof()){
        getline(pokemon_file, line);
        name = line.substr(0,line.find(","));
        line = line.substr(name.size()+1, line.size());
        sscanf(line.c_str(),"%d,%d,%d,%d ",&pp_value,&acc_value,&damege_value,&first_usage_value);
        attacks.push_back(Attack(name,pp_value,acc_value,damege_value,first_usage_value));
        number_of_attacks++;
    }
}

Pokemon::~Pokemon(){
    attacks.clear();
}

Attack::Attack(string name,int pp_value, int acc_value, int damege_value, int first_usage_value){
    this->name = name;
    this->pp_value = pp_value;
    this->acc_value = acc_value;
    this->damege_value = damege_value;
    this->first_usage_value = first_usage_value;
}

void Node::print(){
    cout<<"P_HP:"<<hp[0]<<" P_PP:"<<pp[0]<<" B_HP:"<<hp[1]<<" B_PP:"<<pp[1]<<" PROB:"<<prob<<endl;
}