/*****************************************
 * UW User ID:  kkengesw (Kullalon Kengeswararajah)
 * Submitted for ECE 250
 * Semester of Submission:  Fall  2021
 *
 * By  submitting this file, I affirm that
 * I am the author of all modifications to
 * the provided code.
 *****************************************/

#ifndef RPOVDGBQN9TIEO3P
#define RPOVDGBQN9TIEO3P

#include "Exception.h"
#include "ece250.h"
#include <cassert>

template <typename Type>
class Search_tree {
	public:
		class Iterator;

	private:
		class Node {
			public:
				Type node_value;
				int tree_height;

				// The left and right sub-trees
				Node *left_tree;
				Node *right_tree;

				// Hint as to how you can create your iterator
				// Point to the previous and next nodes in linear order
				Node *previous_node;
				Node *next_node;

				// Member functions
				Node( Type const & = Type() );

				void update_height();

				int height() const;
				bool is_leaf() const;
				Node *front();
				Node *back();
				Node *find( Type const &obj );

				void clear();
                void check_fix_tree(Node *&to_this);
                void balance_right_tree(Node *&to_this);
                void balance_left_tree(Node *&to_this);
				bool insert( Type const &obj, Node *&to_this );
				bool erase( Type const &obj, Node *&to_this );
		};

		Node *root_node;
		int tree_size;

		// Hint as to how to start your linked list of the nodes in order 
		Node *front_sentinel;
		Node *back_sentinel;

	public:
		class Iterator {
			private:
				Search_tree *containing_tree;
				Node *current_node;
				bool is_end;

				// The constructor is private so that only the search tree can create an iterator
				Iterator( Search_tree *tree, Node *starting_node );

			public:
				// DO NOT CHANGE THE SIGNATURES FOR ANY OF THESE
				Type operator*() const;
				Iterator &operator++();
				Iterator &operator--();
				bool operator==( Iterator const &rhs ) const;
				bool operator!=( Iterator const &rhs ) const;

			// Make the search tree a friend so that it can call the constructor
			friend class Search_tree;
		};

        // DO NOT CHANGE THE SIGNATURES FOR ANY OF THESE
		Search_tree();
		~Search_tree();

		bool empty() const;
		int size() const;
		int height() const;

		Type front() const;
		Type back() const;

		Iterator begin();
		Iterator end();
		Iterator rbegin();
		Iterator rend();
		Iterator find( Type const & );

		void clear();
		bool insert( Type const & );
		bool erase( Type const & );

	// Friends

	template <typename T>
	friend std::ostream &operator<<( std::ostream &, Search_tree<T> const & );
};

//////////////////////////////////////////////////////////////////////
//                Search Tree Public Member Functions               //
//////////////////////////////////////////////////////////////////////

// The initialization of the front and back sentinels is a hint
template <typename Type>
Search_tree<Type>::Search_tree():
root_node( nullptr ),
tree_size( 0 ),
front_sentinel( new Search_tree::Node( Type() ) ),
back_sentinel( new Search_tree::Node( Type() ) ) {
	front_sentinel->next_node = back_sentinel;
	back_sentinel->previous_node = front_sentinel;
}

template <typename Type>
Search_tree<Type>::~Search_tree() {
	clear();  // might as well use it...
	delete front_sentinel;
	delete back_sentinel;
}

template <typename Type>
bool Search_tree<Type>::empty() const {
	return ( root_node == nullptr );
}

template <typename Type>
int Search_tree<Type>::size() const {
	return tree_size;
}

template <typename Type>
int Search_tree<Type>::height() const {
	return root_node->height();
}

template <typename Type>
Type Search_tree<Type>::front() const {
	if ( empty() ) {
		throw underflow();
	}

	return root_node->front()->node_value;
}

template <typename Type>
Type Search_tree<Type>::back() const {
	if ( empty() ) {
		throw underflow();
	}

	return root_node->back()->node_value;
}

template <typename Type>
typename Search_tree<Type>::Iterator Search_tree<Type>::begin() {
	return empty() ? Iterator( this, back_sentinel ) : Iterator( this, root_node->front() );
}

template <typename Type>
typename Search_tree<Type>::Iterator Search_tree<Type>::end() {
	return Iterator( this, back_sentinel );
}

