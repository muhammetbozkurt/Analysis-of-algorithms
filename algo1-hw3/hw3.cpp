#include<iostream>
#include <queue>
#include "hw3.h"

using namespace std;

class Package{

    private:
        int ordinal;
        int size;
        char color;

    public:
        int th;
        Package *left, *right, *parent;
        Package(int size, int ordinal, char color);
        Package* uncle();
        bool is_on_left();
        Package* sibling();
        void position_changing(Package* new_parent);
        bool is_there_red_child();
        int get_size();
        int get_ordinal();
        char get_color();
        void set_color(char x);
        void set_size(int size);
        void set_ordinal(int);
};

class Tree{
    private:
        Package* root;
    public:
        int blackNode;
        int redNode;
        int totalPackage;
        Tree();
        void left_rotate(Package* x);
        void right_rotate(Package* x);
        Package* search_size(int size);
        void rearranging_packages_colors(Package* x);
        void swap_colors(Package *x1, Package *x2);
        void swap_values(Package*,Package *);
        void delete_node(Package *v);
        Package *successor(Package *x);
        Package *BSTreplace(Package *x);
        void fix_double_black(Package *x);
        void insert(int size, int ordinal);
        void delete_by_size(int size);
        void debug();
        void inorder(Package* x);
        Package* find_smallest();
        void arrange_th(Package* currnet_package);
        Package* finding_nth_smallest(Package* current, int order);
        void count(Package*);
        Package* get_root(){
            return root;
        }
};


Package::Package(int size, int ordinal, char color='r'){
    this->ordinal = ordinal;
    this->size = size;
    this->color = color;
    parent = NULL; 
    left = NULL;
    right = NULL;
    th = 0;
}

bool Package::is_on_left(){
    return this == parent->left;
}

Package* Package::uncle(){
    if(parent == NULL || parent->parent == NULL)
        return NULL;
    
    if(parent->is_on_left())
        return parent->parent->right;
    return parent->parent->left;
}

Package* Package::sibling(){
    if(parent == NULL)
        return NULL;
    if(is_on_left())
        return parent->right;
    return parent->left;
}

void Package::position_changing(Package* new_parent){
    if(parent != NULL){
        if(is_on_left())
            parent->left = new_parent;
        else
            parent->right = new_parent;
    }
    new_parent->parent = parent;
    parent = new_parent;
}

bool Package::is_there_red_child(){
    return (left != NULL && left->color == 'r') || (right != NULL && right->color == 'r'); 
}

int Package::get_ordinal(){
    return ordinal;
}

int Package::get_size(){
    return size;
}

char Package::get_color(){
    return color;
}

void Package::set_color(char color){
    this->color = color;
}

void Package::set_size(int size){
    this->size = size;
}

void Package::set_ordinal(int ordinal){
    this->ordinal = ordinal;
}

Tree::Tree(){
    root = NULL;
    redNode = 0;
    blackNode=0;
    totalPackage=0;
}

void Tree::left_rotate(Package* x){
	Package *new_parent = x->right; 

	if (x == root) 
	root = new_parent; 

	x->position_changing(new_parent); 

	x->right = new_parent->left; 
	if (new_parent->left != NULL) 
	new_parent->left->parent = x; 

	new_parent->left = x; 
}

void Tree::right_rotate(Package* x){
	Package *new_parent = x->left; 

	if (x == root) 
	root = new_parent; 

	x->position_changing(new_parent); 

	x->left = new_parent->right; 
	if (new_parent->right != NULL) 
	new_parent->right->parent = x; 

	new_parent->right = x; 
}

Package* Tree::search_size(int size){
    Package* temp = root;
    Package*res;
    while(temp != NULL){
        if(size == temp->get_size())
            res = temp;

        if(size < temp->get_size()){
            if(temp->left == NULL)
                break;
            else{
                temp = temp->left;
                res = temp;
            }      
        }

        else{
            if(temp->right == NULL)
                break;
            else{
                temp = temp->right;
                res = temp;
            }
        }
    }
    return res;
}

