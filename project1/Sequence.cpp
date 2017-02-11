#include "Sequence.h"

Sequence::Sequence() : m_size(0) {
	m_head = new Node;
	m_head->m_next = m_head;
	m_head->m_prev = m_head;
}
// Creates an empty Sequence which has size 0
// Also creates a dummy head node for the linked list
// that will contain the values in the sequence

Sequence::Sequence(const Sequence& other) : m_size(0) {
	m_head = new Node;
	m_head->m_next = m_head;
	m_head->m_prev = m_head;
	int pos = 0;

	Node* curNodeOther = other.m_head->m_next;
	while (curNodeOther != other.m_head) {
		insert(pos, curNodeOther->m_data);
		curNodeOther = curNodeOther->m_next;
		pos++;
	}

}
// creates a Sequence that's a valid copy of the other Sequence

Sequence::~Sequence() {
	Node* curNode = m_head->m_next;
	Node* tempNext = curNode->m_next;

	while (curNode != m_head) {
		tempNext = curNode->m_next;
		delete curNode;
		curNode = tempNext;
	}
	
	delete m_head;
}
// destroys the sequence
// deletes all the value-holding nodes first and then deletes mhead

Sequence& Sequence::operator=(const Sequence& rhs) {
	if (this != &rhs)
	{
		Sequence temp(rhs);
		swap(temp);
	}
	return *this;
}
// assignment operator for this class

bool Sequence::empty() const {
	return size() == 0 ? true : false;
}
// Return true if the sequence is empty, otherwise false.

int Sequence::size() const
{
	return m_size;
}
// Return the number of items in the sequence.

bool Sequence::insert(int pos, const ItemType& value) 
{
	if (pos<0 || pos>size())
		return false;

	// for loop was hard to comment in a readable manner
	Node* curNode = m_head->m_next;		// curNode will be used to traverse the LL
	int curPos = 0;						// current "index" when traversing LL
								
	// GENERAL CASE:
	// When pos < size 
	while (curNode != m_head) {

		// If the curNode is the node at pos
		// insert new node between curNode and previous Node
		if (curPos == pos) {
			Node* newNode = new Node;
			newNode->m_data = value;
			newNode->m_prev = curNode->m_prev;
			newNode->m_next = curNode;
			curNode->m_prev->m_next = newNode;
			curNode->m_prev = newNode;
			m_size++;

			return true;
		}
		curPos++;
		curNode = curNode->m_next;
	}

	// program reaches this is pos == size
	// If program reaches this, then curNode == m_head.
	// insert newNode before this head node (aka end of list)

	Node* newNode = new Node;
	newNode->m_data = value;
	newNode->m_prev = curNode->m_prev;
	newNode->m_next = curNode;
	curNode->m_prev->m_next = newNode;
	curNode->m_prev = newNode;
	m_size++;

	return true;
}
// Insert value into the sequence so that it becomes the item at
// position pos.  The original item at position pos and those that
// follow it end up at positions one higher than they were at before.
// Return true if 0 <= pos <= size() and the value could be
// inserted.  (It might not be, if the sequence has a fixed capacity,
// (e.g., because it's implemented using a fixed-size array) and is
// full.)  Otherwise, leave the sequence unchanged and return false.
// Notice that if pos is equal to size(), the value is inserted at the
// end.


int Sequence::insert(const ItemType& value) {

	Node* curNode = m_head->m_next;		// curNode will be used to traverse the LL
	int curPos = 0;						// current "index" when traversing LL
		
	// GENERAL CASE:
	// When pos < size 
	while (curNode != m_head) {

		// If the curNode has m_data > value
		// insert new node between curNode and previous Node
		if (value <= curNode->m_data) {
			Node* newNode = new Node;
			newNode->m_data = value;
			newNode->m_prev = curNode->m_prev;
			newNode->m_next = curNode;
			curNode->m_prev->m_next = newNode;
			curNode->m_prev = newNode;
			m_size++;

			return curPos;
		}
		curPos++;
		curNode = curNode->m_next;
	}
	// program reaches this is pos == size
	// If program reaches this, then curNode == m_head.
	// insert newNode before this head node (aka end of list)

	Node* newNode = new Node;
	newNode->m_data = value;
	newNode->m_prev = curNode->m_prev;
	newNode->m_next = curNode;
	curNode->m_prev->m_next = newNode;
	curNode->m_prev = newNode;
	m_size++;

	return curPos;

}
// Let p be the smallest integer such that value <= the item at
// position p in the sequence; if no such item exists (i.e.,
// value > all items in the sequence), let p be size().  Insert
// value into the sequence so that it becomes the item at position
// p.  The original item at position p and those that follow it end
// up at positions one higher than before.  Return p if the value
// was actually inserted. 


