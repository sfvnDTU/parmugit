//============================================================================
// Name        : main.cpp
// Author      : Julian
// Version     :
// Copyright   : It is OURS!!
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <cstdlib> // setting random seed
#include <time.h> // ----
#include <chrono> // chrono::timepoint, chono::system_clock

#include "barebones_tree.h"
#include "sampler.h"
#include "iofilehandler.h"
using namespace std;

void testNetwork(string,int);

int main() {
    srand ((unsigned int) time(NULL)); // set random seed

    string data_file_name = "data/karate_edgelist.txt";
    //data_file_name = "data/celegans_edgelist.txt";


    /**
     * Testing a network
     */
    int num_iterations = 100;
    testNetwork(data_file_name,num_iterations);

	return 0;
}
/**
 * Test a network specifed by an edge list when performing num_of_iterations
 */
void testNetwork(string data_file_name, int num_of_iterations){
    cout << "Running on: " << data_file_name << endl;
    IoFileHandler data_file(data_file_name,0);
    data_file.read_graph();

    Tree new_tree(data_file.getDataEl()); // initialize flat tree
//    cout << new_tree.toString() << endl;

    Sampler sampler = Sampler(new_tree,0.5, 0.5, 1, 1);

    chrono::time_point<chrono::system_clock> start, end;
    start = chrono::system_clock::now();

    sampler.run(num_of_iterations);

    end = chrono::system_clock::now();
    chrono::duration<double> elapsed_seconds = end-start;
    std::time_t end_time = std::chrono::system_clock::to_time_t(end);

    std::cout << "finished computation at " << std::ctime(&end_time)
    << "elapsed time: " << elapsed_seconds.count() << " sec.\n"
    << "mean elapsed time per regraft: " << elapsed_seconds.count()/((double) num_of_iterations) << " sec." << endl;

    cout << sampler.getLast().toString() << endl;
}