void Tree::rearranging_packages_colors(Package *x) { 
	// if x is root color it black and return 
	if (x == root) { 
	x->set_color('b') ; 
	return; 
	} 

	// initialize parent, grandparent, uncle 
	Package *parent = x->parent, *grandparent = parent->parent, 
		*uncle = x->uncle(); 

	if (parent->get_color() != 'b') { 
        if (uncle != NULL && uncle->get_color() == 'r') { 
            // uncle red, perform recoloring and recurse 
            parent->set_color('b'); 
            uncle->set_color('b'); 
            grandparent->set_color('r'); 
            rearranging_packages_colors(grandparent); 
        } 
        else { 
            if (parent->is_on_left()) { 
                if (x->is_on_left()) { 
                    // for left right 
                    swap_colors(parent, grandparent); 
            } 
            else { 
                left_rotate(parent); 
                swap_colors(x, grandparent); 
            } 
            // for left left and left right 
            right_rotate(grandparent); 
            } 
            else { 
                if (x->is_on_left()) { 
                    // for right left 
                    right_rotate(parent); 
                    swap_colors(x, grandparent); 
            } 
            else { 
                swap_colors(parent, grandparent); 
            } 

            left_rotate(grandparent); 
            } 
        } 
	} 
} 

void Tree::swap_colors(Package *x1, Package *x2) { 
	char temp; 
	temp = x1->get_color();
    x1->set_color(x2->get_color()); 
	x2->set_color(temp);
} 

void Tree::swap_values(Package *u, Package *v) { 
	int temp; 
	temp = u->get_size();
    u->set_size(v->get_size()); 
	v->set_size(temp); 
    
    temp = u->get_ordinal();
    u->set_ordinal(v->get_ordinal()); 
	v->set_ordinal(temp); 
}

Package* Tree::successor(Package *x){ 
	Package *temp = x; 
	while (temp->left != NULL) 
    	temp = temp->left; 
	return temp; 
} 

Package* Tree::BSTreplace(Package *x){ 
	if (x->left != NULL and x->right != NULL) 
	    return successor(x->right); 

	if (x->left == NULL and x->right == NULL) 
	    return NULL; 

	if (x->left != NULL) 
	    return x->left; 
	else
	    return x->right; 
} 

void Tree::delete_node(Package *v){ 
	Package *u = BSTreplace(v); 

	bool both_black = ((u == NULL or u->get_color() == 'b') and (v->get_color() == 'b')); 
	Package *parent = v->parent; 

	if (u == NULL) { 
        if (v == root) { 
            root = NULL; 
        } 
        else { 
            if (both_black) { 
            fix_double_black(v); 
            } 
            else { 
                if (v->sibling() != NULL) 
                    v->sibling()->set_color('r'); 
            } 

            if (v->is_on_left()) { 
                parent->left = NULL; 
            } 
            else { 
                parent->right = NULL; 
            } 
        } 
        delete v; 
	} 

	else if (v->left == NULL or v->right == NULL) { 
        if (v == root) { 
            swap_values(u,v);
            v->left = v->right = NULL; 
            delete u; 
        } 
        else { 
            if (v->is_on_left()) { 
                parent->left = u; 
            } 
            else { 
                parent->right = u; 
            } 
            delete v; 
            u->parent = parent; 
            if (both_black) { 
                fix_double_black(u); 
            } 
            else { 
                u->set_color('b'); 
            } 
        } 
	} 

    else{
        swap_values(u, v); 
        delete_node(u);
    }
     
} 

