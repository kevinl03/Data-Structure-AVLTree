#pragma once
#include "stdio.h"
#include <vector>
#include <iostream>

using std::max;
using std::runtime_error;
using std::abs;
using std::cout;
using std::endl;
using std::vector;

// AVL Node Class
// 
template <class K, class V>
class AVLTreeNode {
public:
	// Should have attributes named:
	 //-pointer to parent
	AVLTreeNode* parent;
	// left - pointer to left child
	AVLTreeNode* left;
	// right - pointer to right child
	AVLTreeNode* right;
	// key - node's key
	K key;
	// value - node's value
	V val;
	// height - node's height
	int height;



	AVLTreeNode(K k, V v);

	// Constructors ...
	AVLTreeNode<K, V>* insert(K k, V v);

	//return val
	V search(K k) const;

	//return key predessor
	AVLTreeNode<K, V>* predessor();

	AVLTreeNode<K, V>* remove();

	void updateheight();

	bool isLeftHeavy() const;

	bool isRightHeavy() const;

	void RotateRight();

	void RotateLeft();

	void rebalance();

	
};


template <class K, class V>
AVLTreeNode<K,V>::AVLTreeNode(K k, V v) {
	key = k;
	val = v;
	parent = NULL;
	left = NULL;
	right = NULL;
	height = 0;

}



template <class K, class V>
AVLTreeNode<K, V>* AVLTreeNode<K, V>::insert(K k, V v) {
	//acts as base case since if this case doesn't happen, you can
	// always find somewhere to insert
	//check duplicate key
	if (this->key == k) {
		return NULL;
	}
	if (k < key) {
		if (left == NULL) {
			//position found
			AVLTreeNode<K, V>* newNode = new AVLTreeNode<K, V>(k, v);
			this->left = newNode;
			newNode->parent = this;
			return newNode;
		}
		 else {
			return left->insert(k, v);
		 }
		
	}
	else {
		if (right == NULL) {
			//position found
			AVLTreeNode<K, V>* newNode = new AVLTreeNode<K, V>(k, v);
			this->right = newNode;
			newNode->parent = this;
			return newNode;
		}
		else {
			return right->insert(k, v);
		}
	}
		
}


template <class K, class V>
V AVLTreeNode<K,V>::search(K k) const {
	if (this == NULL) {
		throw std::runtime_error("not found");
	}
	else {
		if (k == key) {
			return val;
		}
		else if (k < key) {
			return left->search(k);
		}
		else {
			return right->search(k);
		}
	}
}

//returns the node with the key that is the predessor of the called object
template <class K, class V>
AVLTreeNode<K, V>* AVLTreeNode<K, V>::predessor(){
	AVLTreeNode<K, V>* cur = this;
	cur = cur->left;
	while (cur->right != NULL) {
		cur = cur->right;
	}
	return cur;

}


//manages parent and children links, and returns the necessary node to be deleted
//also checks for root guards, but does not manage root pointers since thats private member variable
template <class K, class V>
AVLTreeNode<K, V>* AVLTreeNode<K, V>::remove() {
	
	//leaf node

	if (this->left == NULL && this->right == NULL) {
		//fix parent links unless its root who has no parent
		if (parent != NULL) {
			if (key < parent->key) {
				this->parent->left = NULL;
			}
			else {
				this->parent->right = NULL;
			}
		}
		return this;
	}

	//ALSO CHECK IN EACH OF THESE CASE IF IT IS ROOT NOTE AKA: O
	//													        \
	//															 O
	//															  \


	//  only right child \           /
	//                    O   or     O
	//                     \          \ 


	else if (this->left == NULL ) {
		
		//fix parent links 
		if (this->parent != NULL) {

			this->right->parent = this->parent;
			if (key < parent->key) {
				this->parent->left = this->right;
			}
			else {
				this->parent->right = this->right;
			}
		}
		//if the node is a root
		else {
			this->right->parent = NULL;
		}
		return this;
	}
	//only one child      /   \
	//                   O     O
    //                  /     /

	else if (this->right == NULL) {
		if (this->parent != NULL) {
			
			this->left->parent = this->parent;
			if (key < parent->key) {
				this->parent->left = this->left;
			}
			else {
				this->parent->right = this->left;
			}
		}
		//if the node is a root
		else {
			this->left->parent = NULL;
		}
		return this;
	}
	//two children nodes /\
	//we don't actually change links with this node, only key and values
	//get changed
	//but it has to figure out which node must be removed
	else {
		AVLTreeNode<K, V>* pred = this->predessor();

		//remove the predessesor which will be one either a leaf node or a node with only a left child
		AVLTreeNode<K, V>* returnNode = pred->remove();

		//change values and keys of current node so that the previous recursive call
		//is not effected by the value changes of current node during linkage in the case
		// that the predessesor is directy on the left of the 2 children node

		this->key = pred->key;
		this->val = pred->val;
		return returnNode;
	}
	

}

