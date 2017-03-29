#ifndef MYMAP_H
#define MYMAP_H
// MyMap.h

#include <iostream>
using namespace std;

// Skeleton for the MyMap class template.  You must implement the first six
// member functions.

template<typename KeyType, typename ValueType>
class MyMap
{
public:
	MyMap();		//	constructor
	~MyMap();		//	destructor;	deletes	all	of	the	tree's	nodes
	void clear();	//	deletes	all	of	the	trees	nodes	producing an	empty	tree
	int size() const;	//	return	the	number	of	associations	in	the	map

						//	The	associate	method	associates	one	item	(key)	with	another	(value).
						//	If	no	association	currently	exists	with	that	key,	this	method	inserts
						//	a	new	association	into	the	tree	with	that	key/value	pair.		If	there	is
						//	already	an	association	with	that	key	in	the	tree,	then	the	item
						//	associated	with	that	key	is	replaced	by	the	second	parameter	(value).
						//	Thus,	the	tree	contains	no	duplicate	keys.
	void associate(const KeyType& key, const ValueType& value);

	// for a map that can't be modified, return a pointer to const ValueType
	const ValueType* find(const KeyType& key) const;

	// for a modifiable map, return a pointer to modifiable ValueType
	ValueType* find(const KeyType& key)
	{
		return const_cast<ValueType*>(const_cast<const MyMap*>(this)->find(key));
	}

	// C++11 syntax for preventing copying and assignment
	MyMap(const MyMap&) = delete;
	MyMap& operator=(const MyMap&) = delete;

private:
	// Representation:
	//		A Binary search tree with a head pointer
	//		each node in the binary search tree can have a unique key and a value
	//		if m_size = 0, m_head points to nullptr
	//		if m_size > 0, m_head points to some Node

	struct Node {
		KeyType key;
		ValueType value;
		Node* left;
		Node* right;
	};

	// points to head of BST representing the Map
	Node* m_head;

	// number of Nodes in BST aka number of associations in this map
	int m_size;

	// Deletes the passed node's left subtree, then its right subtree, then deletes that node
	void deleteThisNode(Node* deleteMe);
};


// Public member function implementations

template<typename KeyType, typename ValueType>
MyMap<KeyType, ValueType>::MyMap()
	: m_head(nullptr), m_size(0)
{
}


//	destructor;	deletes	all	of	the	tree's	nodes
template<typename KeyType, typename ValueType>
MyMap<KeyType, ValueType>::~MyMap() {
	clear();
}

//	deletes	all	of	the	trees	nodes	producing an	empty	tree
template<typename KeyType, typename ValueType>
void MyMap<KeyType, ValueType>::clear() {
	// Delete nodes in post order
	// left child -> right child -> current node
	Node* cur = m_head;

	deleteThisNode(cur);
}

template<typename KeyType, typename ValueType>
void MyMap<KeyType, ValueType>::deleteThisNode(Node* deleteMe) {
	if (deleteMe == nullptr)
		return;
	deleteThisNode(deleteMe->left);
	deleteThisNode(deleteMe->right);
	delete deleteMe;
}

//	return	the	number	of	associations	in	the	map
template<typename KeyType, typename ValueType>
int MyMap<KeyType, ValueType>::size() const {
	return m_size;
}



//	The	associate	method	associates	one	item	(key)	with	another	(value).
//	If	no	association	currently	exists	with	that	key,	this	method	inserts
//	a	new	association	into	the	tree	with	that	key/value	pair.		If	there	is
//	already	an	association	with	that	key	in	the	tree,	then	the	item
//	associated	with	that	key	is	replaced	by	the	second	parameter	(value).
//	Thus,	the	tree	contains	no	duplicate	keys.
template<typename KeyType, typename ValueType>
void MyMap<KeyType, ValueType>::associate(const KeyType& key, const ValueType& value) {

	// If tree is empty, set new node as head
	if (m_size == 0) {
		m_head = new Node;
		m_head->key = key;
		m_head->value = value;
		m_head->left = nullptr;
		m_head->right = nullptr;

		// adjust map size
		m_size++;

		return;
	}


	// else, locate a suitable spot in the BST
	// and insert it there to preserve the order of the BST
	Node* parent = nullptr;
	Node* cur = m_head;


	// This block will locate the parent node to which our new Node
	// or association will be appended (as a child)
	// Alternatively, if the key already exists in the map, this block
	// will locate the Node with that key and replace its value accordingly

	while (cur != nullptr) {
		// If current node has a key that matches our query, simply
		// replace the value at this node with the new one
		if (key == cur->key) {
			// cerr << "You entered a duplicate key: " << key << endl;
			cur->value = value;
			return;
		}

		// If current node has a key GREATER than our query key, the
		// new node must be inserted somewhere in the left subtree
		else if (key < cur->key) {
			parent = cur;
			cur = cur->left;
		}

		// If current node has a key LESS than our query ket, the
		// new node must be inserted somewhere in the right subtree 
		else {
			parent = cur;
			cur = cur->right;
		}
	}

	// If we reached a nullptr (as cur), it means that we've found
	// the position in which this New Node must be placed and the
	// parent node of this new node
	if (cur == nullptr) {

		// create the New Node
		Node* newRecord = new Node;
		newRecord->key = key;
		newRecord->value = value;
		newRecord->left = nullptr;
		newRecord->right = nullptr;

		// now add this New Node to as the left or right child
		// of its parent node

		// if this node's key is greater than its parent's it must 
		// be the parent's right child
		if (key > parent->key)
			parent->right = newRecord;
		else
			parent->left = newRecord;

		// adjust map size
		m_size++;

		return;
	}
}


template<typename KeyType, typename ValueType>
const ValueType* MyMap<KeyType, ValueType>::find(const KeyType& key) const {

	// If map is empty, nullptr will be returned
	if (m_head == 0)
		return nullptr;

	Node* cur = m_head;
	int steps = 0;

	while (cur != nullptr) {
		steps++;
		// If we've found the node with the appropriate key, return 
		// pointer to the value!
		if (key == cur->key) {
			//cerr << "found something " << cur->key << " " << cur->value << endl;
			// cerr << steps << " steps taken. Size is " << m_size << "\n";
			return &(cur->value);
		}

		// If key is > than current node's key, navigate to right child
		else if (key > cur->key) {
			//cerr << "went right" << endl;
			cur = cur->right;
		}

		else {
			//cerr << "went left" << endl;
			cur = cur->left;
		}
	}
	// cerr << steps << " steps taken. Size is " << m_size << "\n";

	// if we reached a nullptr, the key can't be found in the map!
	return nullptr;
}
#endif // MYMAP_H