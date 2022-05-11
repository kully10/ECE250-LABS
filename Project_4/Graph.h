/*****************************************
 * UW User ID:  kkengesw
 * Submitted for ECE 250
 * Semester of Submission: Fall 2021
 *
 * By submitting this file, I affirm that
 * I am the author of all modifications to
 * the provided  code.
 *****************************************/
#ifndef Graph_h
#define Graph_h

#include <iostream>
#include <fstream>
#include <cstdlib>
#include "string"
#include "vector"

using namespace std;

template <typename Type>
class Graph {
    public:
        Graph( int = 10 , Type = 0);
        ~Graph();
    
        void insert_vertex(Type vertex);
        void insert_edge(Type edge);
        void check_vertexes();
        void disp_order();

    private:
        Type **matrix;
        Type graph_fill;
        int starting_array_capacity;
        int graph_size;
        int vertex_row;
        int *in_degrees;
        Type *array_order;
        void in_degree_array();
        void topological_sort();
};

/////////////////////////////////////////////////////////////////////////
//                   Constructors and Destructors                      //
/////////////////////////////////////////////////////////////////////////

// Constructor
template <typename Type>
Graph<Type>::Graph( int n, Type fill_value):
list(),
graph_fill(fill_value),
starting_array_capacity(n),
graph_size(0),
in_degrees(),
array_order(),
vertex_row(0)
{
    if (n < 10){ // ensure the list has a minimum square dimension of 10
        n = 10;
    }
    graph_fill = fill_value; // fill all array indexes of the adjacency list with a filler, to make searching easier
    graph_size = 0; //graph starts with a size of 0 vertexes
    vertex_row = 0; //place main vertexes starting at row 0
    starting_array_capacity = n; //the number of main vertexes the graph can store
    list = new Type * [n]; //declare "list" to point to an array of pointers
    for (int i = 0; i < n; i++){
        list[i] = new Type [n]; // each index in "list" points to an array (produces and adjacney list that looks like a sqaure matrix)
        for (int j = 0; j<n; j++){
            list[i][j] = fill_value; //iterate thorugh all indexes of the adjacency list and fill with "graph_fill"
        }
    }
}

// Destructor
template <typename Type>
Graph<Type>::~Graph() {
    for(int i = 0; i < starting_array_capacity; i++){
        delete []list[i]; //iterate through the pointer array, and delete each array that the pointer array's index points to
    }
    delete [] list; //delete the pointer array
}

/////////////////////////////////////////////////////////////////////////
//                     Public Member Functions                         //
/////////////////////////////////////////////////////////////////////////template <typename Type>
template <typename Type>
void Graph<Type>::insert_vertex(Type vertex) {
    for(int j = 0; j <graph_size; j++){
        if (list[j][0] == vertex){ //check if the vertex has already been placed inside the main vertex column before
            vertex_row = j; // if so save the row where the vertex was already placed in order to input its following edge point
            return; // exit function at this point as the vertex should not be placed again
        }
    }
    
    int i;
    for ( i =0; i <= starting_array_capacity; i++){ //search for the next open spot in the main vertex column that is open
        if(list[i][0] == graph_fill){
            break;
        }
    }
    if (i >= starting_array_capacity){ //exit program in the case that the initialized graph is not large enough to store all vetrexes
        cout<< "Reinitialize graph with a larger adjacency list." << endl;
        exit (EXIT_FAILURE);
    }
    list[i][0] = vertex; // places the inputed vertex value into spot determined above
    vertex_row = i; // save vertex_row to input its following edge points
    graph_size += 1; // graph size is increased by 1 when vertex is placed
}

template <typename Type>
void Graph<Type>::insert_edge(Type edge) {
    for (int j = 1; j < starting_array_capacity; j++){ // check if any of the previously placed edge points for the current vertex is the same as this edge value
        if(list[vertex_row][j] == edge){
            return; // if edge is already there exit function as edge should not be placed again
        }
    }
    
    int i;
    for ( i =0; i <= starting_array_capacity; i++) //search for the next open spot in the main vertex's row
    {
        if(list[vertex_row][i] == graph_fill){
            break;
        }
    }
    if (i >= starting_array_capacity){
        cout<< "Reinitialize graph with a larger adjacency list." << endl; //exit program in the case that the initialized graph is not large enough to store all edges
        exit (EXIT_FAILURE);
    }
    list[vertex_row][i] = edge; // insert edge at the spot detremined above
}