void Tree::fix_double_black(Package *x){ 
	if (x == root) 
	return; 

	Package *sibling = x->sibling(), *parent = x->parent; 
	if (sibling == NULL) { 
        fix_double_black(parent); 
	} 
    else { 
        if (sibling->get_color() == 'r') { 
                parent->set_color('r');
                sibling->set_color('b'); 
                if (sibling->is_on_left()) { 
                    right_rotate(parent); 
                } 
                else { 
                    left_rotate(parent); 
                } 
                fix_double_black(x); 
        } 
        else { 
            if (sibling->is_there_red_child()) { 
                if (sibling->left != NULL and sibling->left->get_color() == 'r') { 
                    if (sibling->is_on_left()) { 
                        sibling->left->set_color(sibling->get_color()); 
                        sibling->set_color(parent->get_color());
                        right_rotate(parent); 
                    } 
                    else { 
                        sibling->left->set_color(parent->get_color());
                        right_rotate(sibling); 
                        left_rotate(parent); 
                    } 
                } else { 
                    if (sibling->is_on_left()) { 
                        sibling->right->set_color(parent->get_color()); 
                        left_rotate(sibling); 
                        right_rotate(parent); 
                    } 
                    else { 
                        sibling->right->set_color(sibling->get_color());
                        sibling->set_color(parent->get_color());
                        left_rotate(parent); 
                    } 
                } 
                parent->set_color('b'); 
            } 
            else { 
                sibling->set_color('r');
                if (parent->get_color() == 'b') 
                    fix_double_black(parent); 
                else
                    parent->set_color('b');
            } 
        } 
	} 
} 

void Tree::insert(int size, int ordinal){
    Package *new_package = new Package(size,ordinal); 
	if (root == NULL) { 
        new_package->set_color('b'); 
        root = new_package; 
	} 
    else { 
        Package *temp = search_size(size); 
        new_package->parent = temp; 

        if (size < temp->get_size()) 
            temp->left = new_package; 
        else
            temp->right = new_package; 
        rearranging_packages_colors(new_package); 
	} 
}

void Tree::delete_by_size(int size){
    if (root == NULL) 
	    return; 
	Package *v = search_size(size);//, *u 
	if (v->get_size() != size) { 
    	return; 
	} 
	delete_node(v); 
}

void Tree::debug(){

    cout<<"printing:"<<endl;
    
    if(root == NULL)
        cout<<"Empty"<<endl;
    
    else
        inorder(root);
    cout<<endl;
}

void Tree::inorder(Package* x){
    if(x == NULL)
        return;
    inorder(x->left); 
	cout << x->get_size() << " ord: "<<x->get_ordinal()<<endl; 
	inorder(x->right); 
}

Package* Tree::find_smallest(){
    if(root == NULL)
        return NULL;
    Package* temp=root;
    while(temp->left != NULL)
        temp = temp->left;
    return temp;
}

void Tree::arrange_th(Package* current_package){
    if(root == NULL)
        return;
    
    int total = 0;
    if(current_package->left != NULL){
        arrange_th(current_package->left);
        total += current_package->left->th;
    }

    if(current_package->right != NULL){
        arrange_th(current_package->right);
        total += current_package->right->th;
    }
    current_package->th = total + 1;
}

Package* Tree::finding_nth_smallest(Package* current, int order){//if cannot find return NULL
    int l;
	if(current->left != 0)
		l = current->left->th;
	else
		l = 0;

	if(order==l)
		return current;
	
	else if(order<l && current->left != NULL)
		return finding_nth_smallest(current->left, order);

	else if(current->right != NULL)
		return finding_nth_smallest(current->right, order - (l+1));
	else
		return NULL;

}

void Tree::count(Package* current_package){
    if(current_package->get_color() == 'b'){
        blackNode++;
        totalPackage++;
    }
    else{
        redNode++;
        totalPackage++;
    }
    if(current_package->left != NULL)
        count( current_package->left);
    else
        blackNode++;
    if( current_package->right != NULL)
        count(current_package->right);
    else
        blackNode++;
    
    
}
void print_queue(queue<int> q)
{
  while (!q.empty())
  {
    cout << q.front() << " ";
    q.pop();
  }
  cout << endl;
}