template <typename Type>
typename Search_tree<Type>::Iterator Search_tree<Type>::rbegin() {
	return empty() ? Iterator( this, front_sentinel ) : Iterator( this, root_node->back() );
}

template <typename Type>
typename Search_tree<Type>::Iterator Search_tree<Type>::rend() {
	return Iterator( this, front_sentinel );
}

template <typename Type>
typename Search_tree<Type>::Iterator Search_tree<Type>::find( Type const &obj ) {
	if ( empty() ) {
		return Iterator( this, back_sentinel );
	}

	typename Search_tree<Type>::Node *search_result = root_node->find( obj );

	if ( search_result == nullptr ) {
		return Iterator( this, back_sentinel );
	} else {
		return Iterator( this, search_result );
	}
}

template <typename Type>
void Search_tree<Type>::clear() {
	if ( !empty() ) {
		root_node->clear();
		root_node = nullptr;
		tree_size = 0;
	}

	// Reinitialize the sentinels
	front_sentinel->next_node = back_sentinel;
	back_sentinel->previous_node = front_sentinel;
}

template <typename Type>
bool Search_tree<Type>::insert( Type const &obj ) {
	if ( empty() ) {
		root_node = new Search_tree::Node( obj );
		tree_size = 1;
        
        front_sentinel->next_node = root_node;
        back_sentinel->previous_node = root_node;
        
        root_node->next_node = back_sentinel;
        root_node->previous_node = front_sentinel; //Reassign the previous and next node after root node has been created in order to maintain linear order in tree

		return true;
	} else if ( root_node->insert( obj, root_node ) ) {
		++tree_size;
		return true;
	} else {
		return false;
	}
}

template <typename Type>
bool Search_tree<Type>::erase( Type const &obj ) {
	if ( !empty() && root_node->erase( obj, root_node ) ) {
		--tree_size;
		return true;
	} else {
		return false;
	}
}

//////////////////////////////////////////////////////////////////////
//                   Node Public Member Functions                   //
//////////////////////////////////////////////////////////////////////

template <typename Type>
Search_tree<Type>::Node::Node( Type const &obj ):
node_value( obj ),
left_tree( nullptr ),
right_tree( nullptr ),
next_node( nullptr ),
previous_node( nullptr ),
tree_height( 0 ) {
	// does nothing
}

template <typename Type>
void Search_tree<Type>::Node::update_height() {
	tree_height = std::max( left_tree->height(), right_tree->height() ) + 1;
}

template <typename Type>
int Search_tree<Type>::Node::height() const {
	return ( this == nullptr ) ? -1 : tree_height;
}

// Return true if the current node is a leaf node, false otherwise
template <typename Type>
bool Search_tree<Type>::Node::is_leaf() const {
	return ( (left_tree == nullptr) && (right_tree == nullptr) );
}

// Return a pointer to the front node
template <typename Type>
typename Search_tree<Type>::Node *Search_tree<Type>::Node::front() {
	return ( left_tree == nullptr ) ? this : left_tree->front();
}

// Return a pointer to the back node
template <typename Type>
typename Search_tree<Type>::Node *Search_tree<Type>::Node::back() {
	return ( right_tree == nullptr ) ? this : right_tree->back();
}

template <typename Type>
typename Search_tree<Type>::Node *Search_tree<Type>::Node::find( Type const &obj ) {
	if ( obj == node_value ) {
		return this;
	} else if ( obj < node_value ) {
		return (left_tree == nullptr) ? nullptr : left_tree->find( obj );
	} else {
		return ( right_tree == nullptr ) ? nullptr : right_tree->find( obj );
	}
}

// Recursively clear the tree
template <typename Type>
void Search_tree<Type>::Node::clear() {
	if ( left_tree != nullptr ) {
		left_tree->clear();
	}

	if ( right_tree != nullptr ) {
		right_tree->clear();
	}

	delete this;
}

