/*****************************************
 * UW User ID:  kkenges
 * Submitted for ECE 250
 * Semester of Submission:  (Winter|Spring|Fall) 20NN
 *
 * By submitting this file, I affirm that
 * I am the author of all modifications to
 * the provided  code.
 *****************************************/

#ifndef DYNAMIC_DEQUE_H
#define DYNAMIC_DEQUE_H

#include "Exception.h"

template <typename Type>
class Resizable_deque {
	public:
		Resizable_deque( int = 16 );
		Resizable_deque( Resizable_deque const & );
		Resizable_deque( Resizable_deque && );
		~Resizable_deque();

		Type front() const;
		Type back() const;
		int size() const;
		bool empty() const;
		int capacity() const;

		void swap( Resizable_deque & );
		Resizable_deque &operator=( Resizable_deque const& );
		Resizable_deque &operator=( Resizable_deque && );
		void push_front( Type const & );
		void push_back( Type const & );
		void pop_front();
		void pop_back();
		void clear();

	private:
		// Your member variables
        Type *array;
        int ifront;
        int iback;
        int deque_size;
        int initial_array_capacity;
        int array_capacity;

		// Any private member functions
		//   - helper functions for resizing your array?
        bool full();
        void double_capacity();
        bool can_halve();
        void halve_capacity();

	// Friends

	template <typename T>
	friend std::ostream &operator<<( std::ostream &, Resizable_deque<T> const & );
};

/////////////////////////////////////////////////////////////////////////
//                   Constructors and Destructors                      //
/////////////////////////////////////////////////////////////////////////

// Constructor
template <typename Type>
Resizable_deque<Type>::Resizable_deque( int n ):
// Your initalization list
array(),
ifront(0),
iback(n-1),
deque_size(0),
initial_array_capacity(n),
array_capacity(n)
{
	// Enter your implementation here
    if (n < 16){
        n = 16; // ensure deque has a minimum size of 16
    }
    ifront = 0; // front of deque is at start of array
    iback = n -1; // end of deque is at end of array
    deque_size = 0; // deque has no values when initialized
    initial_array_capacity = n; // the starting capacity of deque
    array_capacity = n; // current capacity of deque
    array = new Type[initial_array_capacity]; // create corresponding array isntance
}

// Copy Constructor
template <typename Type>
Resizable_deque<Type>::Resizable_deque( Resizable_deque const &deque )
{
	// Enter your implementation here
    ifront = deque.ifront;
    iback = deque.iback;
    deque_size = deque.deque_size;
    initial_array_capacity = deque.initial_array_capacity;
    array_capacity = deque.array_capacity;
    array = new Type[deque.array_capacity]; // create a new array and copy all variable and deque values over from given array
    
    for (int iterator = 0; iterator < array_capacity; iterator++){
        array[iterator] = deque.array[iterator];
    }
}

// Move Constructor
template <typename Type>
Resizable_deque<Type>::Resizable_deque( Resizable_deque &&deque )
// Your initalization list
{
	// Enter your implementation here
}

// Destructor
template <typename Type>
Resizable_deque<Type>::~Resizable_deque() {
	// Enter your implementation here
    delete []array; // delete the array and deallocate associated memory
}

/////////////////////////////////////////////////////////////////////////
//                     Public Member Functions                         //
/////////////////////////////////////////////////////////////////////////

template <typename Type>
int Resizable_deque<Type>::size() const {
	// Enter your implementation here
	return deque_size; // return current size of deque
}

template <typename Type>
int Resizable_deque<Type>::capacity() const {
	// Enter your implementation here
	return array_capacity; // retunr the current capacity of the deque
}

template <typename Type>
bool Resizable_deque<Type>::empty() const {
	// Enter your implementation here
	return (size() == 0); // returns true if the deque is empty, given by whether size is zero or not
}

template <typename  Type>
Type Resizable_deque<Type>::front() const {
	// Enter your implementation here
    if(empty()){
        throw underflow();
    }
    else{
        return array[ifront]; // if deque is empty: throw underflow, otherwise return the front value
    }
}

template <typename  Type>
Type Resizable_deque<Type>::back() const {
	// Enter your implementation here
    if(empty()){
        throw underflow();
    }
    else{
        return array[iback]; // if deque is empty: throw underflow, otherwise return the back value
    }
}

template <typename Type>
void Resizable_deque<Type>::swap( Resizable_deque<Type> &deque ) {
	// Swap the member variables
	//     std::swap( variable, deque.variable );
	// Enter your implementation here
    std::swap(ifront, deque.ifront);
    std::swap(iback, deque.iback);
    std::swap(deque_size, deque.deque_size);
    std::swap(initial_array_capacity, deque.initial_array_capacity);
    std::swap(array_capacity, deque.array_capacity);
    std::swap(array, deque.array); // swap the memeber varriables of a deque with another
}

template <typename Type>
Resizable_deque<Type> &Resizable_deque<Type>::operator=( Resizable_deque<Type> const &rhs ) {
	// This is done for you...
	Resizable_deque<Type> copy( rhs );
	swap( copy );

	return *this;
}

template <typename Type>
Resizable_deque<Type> &Resizable_deque<Type>::operator=( Resizable_deque<Type> &&rhs ) {
	// This is done for you...
	swap( rhs );

	return *this;
}

template <typename Type>
void Resizable_deque<Type>::push_front( Type const &obj ) {
	// Enter your implementation here
    if (full()){
        double_capacity(); // check if deque is full and if double capaicity if needed
    }
    ifront -=1; // move the front index position one to the left
    if (ifront <  0){
        ifront = array_capacity - 1; // if negative value , set front index to be at end of array (circular array)
    }
    array[ifront] =obj; // insert object into front of deque
    deque_size++; // increase the size of the deque by 1
}