HW3_Result hw3(  int eastWarehousePackageCount, int eastWarehousePackageSizes [], int eastWarehousePackageOrdinals [],
                 int westWarehousePackageCount, int westWarehousePackageSizes [], int westWarehousePackageOrdinals [] ){
    //make east tree
    //make west tree
    Tree west;
    Tree east;
    int counter = 0;
    Package* temp;
    queue<int> ordinal_queue;
    queue<int> size_queue;

    for(int i=0; i<eastWarehousePackageCount; i++)
        east.insert(eastWarehousePackageSizes[i], eastWarehousePackageOrdinals[i]);
    
    for(int i=0; i<westWarehousePackageCount; i++)
        west.insert(westWarehousePackageSizes[i], westWarehousePackageOrdinals[i]);

    
    east.arrange_th(east.get_root());
    west.arrange_th(west.get_root());

    // until one of them is empty
    bool east_smallest_ship_flag = true, west_smallest_ship_flag = false, west_to_east_ship_flag = false, east_to_west_ship_flag = false;
    while(east.get_root() != NULL && west.get_root() != NULL){
        if(east_smallest_ship_flag){ 
            int size;
            int smallest_ordinal = INT32_MAX;

            counter = 0;
            temp = east.find_smallest();//returns package adress
            size = temp->get_size();
            size_queue.push(size);
            ordinal_queue.push(temp->get_ordinal());
            counter++;
            
            if(smallest_ordinal > temp->get_ordinal())
                smallest_ordinal = temp->get_ordinal();
            east.delete_node(temp);//delete node
            temp=east.find_smallest();
           
            while(temp != NULL && size == temp->get_size()){//collect packageges that have same size
                size_queue.push(temp->get_size());
                ordinal_queue.push(temp->get_ordinal());
                counter++;
                east.delete_node(temp);
                temp=east.find_smallest();
            }
            //-------------------tranfer to west
            //find smallest ordinal num (n) and check for nth package
            //if nth package exist accept and send cargo
            //if not do not accept packages
            
            west.arrange_th(west.get_root());
            temp = west.finding_nth_smallest(west.get_root(),smallest_ordinal-1);

            if(temp == NULL){//kaul etme
                west_to_east_ship_flag = false;
                west_smallest_ship_flag = true;
                for(int i=0;i<counter;i++){//eldekileri ekle
                    size_queue.pop();
                    ordinal_queue.pop();
                }
                if(west.get_root()==NULL || east.get_root()==NULL)
                    break;
            }
            else{
                //önce kopar
                if(west.get_root()!=NULL && east.get_root()!=NULL){
                    size_queue.push(temp->get_size());
                    ordinal_queue.push(temp->get_ordinal());//add to end of queue
                    east.delete_node(temp);
                }
                for(int i=0;i<counter;i++){//add packags to tree
                    west.insert(size_queue.front(),ordinal_queue.front());
                    size_queue.pop();
                    ordinal_queue.pop();
                }
                if(west.get_root()==NULL || east.get_root()==NULL)
                    break;
                west_to_east_ship_flag = true;
                west_smallest_ship_flag = false;
            }

            east_to_west_ship_flag = false;
            east_smallest_ship_flag = false;
        }


        else if(west_smallest_ship_flag){ 

            int size;
            int smallest_ordinal = INT32_MAX;

            counter = 0;
            temp = west.find_smallest();//returns package address 
            size = temp->get_size();
            size_queue.push(size);
            ordinal_queue.push(temp->get_ordinal());
            if(smallest_ordinal > temp->get_ordinal())
                smallest_ordinal = temp->get_ordinal();
            west.delete_node(temp);//delete node
            counter++;
            temp=west.find_smallest();
            while(temp != NULL && size == temp->get_size()){//aynı size sahip olanlari topladik
                size_queue.push(temp->get_size());
                ordinal_queue.push(temp->get_ordinal());
                west.delete_node(temp);
                counter++;
                temp=west.find_smallest();
            }

            //-------------------transfer to east 
            //find smallest ordinal num (n) and check for nth package
            //if nth package exist accept and send cargo
            //if not do not accept packages
            east.arrange_th(east.get_root());
            temp = east.finding_nth_smallest(east.get_root(),smallest_ordinal-1);

            if(temp == NULL){
                east_to_west_ship_flag = false;
                east_smallest_ship_flag = true;
                for(int i=0;i<counter;i++){
                    size_queue.pop();
                    ordinal_queue.pop();
                }
            }
            else{
                if(west.get_root()!=NULL && east.get_root()!=NULL){
                    size_queue.push(temp->get_size());
                    ordinal_queue.push(temp->get_ordinal());//add to end of queue
                    west.delete_node(temp);
                }
                for(int i=0;i<counter;i++){
                    east.insert(size_queue.front(),ordinal_queue.front());
                    size_queue.pop();
                    ordinal_queue.pop();
                }
                east_to_west_ship_flag = true;
                east_smallest_ship_flag = false;
            }
            if(west.get_root()==NULL || east.get_root()==NULL)
                    break;

            west_to_east_ship_flag = false;
            west_smallest_ship_flag = false;
        }

        else if(west_to_east_ship_flag){

            //west ten gelen temp i east a yerleştirmeye çalış 
            //önce temp in ordinali şartı sağlıyor mu diye bak
            Package* buffer;
            east.arrange_th(east.get_root());
            //print_queue(ordinal_queue);
            //print_queue(size_queue);
            buffer = east.finding_nth_smallest(east.get_root(), ordinal_queue.front()-1);
            if(buffer == NULL){
                //dont acceot package and start a east smallest ship
                size_queue.pop();
                ordinal_queue.pop();
                east_smallest_ship_flag = true;
                east_to_west_ship_flag = false;
            }
            else{
                //n th ci yi kopar buffera sonra temp i ekle bufferı temp e at
                //east to west bayrağı doğru olsun 
                size_queue.push(buffer->get_size());
                ordinal_queue.push(buffer->get_ordinal());//queue nun sonuna ekledi
                east.delete_node(buffer);
                //eldekileri agaca ekle
                east.insert(size_queue.front(),ordinal_queue.front());
                size_queue.pop();
                ordinal_queue.pop();
                
                east_to_west_ship_flag = true;
                east_smallest_ship_flag = false;
            }

            west_to_east_ship_flag = false;
            west_smallest_ship_flag = false;
        }
        else if(east_to_west_ship_flag){
            //west ten gelen temp i east a yerleştirmeye çalış 
            //önce temp in ordinali şartı sağlıyor mu diye bak
            Package* buffer;
            west.arrange_th(west.get_root());
            buffer = west.finding_nth_smallest(west.get_root(), ordinal_queue.front()-1);
            if(buffer == NULL){
                //dont acceot package and start a west smallest ship
                size_queue.pop();
                ordinal_queue.pop();
                west_smallest_ship_flag = true;
                west_to_east_ship_flag = false;
            }
            else{
                //n th ci yi kopar buffera sonra temp i ekle bufferı temp e at
                //east to west bayrağı doğru olsun 
                size_queue.push(buffer->get_size());
                ordinal_queue.push(buffer->get_ordinal());//queue nun sonuna ekledi
                west.delete_node(buffer);
                //eldekileri agaca ekle
                west.insert(size_queue.front(),ordinal_queue.front());
                size_queue.pop();
                ordinal_queue.pop();
                
                west_to_east_ship_flag = true;
                west_smallest_ship_flag = false;
            }

            east_to_west_ship_flag = false;
            east_smallest_ship_flag = false;
        }
    }
    
    HW3_Result my_res;

    if(west.get_root() != NULL){
        west.count(west.get_root());
        my_res.packageCount = west.totalPackage;
        my_res.redNodeCount = west.redNode;
        my_res.blackNodeCount = west.blackNode;        
        return my_res;
    }
    east.count(east.get_root());
    my_res.packageCount = east.totalPackage;
    my_res.redNodeCount = east.redNode;
    my_res.blackNodeCount = east.blackNode;
    
    return my_res;
}