template <typename Type>
void Search_tree<Type>::Node::check_fix_tree(Search_tree<Type>::Node *&to_this) {
    if ((right_tree->height() - left_tree->height()) >= 2 || (left_tree == nullptr )){ //right tree needs balancing when it has 2 or more nodes than left tree or when left tree has nothing
        balance_right_tree(to_this);
    }
    else if ((left_tree->height() - right_tree->height()) >= 2 || (right_tree == nullptr )){ //left tree needs balancing when it has 2 or more nodes than right tree or when right tree has nothing
        balance_left_tree(to_this);
    }
}

template <typename Type>
void Search_tree<Type>::Node::balance_right_tree(Search_tree<Type>::Node *&to_this) {
    
    if (right_tree->right_tree->height() > right_tree->left_tree->height()) { //Balance tree according to case 1 (RR balancing)
        // initial new node pointers to rearrange tree nodes
        Node *b = right_tree;
        Node *BR = b->left_tree;
    
        b->left_tree = to_this;
        to_this = b;
        right_tree = BR;
        
        update_height(); //update the height of the tree after balancing
    }
    else if(right_tree->left_tree->height() > right_tree->right_tree->height()){ //Balance tree according to case 2 (RL balancing)
        // initial new node pointers to rearrange tree nodes
        Node *b = right_tree;
        Node *d = b->left_tree;
        Node* DL = d->right_tree;
        Node* DR = d->left_tree;
    
        d->right_tree = b;
        d->left_tree = to_this;
        to_this = d;
        b->left_tree = DL;
        right_tree = DR;
    
        update_height(); //update the height of the tree after balancing
    }
}

template <typename Type>
void Search_tree<Type>::Node::balance_left_tree(Search_tree<Type>::Node *&to_this) {
        
    if (left_tree->left_tree->height() > left_tree->right_tree->height()) { //Balance tree according to case 1 (LL balancing)
        // initial new node pointers to rearrange tree nodes
        Node *b = left_tree;
        Node *BR = b->right_tree;
        
        b->right_tree = to_this;
        to_this = b;
        left_tree = BR;
        
        update_height(); //update the height of the tree after balancing
    }
    else if(left_tree->right_tree->height() > left_tree->left_tree->height()){ //Balance tree according to case 2 (LR balancing)
        // initial new node pointers to rearrange tree nodes
        Node *b = left_tree;
        Node *d = b->right_tree;
        Node *DL = d->left_tree;
        Node *DR = d->right_tree;
        
        d->left_tree = b;
        d->right_tree = to_this;
        to_this = d;
        b->right_tree = DL;
        left_tree = DR;
        
        update_height(); //update the height of the tree after balancing
    }
}

template <typename Type>
bool Search_tree<Type>::Node::insert( Type const &obj, Search_tree<Type>::Node *&to_this ) {
	if ( obj < node_value ) {
		if ( left_tree == nullptr ) {
			left_tree = new Search_tree<Type>::Node( obj );
            update_height();
            
            left_tree->next_node = to_this; // if object is inserted in the left tree reassign the order of the nodes within the left tree in order to miantain linear order
            left_tree->previous_node = to_this->previous_node;
            left_tree->previous_node->next_node = left_tree;
            left_tree->next_node->previous_node = left_tree;
          
            
			return true;
		} else {
			if ( left_tree->insert( obj, left_tree ) ) {
				update_height();
                check_fix_tree(to_this); //when object is placed in left tree, check to make sure balance is maintained, if not, balance the tree
				return true;
			} else {
				return false;
			}
		}
	} else if ( obj > node_value ) {
		if ( right_tree == nullptr ) {
			right_tree = new Search_tree<Type>::Node( obj );
			update_height();
            
            right_tree->next_node = to_this->next_node; // if object is inserted in the right tree reassign the order of the nodes within the right tree in order to maintain linear order
            right_tree->previous_node = to_this;
            right_tree->previous_node->next_node = right_tree;
            right_tree->next_node->previous_node = right_tree;
            
			return true;
		} else {
			if ( right_tree->insert( obj, right_tree ) ) {
				update_height();
                check_fix_tree(to_this); //when object is placed in right tree, check to make sure balance is maintained, if not, balance the tree
				return true;
			} else {
				return false;
			}
		}
	} else {
		return false;
	}
}

