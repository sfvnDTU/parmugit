/*
 * tree.h
 *
 *  Created on: Jan 6, 2015
 *      Author: sfvn
 */

#ifndef TREE_H_
#define TREE_H_

#include <unordered_map>
#include <vector>
#include <utility>
#include <set>
#include <list>
#include <string>
#include <algorithm>
#include "node.h"


class Tree {

private:
    Node * rootP;
    std::list<Node> nodes;
    std::list<int> leaves;
    std::vector<int> vec_leaves;
    std::list<std::pair<int, int>> graph;
    Adj_list adjacencyList;
    int InitBinaryTree();
    Node * makeNleafTree(int, int, int);

    int InitFlatTree();
    int nextInternalNodeId;
	// TODO make functions private when not debugging them.

public:
    // Constructors
    Tree(std::list<std::pair<int, int>>); // make the naive tree building in the adjacency matrix.
    Tree(std::list<std::pair<int, int>>, std::string);
    Tree(std::list<std::pair<int, int>>, std::list<std::pair<int, int>>
         , std::vector<int>); // Builds tree based on data, tree and data-tree relation

    //TODO: Rule of "5"
    //virtual ~Tree();                     // Default destructor
    Tree(Tree const &);                  // Copy constructor
    //Tree(Tree&&) = default;                    // Move constructor
    //Tree& operator=(const Tree&) & = default;  // Copy assignment operator
    //Tree& operator=(Tree&&) & = default;       // Move assignment operator

    // Get and set - Trivial stuff
    Adj_list getAdjacencyList();
    Adj_list * getAdjacencyListP();
    void setRootP(Node *);
    Node * getRoot(); // Returns pointer tree-root
    int getNextInternalNodeId();
    Node * getNode(int); // Get node by unique 'leaf-id'
    void removeNode(Node *);  // Removes node from nodes-list (called from Node class)

    // Regrafting
	double regraft(); // return new regrafted tree
    void regraft(int,int); // DEBUG testing specific move;
	Node * getRandomScion();
	Node * getRandomStock();
	int cutSubtree(Node * ); //Modifies the tree and returns change in the number of internal nodes
    int insertSubtree(Node *, Node *, bool); //Modifies the tree and returns change in the number of internal nodes


    // Evaluating likelihood
    double evaluateLogLikeTimesPrior(double, double, int, int);

    // Print
    std::string toString();

    //TODO: Dont know what to do with this...?
    // void recalculate(); // calculate all of the counts on the tree from the graph
};



#endif /* TREE_H_ */