template <typename Type>
void Graph<Type>::check_vertexes(){
    for (int i = 0; i < graph_size; i++){
        for(int j = 1; j <starting_array_capacity; j++){ //iterate through every single index in the adjcanecy list other than the column containing the main vertexes
            if (list[i][j] == graph_fill){
                break;
            }
            int checker = 0;
            for(int n=0; n < graph_size; n++){
                if (list[i][j] != list[n][0] ){ //check if the the edge (index value from before) has already been placed in the main vertex column
                    checker ++; // increase counter every single time the main vertex column index does not equal the current edge
                }
                else{
                    break;
                }
                if (checker == graph_size){ //if counter has reached graph_size, it means that edge point is not in the main vertex column, and therfore must be placed there so future refernces(sorting)
                    insert_vertex(list[i][j]);
                }
            }
        }
    }
}
    
template <typename Type>
void Graph<Type>::disp_order() {
    topological_sort(); //carries out a topological sort and stores the order in an array_order
    for ( int i = graph_size -1; i >= 0; i--){ //iterate sthrough the array in a areverse manner and displays each index value to the terminal
        cout << array_order[i] << endl;
    }
}

/////////////////////////////////////////////////////////////////////////
//                      Private member functions                       //
/////////////////////////////////////////////////////////////////////////

template <typename Type>
void Graph<Type>::in_degree_array() {
    in_degrees = new int[graph_size]; // create an array to store the in degrees of each vertex
    for (int i =0; i < graph_size; i++){//iterates through the column in the adjacency list that stores all the main vertexes
        Type vertex_search = list[i][0]; //store a main vertex
        int vertex_count = 0; //initialize a variable to keep count of the in_degree
        for (int j = 0; j < graph_size; j++){
            for(int n = 1; n < starting_array_capacity; n++){ //iterates through all edge points that were inserted
                if (list[j][n]== graph_fill){
                    break;
                }
                else if (list[j][n]== vertex_search){ // if there is an instance where the edge point is equal to the main vertex in question, increase the counter
                    vertex_count++;
                }
            }
        }
        in_degrees[i] = vertex_count; //store the in degree of the vertex at the index number that it is found in the main vertex column of the adjacency list
    }
}

template <typename Type>
void Graph<Type>::topological_sort(){
    in_degree_array(); // call to create the array of indegrees
    array_order = new Type[graph_size]; // create an array to store the order (think of this array in place of the queue, I used an array for the ease of keeping track of the order)
    
    int push_count = 0; //variable keeps track of where to insert the next vertex that is "pushed on"
    for (int i =0; i <graph_size; i++){
        if(in_degrees[i]==0){
            array_order[push_count] = list[i][0]; //push all source vetices (in degree = 0) onto the order array
            in_degrees[i]= -1; // change the in degree of the source vertices to a non valid number, prevents confusiosn when checking for an in degree of 0
            push_count++; //increase to point to next index in array
        }
    }
    
    vector<Type> vector{};
    Type vector_search = graph_fill;
    int pop_count = 0; // variable keeps track of the next index(front) after a "pop" has occured
    while(push_count != graph_size){ //as we need to push all vertices to have a complete order, we loop until the push_count is equal to number of vertices(graph_size)
        vector_search = array_order[pop_count]; //access the "front" of the array
        pop_count++; //increase so that the next "pop" at the "front" occurs at the next index
        int vector_size = 0;
        for(int j = 0; j < graph_size; j++){
            if (list[j][0] ==vector_search){ //iterate to find the row that the vertex is loacted
                for (int n=1; n <starting_array_capacity; n++){
                    if(list[j][n] != graph_fill){
                        vector.push_back(list[j][n]); //push all the edge points of that main vertex into a vector
                        vector_size++; // keep track of how many times a valid edge point is pushed onto the vector
                    }
                }
            }
        }
        
        for( int t = 0; t < vector_size; t++){//loop the number of times that an edgepoint was pushed onto the vector
            for(int i = 0; i < graph_size; i++){
                if (list[i][0] == vector.back() && in_degrees[i] > 0){ //find the row in the  main vertex column of the adjacencly list that corresponds to the edge point/vertex
                    in_degrees[i]--; //decrement the in degree of that edge point/vertex by 1 if it has an indegree greater than 0
                }
            }
            vector.pop_back();//move to the next edge point stored in vector
        }
        
        for(int n = 0; n < graph_size; n++){ //iterate through the in degree array
            if(in_degrees[n] == 0){
                array_order[push_count] = list[n][0]; //if any vertex has an in degree of 0, "push" it onto array_order
                in_degrees[n] = -1; // change the in degree of the vertices to a non valid number, prevents confusiosn when checking for an in degree of 0
                push_count++; //increase to point to next index in array
            }
       }
    }
}

#endif
