/*****************************************
 * UW User ID:  kkengesw
 * Submitted for ECE 250
 * Semester of Submission: Fall 2021
 *
 * By submitting this file, I affirm that
 * I am the author of all modifications to
 * the provided  code.
 *****************************************/

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include "Graph.h"

using namespace std;
int main(int argc, char** argv)
{
	//first input argument is always the program name...
    char* fileName = argv[1];
    
    Graph<string> graph (50, " ");
	//open the file
	ifstream fin(fileName);
	
	if(!fin)
	{
		cout<< "File not found" << endl;
		return EXIT_FAILURE;
	}
	
	string line;
	while(getline(fin,line)) //retrieve lines from the input file and place them in the adjcency list accordingly
	{
        if (line.empty()){
            do{
                getline(fin,line);
            }while(line.empty()); //if the line is empty continue to get the next line until the line is not empty
        }
        string first_8_characters = line.substr(0,8);
        if(first_8_characters != "#include"){
            graph.insert_vertex(line); //check if the first 8 characters of the line has "#include", if it doesn't then the line contains a main vertex
        }
        else{
            line = line.erase(0,10);
            line.resize(line.size() - 1);
            graph.insert_edge(line); // otherwise the line is a vertex that main vertex points to (consider as and edge)
        }
    }
    
    graph.check_vertexes(); //checks if there any edges that haven't been declared as a main vertex, if so th e function updates the main vertex row of the adjacency list
    
    graph.disp_order(); // creates an in_degree order, does a topological sort, and then displays one valid order in which the functions can be compiled
    
	return EXIT_SUCCESS;
}