//make cleaner code
template <class K, class V>
int getHeight(AVLTreeNode<K, V>* node) {
	if (node == NULL) {
		return -1;
	}
	return node->height;
}

//update the height
template <class K, class V>
void AVLTreeNode<K, V>::updateheight() {
	this->height = max(getHeight(this->left), getHeight(this->right)) + 1;
}

//return the first node that is unbalanced (and is the only one that needs to be changed)
template <class K, class V>
AVLTreeNode<K, V>* updateAncestryHeights(AVLTreeNode<K, V>* lowestAncestor) {
	AVLTreeNode<K, V>* cur = lowestAncestor;
	while (cur != NULL) {
		cur->updateheight();
		//will determine the type of misbalance in the AVLTree class

		int balance = abs(getHeight(cur->left) - getHeight(cur->right));

		if (balance > 1) {
			return cur;
		}
		
		cur = cur->parent;
	}
	//this means all nodes in ancestry are balanced and we reached the root parent
	return NULL;
}

template <class K, class V>
bool AVLTreeNode<K, V>::isLeftHeavy() const {
	return getHeight(this->left) > getHeight(this->right);
}

template <class K, class V>
bool AVLTreeNode<K, V>::isRightHeavy() const {
	return getHeight(this->right) > getHeight(this->left);
}

template <class K, class V>
void AVLTreeNode<K, V>::RotateLeft() {
	AVLTreeNode<K, V>* tmpparent = parent;

	if (tmpparent != NULL) {
		if (right->key < tmpparent->key) {
			tmpparent->left = right;
		}
		else {
			tmpparent->right = right;
		}
	}
	//might be null might not be, but it doesnt matter
	right->parent = tmpparent;


	parent = right;

	AVLTreeNode<K, V>* tmpleft = right->left;

	right->left = this;
	this->right = tmpleft;
	if (tmpleft != NULL) {
		tmpleft->parent = this;
	}
	this->updateheight();
	this->parent->updateheight();

}


template <class K, class V>
void AVLTreeNode<K, V>::RotateRight() {
	AVLTreeNode<K, V>* tmpparent = parent;

	if (tmpparent != NULL) {
		if (left->key < tmpparent->key) {
			tmpparent->left = left;
		}
		else {
			tmpparent->right = left;
		}
	}
	//might be null might not be, but it doesnt matter
	left->parent = tmpparent;


	parent = left;

	AVLTreeNode<K, V>* tmpright = left->right;

	left->right = this;
	this->left = tmpright;
	if (tmpright != NULL) {
		tmpright->parent = this;
	}
	this->updateheight();
	this->parent->updateheight();
}

template <class K, class V>
void AVLTreeNode<K, V>::rebalance() {
	//determine which rotation to use
	if (this->isLeftHeavy()) {

		/*	    O
			   /
			  O
			 /
			O              */

		if (this->left->isLeftHeavy()) {
			this->RotateRight();
			return;
		}

		/*	    O
			   /
			  O
			   \
				O              */
		else  {

			//check to update root

			//LR rotation
			this->left->RotateLeft();
			this->RotateRight();
			return;
		}
	}

	else if (this->isRightHeavy()) {

		/*	       O
					\
					 O
					  \
					   O       */

		if (this->right->isRightHeavy()) {

			//check to update root

			this->RotateLeft();
			return;
		}

		/*	    O
				 \
				  O
				 /
				O              */
		else {

			//check to update root

			this->right->RotateRight();
			this->RotateLeft();
			return;
		}
	}
}


