/*
 * tree.cpp
 *
 *  Created on: Jan 6, 2015
 *      Author: sfvn
 */

#include "barebones_tree.h"

#include <iostream>
#include <cmath>
using namespace std;

/**
 * Tree constructor choice
 */
Tree::Tree(list<pair<int, int>> data_graph, string initType): nextInternalNodeId(0){

    //first construct adjacency list and leaf list, same regardless of tree stucture.
    for (list<pair<int,int>>::iterator it = data_graph.begin(); it != data_graph.end(); it++){
        leaves.push_back(it->first);
        leaves.push_back(it->second);
    }

    // Find only the unique elements
    leaves.sort();
    leaves.unique();

    int N = (int) leaves.size();
    adjacencyList = Adj_list(N,data_graph);

    //Copies the elements to a vector ***** TEMPORARY *****
    vec_leaves = vector<int>();
    copy(leaves.begin(), leaves.end(), back_inserter(vec_leaves));
    
    if (initType == "Binary") {
        InitBinaryTree();
    } else { //Flat tree
        InitFlatTree();
    }

    //Correct internal number count
    rootP->updateNumInternalNodes();

}

/**
 * NOT IMPLEMENTED!!!
 */
int Tree::InitBinaryTree(){
    setRootP(makeNleafTree(0, (int) vec_leaves.size() -1 ,2) );
    return 0;
}

/**
 * NOT IMPLEMENTED!!!
 */
Node * Tree::makeNleafTree(int a, int b, int N){
    
    if ((b-a) < (N)) {
        //Create new internal node
        nodes.push_back(Node(this,getNextInternalNodeId()));
        Node * parent = & nodes.back();
        parent->setInternalNodeValue(true);
        
        //Add the up to N leafs
        for (int i = 0; (i < N) && (i <= (b-a)) ; i++) {
            nodes.push_back(Node(this,a+i));
            Node * child_P = & nodes.back();
            child_P->setParent(parent);
            parent->addChild(child_P);
        }
        return parent;
    } else {
        //Create internal node
        nodes.push_back(Node(this,getNextInternalNodeId()));
        Node * parent = & nodes.back();
        parent->setInternalNodeValue(true);
        
        //Binary split
        Node * new_child = makeNleafTree(a, b/2, N);
        parent->addChild(new_child);
        
        new_child = makeNleafTree(b/2+1, b, N);
        parent->addChild(new_child);
        return parent;
    }
    
    
    //return nullptr; //Return nullptr when root is selected.
}

/**
 * Initialise a flat treee structure
 */
int Tree::InitFlatTree(){
    /*
     * Initialisation step, here init is worse case for parameters (flat tree).
     */
    // Add a new Node for each leaf and add is as a child of root
    
    nodes.push_back(Node(this,getNextInternalNodeId()));
    rootP = &(nodes.back());


    for (list<int>::iterator it = leaves.begin(); it != leaves.end(); it++){
        nodes.push_back(Node(this,*it));
        rootP->addChild(&(nodes.back()));
    }
	rootP->updateNumInternalNodes();
    return 0;
}


/**
 * Construct flat tree
 */
Tree::Tree(list<pair<int,int>> data_graph): nextInternalNodeId(0) {

    // insert all the indexes from the edge list into leaves
    for (list<pair<int,int>>::iterator it = data_graph.begin(); it != data_graph.end(); it++){
        leaves.push_back(it->first);
        leaves.push_back(it->second);
    }

    // Find only the unique elements
    leaves.sort();
    leaves.unique();

    //Copies the elements to a vector ***** TEMPORARY *****
    vec_leaves = vector<int>();
    copy(leaves.begin(), leaves.end(), back_inserter(vec_leaves));
    cout << "Vec_leaves size: " << vec_leaves.size() << endl;
    cout << "Leaves size: " << leaves.size() << endl << flush;


    int N = (int) leaves.size();
    adjacencyList = Adj_list(N,data_graph);



    InitFlatTree();
//    InitBinaryTree();
    
    rootP->updateNumInternalNodes();
}

