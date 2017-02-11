#ifndef SEQUENCE_H
#define SEQUENCE_H
#include <string>

typedef unsigned long ItemType;

class Sequence
{
public:
	Sequence();
	// creates an empty Sequence
	// also creates a dummy Head node

	Sequence(const Sequence& other);
	// creates a Sequence that's a valid copy of the other Sequence
	// creates a different copy of other's linked list 
	// instead of just copying the m_head pointer

	~Sequence();
	// destroys the sequence, frees all the Nodes

	Sequence& operator=(const Sequence& rhs);
	// assignment operator for this class

	bool empty() const;
	// Return true if the sequence is empty, otherwise false.

	int size() const;
	// Return the number of items in the sequence.

	bool insert(int pos, const ItemType& value);
	// Insert value into the sequence so that it becomes the item at
	// position pos.  The original item at position pos and those that
	// follow it end up at positions one higher than they were at before.
	// Return true if 0 <= pos <= size() and the value could be
	// inserted.  (It might not be, if the sequence has a fixed capacity,
	// (e.g., because it's implemented using a fixed-size array) and is
	// full.)  Otherwise, leave the sequence unchanged and return false.
	// Notice that if pos is equal to size(), the value is inserted at the
	// end.

	int insert(const ItemType& value);
	// Let p be the smallest integer such that value <= the item at
	// position p in the sequence; if no such item exists (i.e.,
	// value > all items in the sequence), let p be size().  Insert
	// value into the sequence so that it becomes the item at position
	// p.  The original item at position p and those that follow it end
	// up at positions one higher than before.  Return p if the value
	// was actually inserted.  

	bool erase(int pos);
	// If 0 <= pos < size(), remove the item at position pos from
	// the sequence (so that all items that followed this item end up at
	// positions one lower than they were at before), and return true.
	// Otherwise, leave the sequence unchanged and return false.

	int remove(const ItemType& value);
	// Erase all items from the sequence that == value.  Return the
	// number of items removed (which will be 0 if no item == value).

	bool get(int pos, ItemType& value) const;
	// If 0 <= pos < size(), copy into value the item at position pos
	// in the sequence and return true.  Otherwise, leave value unchanged
	// and return false.

	bool set(int pos, const ItemType& value);
	// If 0 <= pos < size(), replace the item at position pos in the
	// sequence with value and return true.  Otherwise, leave the sequence
	// unchanged and return false.

	int find(const ItemType& value) const;
	// Let p be the smallest integer such that value == the item at
	// position p in the sequence; if no such item exists, let p be -1.
	// Return p.

	void swap(Sequence& other);
	// Exchange the contents of this sequence with the other one.

private:
	// Invariant:
	// m_size < 0, m_size > DEFAULT_MAX_ITEMS

	// Nodes for doubly linked linked list
	struct Node
	{
		ItemType m_data;
		Node* m_next;
		Node* m_prev;
	};

	Node* m_head; // head of linked list
	int m_size;	// current size of sequence
};

inline
int subsequence(const Sequence& seq1, const Sequence& seq2) {
	int seq2Size = seq2.size();
	int seq1Size = seq1.size();
	ItemType curSeq1Item;
	ItemType curSeq2Item;
	int i = 0;
	int j = 0;

	for (i = 0; i < seq1Size; i++) {
		seq1.get(i, curSeq1Item);
		seq2.get(j, curSeq2Item);

		if (curSeq1Item == curSeq2Item) {
			j++;
			if (j == seq2Size)
				return i - j + 1;
		}
		else
			j = 0;

	}
	return -1;
}
// returns the earliest index/place in seq1 which represents the
// start of a subsequence that's equal to seq2

inline
void interleave(const Sequence& seq1, const Sequence& seq2, Sequence& result) {
	Sequence tempRes;

	int curPos2 = 0;
	int curPos1 = 0;
	int curPosTempRes = 0;

	for (; curPos1 < seq1.size() || curPos2 < seq2.size(); curPos1++, curPos2++) {
		if (curPos1 < seq1.size()) {
			ItemType seq1Val;
			seq1.get(curPos1, seq1Val);
			tempRes.insert(curPosTempRes, seq1Val);
			curPosTempRes++;
		}
		if (curPos2 < seq2.size()) {
			ItemType seq2Val;
			seq2.get(curPos2, seq2Val);
			tempRes.insert(curPosTempRes, seq2Val);
			curPosTempRes++;
		}
	}

	result = tempRes;
}
#endif