// AVL Tree class
//template <class K, class>
template <class K, class V>
class AVLTree
{
private:
	// Tree attributes go here
	unsigned int Tsize;
	// Your tree MUST have a root node named root
	AVLTreeNode<K, V>* root;

public:
	void* getRoot() const { return root; }; // DO NOT REMOVE

	AVLTreeNode<K, V>* getRootNode() const { return root; };

	AVLTree(); //default constructor

	AVLTree(AVLTree<K, V>& orig);

	~AVLTree();

	AVLTree<K, V>& operator=(AVLTree<K, V>& rhs);
	
	bool insert(K key, V val); //insert elem

	V search(K key);

	bool remove(K key);
	
	unsigned int size() const { return Tsize; };

	vector<K> keys();

	vector<V> values();


};


// AVL Tree Methods go here

//template <class K, class V>

template <class K, class V>
AVLTree<K, V>::AVLTree() {
	root = NULL;
	Tsize = 0;
}



template <class K, class V>
bool AVLTree<K, V>::insert(K key, V val) {
	//managing root
	if (root == NULL) {
		root = new AVLTreeNode<K, V>(key, val);
		//set parent n shit
		Tsize++;
		return true;
	}
	else {
		
		AVLTreeNode<K, V>* inserted = root->insert(key, val);
		//that means a key already existed
		if (inserted == NULL) {
		
			return false;
		}
		//new node inserted
		else {
			Tsize++;
			AVLTreeNode<K, V>* unbalancedNode = updateAncestryHeights(inserted);


			//an unbalanced Node was found
			if (unbalancedNode != NULL) {


				unbalancedNode->rebalance();
				if (unbalancedNode->parent->parent == NULL) {
					root = unbalancedNode->parent;
				}

				
			}
			return true;
		}
		
	}
	
	
}

template <class K, class V>
V AVLTree<K, V>::search(K key) {
	return root->search(key);
}

//manages which nodes to delete, handling root pointers, size decrementing, and checking if key exists
template <class K, class V>
bool AVLTree<K, V>::remove(K key) {

	//no elements
	if (root == NULL) {
		return false;
	}
	else {
		//find the key
		AVLTreeNode<K, V>* cur = root;
		while (cur != NULL && cur->key != key) {
			

			if (key < cur->key) {
				cur = cur->left;
			}
			else {
				cur = cur->right;
			}
		}
		if (cur == NULL) {
			return false;
		}
		

		//singleton root node in a tree
		if (cur == root && cur->left == NULL && cur->right ==NULL) {
			root = NULL;
			//moved here because this function actually changes the links of the root
			//so we must compare the roots first
			AVLTreeNode<K, V>* removeNode = cur->remove();
			delete removeNode;
			Tsize--;
			return true;
		}
		//root node with one child  O
		//						   /

		else if (cur == root && cur->left != NULL && cur->right == NULL) {
			root = cur->left;
			AVLTreeNode<K, V>* removeNode = cur->remove();
			delete removeNode;
			Tsize--;
			return true;
			
		}
		//root node with one child  O
		//						     \

		else if (cur == root && cur->left == NULL && cur->right != NULL) {
			root = cur->right;	
			AVLTreeNode<K, V>* removeNode = cur->remove();
			delete removeNode;
			Tsize--;
			return true;
		}

		// two children for root O
		//                      / \

		//root pointers stays the same, only the value/key changes
		
		else {
			AVLTreeNode<K, V>* ancestor = cur->parent;


			//when the removed cur has two children, we need to start
			//updating ancestry heights from the predessor parent, not from cur parent
			if (cur->left != NULL && cur->right != NULL) {
				ancestor = cur->predessor()->parent;
			}


			AVLTreeNode<K, V>* toRemove = cur->remove();

			AVLTreeNode<K, V>* unbalancedNode = updateAncestryHeights(ancestor);

			if (unbalancedNode != NULL) {


				unbalancedNode->rebalance();

				if (unbalancedNode->parent->parent == NULL) {
					root = unbalancedNode->parent;
				}
			}

			delete toRemove;
			Tsize--;
			return true;
		}
	}	
	
}