bool Sequence::erase(int pos) {
	if (pos < 0 || pos >= size())
		return false;
	
	Node* curNode = m_head->m_next;		// curNode will be used to traverse the LL
	int curPos = 0;						// current "index" when traversing LL
 
	// curNode will be the pointer to the node that's going to be deleted
	while (curNode != m_head) {

		if (curPos == pos) {
			curNode->m_prev->m_next = curNode->m_next;
			curNode->m_next->m_prev = curNode->m_prev;
			delete curNode;
			m_size--;

			return true;
		}
		curPos++;
		curNode = curNode->m_next;
	}

	// program should not reach this ever if curNode == m_head at the beginning, size is 0
	// so this function would've returned false in the 2nd line
	return false; // put this here just to alert user if somehow program gets here
}
// If 0 <= pos < size(), remove the item at position pos from
// the sequence (so that all items that followed this item end up at
// positions one lower than they were at before), and return true.
// Otherwise, leave the sequence unchanged and return false.

int Sequence::remove(const ItemType& value) {
	int numRemoved = 0;
	Node* curNode = m_head->m_next;		// curNode will be used to traverse the LL

	while (curNode != m_head) {

		if (curNode->m_data == value) {
			Node* deleteThis = curNode;	// pass the node that will be deleted to this variable
										// this is needed because if we use curNode for deleting
										// Nodes, we can no longer traverse the LL

			curNode = curNode->m_prev;

			deleteThis->m_prev->m_next = deleteThis->m_next;
			deleteThis->m_next->m_prev = deleteThis->m_prev;

			delete deleteThis;

			numRemoved++;
			m_size--;
		}
		curNode = curNode->m_next;
	}

	return numRemoved;
}
// Erase all items from the sequence that == value.  Return the
// number of items removed (which will be 0 if no item == value).


bool Sequence::get(int pos, ItemType& value) const {
	if (pos < 0 || pos >= size())
		return false;

	Node* curNode = m_head->m_next;		// Node pointer starts at first real Node
	int curPos = 0;						// which has index 0!

	for (curPos = 0; curPos < size(); curPos++) {
		if (curPos == pos) {
			value = curNode->m_data;
			return true;
		}
		curNode = curNode->m_next;
	}
}
// If 0 <= pos < size(), copy into value the item at position pos
// in the sequence and return true.  Otherwise, leave value unchanged
// and return false.

bool Sequence::set(int pos, const ItemType& value) {
	if (pos < 0 || pos >= size())
		return false;

	Node* curNode = m_head->m_next;		// Node pointer starts at first real Node
	int curPos = 0;						// which has index 0!

	for (curPos = 0; curPos < size(); curPos++) {
		if (curPos == pos) {
			curNode->m_data = value;
			return true;
		}
		curNode = curNode->m_next;
	}
}
// If 0 <= pos < size(), replace the item at position pos in the
// sequence with value and return true.  Otherwise, leave the sequence
// unchanged and return false.

int Sequence::find(const ItemType& value) const {
	Node* curNode = m_head->m_next;		// Node pointer starts at first real Node
	int curPos = 0;						// which has index 0!

	for (curPos = 0; curPos < size(); curPos++) {
		if (curNode->m_data == value) {
			return curPos;
		}
		curNode = curNode->m_next;
	}

	// program will only reach this point if value was NOT found
	return -1;

}
// Let p be the smallest integer such that value == the item at
// position p in the sequence; if no such item exists, let p be -1.
// Return p.


void Sequence::swap(Sequence& other) {
	Node* tempHead = other.m_head;
	int tempSize = other.m_size;
	other.m_head = m_head;
	other.m_size = m_size;
	m_size = tempSize;
	m_head = tempHead;
}
// Exchange the contents of this sequence with the other one.
// swaps head pointers and size member variables

/*
class Sequence
{
	public:
		Sequence();
		// creates an empty Sequence

		Sequence::Sequence(const Sequence& other);
		// creates a Sequence that's a valid copy of the other Sequence

		~Sequence();
		// destroys the sequence

		Sequence& Sequence::operator=(const Sequence& rhs);
		// assignment operator for this class



		

	private:
			// Invariant:
			// m_size < 0, m_size > DEFAULT_MAX_ITEMS

		// Nodes for linked list
		struct Node
		{
			ItemType m_data;
			Node* m_next;
			Node* m_prev;
		};
		
		Node* head; // head of linked list				
		int m_size;	// current size of sequence
};
*/