#ifndef NODE_H
#define NODE_H
#include "object.h"
using namespace std;

class Node {
    public:
         Object data; //a class that stores all the information about a shape displayed on the screen
 	 Node * next; //pointer to the next object in the linked list
	Node();
	~Node();
};

#endif