//helper function
template <class K, class V>
void inordervecKeys(AVLTreeNode<K, V>* node, vector<K>& vec) {
	//
	if (node->left != NULL) {
		inordervecKeys(node->left, vec);
	}
	vec.push_back(node->key);

	if (node->right != NULL) {
		inordervecKeys(node->right, vec);
	}
}

template <class K, class V>
vector<K> AVLTree<K, V>::keys() {
	vector<K> returnvec;
	if (root == NULL) {
		return returnvec;
	}
	inordervecKeys(root, returnvec);
	return returnvec;
}

//helper function
template <class K, class V>
void inordervecValues(AVLTreeNode<K, V>* node, vector<V>& vec) {
	if (node->left != NULL) {
		inordervecValues(node->left, vec);
	}
	vec.push_back(node->val);

	if (node->right != NULL) {
		inordervecValues(node->right, vec);
	}
}

template <class K, class V>
vector<V> AVLTree<K, V>::values() {
	vector<V> returnvec;
	if (root == NULL) {
		return returnvec;
	}
	inordervecValues(root, returnvec);
	return returnvec;
}

template <class K, class V>
void inordercopy(AVLTreeNode<K, V>* nodeparent, AVLTreeNode<K, V>* newnodeparent) {
	if (nodeparent->left != NULL) {
		AVLTreeNode<K, V>* newleft = new AVLTreeNode<K, V>(nodeparent->left->key, nodeparent->left->val);
		newleft->parent = newnodeparent;
		newleft->parent->left = newleft;
		newleft->height = nodeparent->left->height;
		inordercopy(nodeparent->left, newleft);
	}
	if (nodeparent->right != NULL) {
		AVLTreeNode<K, V>* newright = new AVLTreeNode<K, V>(nodeparent->right->key, nodeparent->right->val);
		newright->parent = newnodeparent;
		newright->parent->right = newright;
		newright->height = nodeparent->right->height;
		inordercopy(nodeparent->right,newright);
	}
}

template <class K, class V>
AVLTree<K, V>::AVLTree(AVLTree<K, V>& orig) {
	if (orig.root != NULL) {
		AVLTreeNode<K, V>* newroot = new AVLTreeNode<K, V>(orig.root->key, orig.root->val);
		root = newroot;
		Tsize = orig.Tsize;
		inordercopy(orig.root, newroot);
	}
	else {
		root = NULL;
		Tsize = 0;
	}
	
}

template <class K, class V>
void destructorHelp(AVLTreeNode<K, V>* node) {
	if (node == NULL) {
		return;
	}
	destructorHelp(node->left);
	destructorHelp(node->right);
	node = NULL;
	delete node;


}

template <class K, class V>
AVLTree<K, V>::~AVLTree() {
	if (root != NULL) {
		destructorHelp(root);
	}
	root = NULL;
	Tsize = NULL;

}



template <class K, class V>
AVLTree<K, V>& AVLTree<K,V>::operator=(AVLTree<K, V>& rhs) {
	if (this == &rhs) {
		return rhs;
	}
	else {
		//deallocate all the nodes
		destructorHelp(this->root);
		//empty tree
		if (rhs.root == NULL) {
			root = NULL;
			Tsize = 0;
			return *this;
			
			
		}

		else {
			AVLTreeNode<K, V>* newroot = new AVLTreeNode<K, V>(rhs.root->key, rhs.root->val);
			root = newroot;
			Tsize = rhs.Tsize;
			inordercopy(rhs.root, newroot);
			return *this;
		}

	}
}
	