template <typename Type>
void Resizable_deque<Type>::push_back( Type const &obj ) {
    if (full()){
        double_capacity(); // check if deque is full and if double capaicity if needed
    }
    iback +=1; // move the back index position one to the right
    if (iback == array_capacity){     // if back index is past the scope of array, set back index to be at front of array (circular array)
        iback = 0;
    }
    array[iback] =obj; // insert object into back of deque
    deque_size++;  // increase the size of the deque by 1
}

template <typename Type>
void Resizable_deque<Type>::pop_front() {
	// Enter your implementation here
    if (empty()){
        throw underflow(); // throw underflow if deque is emoty
    }
    else{
        array[ifront] = nullptr; // erase front value by setting to nullptr
        ifront += 1; // move front index to new position by shifting right by 1
        if (ifront == array_capacity){ // if front index is past the scope of array, set front index to be at front of array (circular array)
            ifront = 0;
        }
        deque_size--; // decrease the size of the deque by 1
        if(can_halve()){
            halve_capacity(); // check if deque meets criteria to be halved and if so, halve the size of deque
        }
    }
}

template <typename Type>
void Resizable_deque<Type>::pop_back() {
	// Enter your implementation here
    if(empty()){
        throw underflow(); // throw underflow if deque is emoty
    }
    else{
        array[iback] = nullptr; // erase back value by setting to nullptr
        iback -= 1; // move back index to new position by shifting left by 1
        if (iback < 0){
            iback = array_capacity - 1; // if negative value , set back index to be at end of array (circular array)
        }
        deque_size--; // decrease the size of the deque by 1
        if(can_halve()){
            this->halve_capacity(); // check if deque meets criteria to be halved and if so, halve the size of deque
        }
    }
}

template <typename Type>
void Resizable_deque<Type>::clear() {
    while(!empty()){
        pop_front(); // continue to pop off all elements until deque is empty
    }
    array_capacity = initial_array_capacity;
    ifront = 0;
    iback = array_capacity -1; // reset indexes and capacity to intial conditions
}
/////////////////////////////////////////////////////////////////////////
//                      Private member functions                       //
/////////////////////////////////////////////////////////////////////////

// Enter any private member functios (helper functions) here
template <typename Type>
bool Resizable_deque<Type>::full() {
    return (deque_size == array_capacity); // check if deque is full: when the current size is equal to current capacity
}

template <typename Type>
void Resizable_deque<Type>::double_capacity() {
    int original_capacity = array_capacity; // store capacity value before doubled
    array_capacity *= 2; // double the array capacity
    Type *array_copy = new Type[array_capacity]; // new array to copy deque values
    int index = 0;
    
    if(ifront < iback){
        for(int iterator = ifront; iterator <= iback; iterator++){
            array_copy[index] = array[iterator];
            ++index;
        } // if front index is before back index in array, iterate from ifront to iback and transfer values over to array_copy which starts at index-0, and increase value of index
    }
    else{
        for (int iterator = ifront; iterator < original_capacity; iterator++){
            array_copy[index] = array[iterator];
            ++index;
        }
        for (int iterator = 0; iterator <= iback; ++iterator){
            array_copy[index] = array[iterator];
            ++index;
        } // when ifront is after iback, iterate from ifront to end of array, and the iterate from start of array to iback; transfer values over to array_copy and increase index value along the way; array_copy starts at index-0
    }
    ifront = 0; // set ifront to be at the front of the array_copy (normalization)
    iback = index-1; //iback is set at the last index a value was trasnfered to in array_copy (normalization)
    delete [] array;
    array = array_copy; // delete old array and reassign it to the bigger copy
}

template <typename Type>
bool Resizable_deque<Type>::can_halve() {
    return ((deque_size == (array_capacity/4)) && (array_capacity > initial_array_capacity)); // check if deque can be halved: when current size is 1/4 the current capacity and the current capacity is larger than initial capacity
}

template <typename Type>
void Resizable_deque<Type>::halve_capacity() {
    int original_capacity = array_capacity;  // store capacity value before halved
    array_capacity = array_capacity/2; // divide capacity by 2
    Type *array_copy = new Type[array_capacity]; // new array to copy deque values
    int index = 0;
    
    if(ifront < iback){
        for(int iterator = ifront; iterator <= iback; iterator++){
            array_copy[index] = array[iterator];
            ++index;
        } // if front index is before back index in array, iterate from ifront to iback and transfer values over to array_copy which starts at index-0, and increase value of index
    }
    else{
        for (int iterator = ifront; iterator < original_capacity; iterator++){
            array_copy[index] = array[iterator];
            ++index;
        }
        for (int iterator = 0; iterator <= iback; iterator++){
            array_copy[index] = array[iterator];
            ++index;
        } // when ifront is after iback, iterate from ifront to end of array, and the iterate from start of array to iback; transfer values over to array_copy and increase index value along the way; array_copy starts at index-0
    }
    ifront = 0; // set ifront to be at the front of the array_copy (normalization)
    iback = index-1; //iback is set at the last index a value was trasnfered to in array_copy (normalization)
    delete [] array;
    array = array_copy; // delete old array and reassign it to the smaller copy
}
/////////////////////////////////////////////////////////////////////////
//                               Friends                               //
/////////////////////////////////////////////////////////////////////////

// You can modify this function however you want:  it will not be tested

template <typename T>
std::ostream &operator<<( std::ostream &out, Resizable_deque<T> const &list ) {
	out << "not yet implemented";

	return out;
}

#endif