template <typename Type>
bool Search_tree<Type>::Node::erase( Type const &obj, Search_tree<Type>::Node *&to_this ) {
	if ( obj < node_value ) {
		if ( left_tree == nullptr ) {
            return false;
		} else {
			if ( left_tree->erase( obj, left_tree ) ) {
                update_height();
                if(left_tree != nullptr && (right_tree->height() - left_tree->height() > 1)){ //if the left tree is not empty after the erase, and right tree is bigger than left tree by more than 1 in hieght, need to balance right tree
                    balance_right_tree(to_this);
                }
				return true;
			}

			return false;
		}
	} else if ( obj > node_value ) {
		if ( right_tree == nullptr ) {
			return false;
		} else {
			if ( right_tree->erase( obj, right_tree ) ) {
                update_height();
                if(right_tree != nullptr && (left_tree->height() - right_tree->height() > 1)){ //if the right tree is not empty after the erase, and left tree is bigger than right tree by more than 1 in hieght, need to balance left tree
                    balance_left_tree(to_this);
                }
				return true;
			}

			return false;
		}
	} else {
		assert( obj == node_value );

		if ( is_leaf() ) {
			to_this = nullptr;
			delete this;
            
            next_node->previous_node = previous_node; //Reassign nodes to maintain linear order when a node is deleted(done for all cases)
            previous_node->next_node = next_node;
		} else if ( left_tree == nullptr ) {
			to_this = right_tree;
			delete this;
            
            next_node->previous_node = previous_node;
            previous_node->next_node = next_node;
		} else if ( right_tree == nullptr ) {
			to_this = left_tree;
			delete this;
            
            next_node->previous_node = previous_node;
            previous_node->next_node = next_node;
		} else {
			node_value = right_tree->front()->node_value;
			right_tree->erase( node_value, right_tree );
			update_height();
		}
		return true;
	}
}

//////////////////////////////////////////////////////////////////////
//                   Iterator Private Constructor                   //
//////////////////////////////////////////////////////////////////////

template <typename Type>
Search_tree<Type>::Iterator::Iterator( Search_tree<Type> *tree, typename Search_tree<Type>::Node *starting_node ):
containing_tree( tree ),
current_node( starting_node ) {
	// This is done for you...
	// Does nothing...
}

//////////////////////////////////////////////////////////////////////
//                 Iterator Public Member Functions                 //
//////////////////////////////////////////////////////////////////////

template <typename Type>
Type Search_tree<Type>::Iterator::operator*() const {
	// This is done for you...
	return current_node->node_value;
}

template <typename Type>
typename Search_tree<Type>::Iterator &Search_tree<Type>::Iterator::operator++() {
	// Update the current node to the node containing the next higher value
	// If we are already at end do nothing
	// Your implementation here, do not change the return value
    auto itr = containing_tree->end(); //load the tree end value
    if(*this != itr){ // compare current iterator value to end value, as there can be no duplicates this will check if iterator is at the end node
       current_node = current_node->next_node; //move the current node to the next node, works as data is kept in linear order
    }
    return *this;
}

template <typename Type>
typename Search_tree<Type>::Iterator &Search_tree<Type>::Iterator::operator--() {
	// Update the current node to the node containing the next smaller value
	// If we are already at either rend, do nothing
	// Your implementation here, do not change the return value
    auto itr = containing_tree->rend(); //load the tree rend value
    if (*this != itr){ // compare current iterator value to rend value, as there can be no duplicates this will check if iterator is at the rend node
        current_node = current_node->previous_node; //move the current node to the previous node, works as data is kept in linear order
    }
    return *this;
}

template <typename Type>
bool Search_tree<Type>::Iterator::operator==( typename Search_tree<Type>::Iterator const &rhs ) const {
	// This is done for you...
	return ( current_node == rhs.current_node );
}

template <typename Type>
bool Search_tree<Type>::Iterator::operator!=( typename Search_tree<Type>::Iterator const &rhs ) const {
	// This is done for you...
	return ( current_node != rhs.current_node );
}

//////////////////////////////////////////////////////////////////////
//                            Friends                               //
//////////////////////////////////////////////////////////////////////

// You can modify this function however you want:  it will not be tested

template <typename T>
std::ostream &operator<<( std::ostream &out, Search_tree<T> const &list ) {
	out << "not yet implemented";

	return out;
}

#endif