/**
* Special test constructor
* (First element in tree_struct_graph must contain root)
*/
Tree::Tree(list<pair<int,int>> data_graph, list<pair<int,int>> tree_struct_graph,
           vector<int> data_leaf_relation): nextInternalNodeId(0) {

    // - Construct adj list from data_graph
    int N = ((int) data_leaf_relation.size())/2; //Number of leaves in graph
    adjacencyList = Adj_list(N, data_graph);

    // - Construct the tree from tree_struct_graph
    //Get first relation parrent --> child, assumption the root is first
    pair<int,int> element = tree_struct_graph.front();
    tree_struct_graph.pop_front();

    nodes.push_back(Node(this, element.first));
    rootP = &(nodes.back());

    //Add the first node as a child
    Node new_child = Node(this, element.second);

    nodes.push_back(new_child);
    rootP->addChild(&(nodes.back()));

    //Insert parrent-->child relations for the rest
    while (!tree_struct_graph.empty()) {
        //Get next relation parrent --> child
        element = tree_struct_graph.front();
        tree_struct_graph.pop_front();

        Node * parent = this->getNode(element.first);

        //Check if the child node is already created,
        //  then it is a nullptr if it does not exist
        Node* existing_nodeP = this->getNode(element.second);
        if (existing_nodeP==nullptr) {
            new_child = Node(this,element.second);
            nodes.push_back(new_child);
            parent->addChild(& (nodes.back()));
        } else{ //Exists, and we just need to add a pointer to its child
            parent->addChild(existing_nodeP);
        }


    }

    //Updates internal count.
    rootP->updateNumInternalNodes();


    /*
     * For each leaf node, correct the leaf ID, so it correspond to the data ID
     *  each internal node is assigned a unique negative number.
     */
    rootP->setLeafId(getNextInternalNodeId());

    for (auto it = nodes.begin(); it != nodes.end(); it++) {
        if (it->isInternalNode()){ //Internal node
            it->setLeafId(getNextInternalNodeId());

        } else { //Leaf node
             //Find what the fake_id corresponds to in real id
            int fake_id = it->getLeafId();
            it->setLeafId(data_leaf_relation[fake_id]);
        }
    }

}

/**
* Copy constructor
*
*/
Tree::Tree(Tree const &old_tree)  {
    nodes = old_tree.nodes;
    leaves = old_tree.leaves;
    vec_leaves =old_tree.vec_leaves;
    graph = old_tree.graph;
    adjacencyList = old_tree.adjacencyList;
    nextInternalNodeId = old_tree.nextInternalNodeId;

    //Find new root node
    for (auto it = nodes.begin(); it != nodes.end();++it){
        if (it->getParent() == nullptr) {
            rootP = &(*it);
        }else{
        }
    }
    
    for (auto it = nodes.begin(); it != nodes.end();++it){
        if (it->getParent() != nullptr){
            //parent update
            int old_parent = it->getParent()->getLeafId();
            it->setParent(getNode(old_parent));
            
        }
        
        //children update
        list<Node *> children = it->getChildren();
        list<Node *> new_children;
        for (auto it_child = children.begin();
             it_child != children.end(); ++it_child) {

            //Find id, and find it in
            int id = (*it_child)->getLeafId();
            new_children.push_back(getNode(id));

        }
        it->setChildren(new_children);

        //tree pointer update
        it->setTreePointer(this);

    }

    //Update rest
//    for (auto it nodes)

}

int Tree::getNextInternalNodeId(){
    --nextInternalNodeId;
//    cout << "New internal node: "+ to_string(nextInternalNodeId) << endl;
    return nextInternalNodeId;
//  return --nextInternalNodeId;
}

/**
 * Finds a specific node (characterized by an unique id) and
 * returns a pointer to this node or nullptr if it isn't pressent
 * (Basic implementation!)
 */
Node * Tree::getNode(int leaf_id){
    //Iterates over all internal and leaf nodes
    if (leaf_id == rootP->getLeafId()) { // is root?
        return rootP;
    }
    for(list<Node>::iterator it = nodes.begin();
        it != nodes.end(); it++){

        if (it->getLeafId() == leaf_id) {
            return &(* it);
        }

    }
    return nullptr;
}

list<pair<int, int>> Tree::getCountsAll(){
    return rootP->getCountsAll();
}


/**
    Get random node in tree (based on nodes list)
    All nodes are weigthed equally
*/
Node * Tree::getRandomScion() {
    int num_nodes = (int) nodes.size();
    // Sample random id in nodes list
    int random_node_id = (int) round( ((double)rand()/RAND_MAX)*(num_nodes-1));
    list<Node>::iterator it = nodes.begin();
    for (int i = 0; i!= random_node_id; ++i) {
            // loop through list until you find the element
            // NB! Convert all this to vector for random access PLZ!!!?!?
        ++it;
    }
    return &(*it);
}


