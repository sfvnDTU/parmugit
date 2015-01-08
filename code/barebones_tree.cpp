/*
 * tree.cpp
 *
 *  Created on: Jan 6, 2015
 *      Author: sfvn
 */

#include "barebones_tree.h"

#include <iostream>
using namespace std;


Tree::Tree(list<tuple<int,int>> input_graph) {
	root = Node();
    graph = input_graph;

    // insert all the indexes from the edge list into leaves
	for (list<tuple<int,int>>::iterator it = graph.begin(); it != graph.end(); it++){
        leaves.push_back(get<0>(*it));
        leaves.push_back(get<1>(*it));
    }

    // Find only the unique elements
    leaves.sort();
    leaves.unique();

    // Add a new Node for each leaf and add is as a child of root
    for (list<int>::iterator it = leaves.begin(); it != leaves.end(); it++){
        Node aNode = Node(*it);
        nodes.push_back(aNode);
        root.addChild(&aNode);
    }

    //root.toString();

    // Node * p = root.children.front();

    // cout << p->toString();

    // gives segfault when acessing the childs leaves list

}


string Tree::toString(){
    return root.toString();
}

/////////////////////////
// Node class functions //
//////////////////////////

Node::Node() {
    // Trivial constructor.
	parent = nullptr;
}

Node::Node(int L) {
    // Construct a node with the leaf L
    // This defines a leaf-node
	parent = nullptr;
	leaves.push_back(L);
}

list<int> Node::getLeaves() {
	return leaves;
}

Node * Node::getParent() {
	return parent;
}

void Node::setParent(Node * new_parent) {
    // Set the pointer "parent" to a new value.
	parent = new_parent;
}

void Node::addChild(Node * childP) {
    // Add a child by:
    //  - Setting the childs parent pointer.
    //  - Adding the childs pointer to the child list.
    //  - Inserting the childs leaves-list on to the end of the parents.
    childP->setParent(this);
	children.push_back(childP);
	leaves.splice(leaves.end(),childP->getLeaves());
}


string Node::toString(){
    // Building a string representing the tree by printing all of the leaf-Sets

    string s = "leaves: (";

    if(!leaves.empty()){
        cout << leaves.size() << endl;
        for (list<int>::iterator it = leaves.begin(); it != leaves.end(); it++){
            s += "," + to_string(*it);
        }
    }

    s += ")\n";

    // -- Recurse into children to print the entire subtree.

    //for (list<Node *>::iterator it = children.begin(); it != children.end(); it++){
    //    Node * childP = *it;
        //Node child = *childP;
//        string s = childP;
        //s += child.toString();
    //}

    return s ;
}