/**
* Get random node in tree (recursive operation)
* - Chooses internal nodes with weight 2 and leaves with weight 1
* - calls getRandomChild from node-class
*/

Node * Tree::getRandomStock() {
    return rootP->getRandomDescendant();
}

void Tree::removeNode(Node * nodeP){
    nodes.remove(*nodeP);
}

Adj_list Tree::getAdjacencyList(){
    return adjacencyList;
}

void Tree::setRootP(Node * node){
    rootP = node;
}




/**
 * Modifies tree by random regrafting
 * Returns the Metropolis-Hasting 'move-ratio' (NB! non-logarithmic)
 */
double Tree::regraft(){
// TODO: finish the regrafting
    Node * scionP = this->getRandomScion();
    if(!(scionP==rootP)){
//        cout << "cutting: " << scionP->getLeafId() << endl;
        int n_collapsed = this->cutSubtree(scionP);
        rootP->updateNumInternalNodes();
        
//        cout << "Mutilated tree :'( \n" << toString() << endl << flush;
        
        Node * stockP = this->getRandomStock();
//        cout << "inserting at: " << stockP->getLeafId();
        // TODO: random child or sibling
        bool unbiased_coinflip = ((double) rand()/RAND_MAX) > 0.5;
//        cout << " , as child : "+to_string(unbiased_coinflip) << endl;
        int n_created = this->insertSubtree(stockP, scionP, unbiased_coinflip);
        rootP->updateNumInternalNodes();

        // Move probabilities
        int n_nodes = (int)nodes.size();
        double p_scion = 1.0/(n_nodes);
        double p_stock = 1.0/(n_nodes - n_collapsed + n_created);

        return p_stock/p_scion;
    } else{ // scion was root - ratio of move probabilities is 1
        return 1;
    }
}

/**
 * Regraft specific nodes !!! ASSUMES ITS A VALID OPERATION!!
 */
void Tree::regraft(int scionVal, int stockVal){
    // TODO: finish the regrafting
    Node * scionP = this->getNode(scionVal);
    if(!(scionP==rootP)){
        cout << "cutting: " << scionP->getLeafId() << endl;
        this->cutSubtree(scionP);
        rootP->updateNumInternalNodes();

        Node * stockP = this->getNode(stockVal);
        cout << "inserting: " << stockP->getLeafId() << endl;
        // TODO: random child or sibling
        this->insertSubtree(stockP, scionP, false);
        rootP->updateNumInternalNodes();
    }
}

string Tree::toString(){
//    string sAdj = adjacencyList.toString();
    return rootP->toString(); //+ sAdj ;
}


/**
* Evaluating log-likelihood x prior of tree
*/

double Tree::evaluateLogLikeTimesPrior(double alpha, double beta, int rho_plus, int rho_minus){
//    double root_node_contribution = this->rootP->evaluateNodeLogLike(alpha,beta,rho_plus,rho_minus);
//    double root_subtree_contribution = this->rootP->evaluateSubtreeLogLike(alpha
//                                                    ,beta,rho_plus,rho_minus);
//
//    return root_node_contribution + root_subtree_contribution;
    return rootP->evaluateSubtreeLogLike(alpha,beta,rho_plus,rho_minus);
}

int Tree::cutSubtree(Node * scionP){
    // assumes that scionP doesn't point to root.
    Node * parentP = scionP->getParent();
    int collapsed = parentP->removeChild(scionP);
    scionP->setParent(nullptr);
    return collapsed;
}

Node * Tree::getRoot(){
    return rootP;
}

int Tree::insertSubtree(Node * stockP, Node * scionP, bool asChild){

    int created = 0;
    //TODO: Fix less hot hotfix
    // Cannot be added as a child to a leaf, only as sibling
    if (! stockP->isInternalNode()){
        asChild = false;
    }

    if(asChild){
        stockP->addChild(scionP);
    }else{ //As sibling

        // Create a new node
        nodes.push_back(Node(this,getNextInternalNodeId()));
        Node * new_parent = &(nodes.back());
        new_parent->setInternalNodeValue(true);

        // Constuct and add a new parent
        Node * stock_parent = stockP->getParent();
        if(stock_parent != nullptr){ // if stock is not root
            new_parent->setParent(stock_parent);
            stock_parent->addChild(new_parent);
            stock_parent->removeChild(stockP);
        } else {// if stock is root
            setRootP(new_parent);
        }


        created++;

        new_parent->addChild(stockP);
        new_parent->addChild(scionP);
    }
    return created;
}
