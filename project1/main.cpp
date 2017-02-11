#include <iostream>
#include "Sequence.h"
#include <cassert>

using namespace std;

void destructorTest() {
	Sequence a;
	Sequence b;
	b.insert("1");
	b.insert("2");
	b.insert("3");

	Sequence c;
	c.swap(b);
}

void show(Sequence a) {
	int i;

	ItemType x;
	for (i = 0; i < a.size(); i++) {
		a.get(i, x);
		cout << x << endl;
	}

	cout << endl;

}

int main() {

	Sequence a;

	//** TESTING INSERT **//

	// Test invalid parameters for 2 argument insert
	assert(a.insert(-1, "testString1") == false);
	assert(a.insert(1, "testString1") == false);

	// Inserting when Sequence is empty
	assert(a.insert(0, "testString1"));

	// 1 Argument insert when sequence is empty
	Sequence b;
	assert(b.insert("testString1") == 0);

	// Inserting at the beginning & end
	assert(a.insert(0, "testString0"));
	assert(a.insert(2, "testString2"));

	// Testing insert's ability to add strings in the right order
	assert(a.insert("testString") == 0);
	assert(a.insert("testString5") == 4);
	assert(a.insert("testString4") == 4);
	assert(a.insert("testString3") == 4);

	// LINKED LIST LOOKS LIKE THIS:
	//	testString		
	//	testString0	   
	//	testString1	   
	//	testString2	   
	//	testString3	   
	//	testString4	   
	//	testString5	     

	//** TESTING GET **//
	ItemType tempString;

	// getting from begin, mid, end
	assert(a.get(0, tempString));
	assert(tempString == "testString");
	assert(a.get(4, tempString));
	assert(tempString == "testString3");
	assert(a.get(6, tempString));
	assert(tempString == "testString5");
	
	// Get from invalid parameters
	assert(!a.get(-1, tempString));
	// tempString must still be testString 5
	assert(tempString == "testString5");
	assert(!a.get(7, tempString));
	// tempString must still be testString 5
	assert(tempString == "testString5");



	//** TESTING ERASE **//

	// Removing from beginning and end and then middle
	assert(a.erase(6));
	assert(a.erase(0));
	assert(a.erase(2));

	// Removing last element from Sequence
	assert(b.erase(0));
	assert(b.size() == 0);
	assert(b.empty());


	//** TESTING REMOVE **//

	// Testing removing 1 node from beginning, middle & end
	// Some Setup
	assert(a.insert("testString5"));
	assert(a.insert("testString2"));
	// Actual remove()
	assert(a.remove("testString0") == 1);
	assert(a.size() == 5);
	assert(a.remove("testString3") == 1);
	assert(a.size() == 4);
	assert(a.remove("testString5") == 1);
	assert(a.size() == 3);

	// removing until empty & removing from empty Sequence
	assert(a.remove("testString1") == 1);
	assert(a.size() == 2);
	assert(a.remove("testString2") == 1);
	assert(a.size() == 1);
	assert(a.remove("testString4") == 1);
	assert(a.size() == 0);
	assert(a.remove("testString4") == 0);
	assert(a.size() == 0);

	// removing multiple items
	// Setup:
	assert(a.insert("testString2") == 0);
	assert(a.insert("testString2") == 0);
	assert(a.insert("testString2") == 0);
	assert(a.size() == 3);
	// Removal:
	assert(a.remove("testString2") == 3);
	assert(a.size() == 0);


	//** TESTING SET **//

	// Testing Set
	// On empty sequence:
	assert(a.set(0, "thisShouldn'tWork") == false);
	assert(a.set(-1, "thisShouldn'tWork") == false);
	// setup for non empty set
	assert(a.insert("testString2") == 0);
	assert(a.insert("testString2") == 0);
	assert(a.insert("testString2") == 0);
	// setting begining, mid, and end
	assert(a.set(0, "test0") == true);
	assert(a.set(1, "test1") == true);
	assert(a.set(2, "test2") == true);
	// Invalid parameters on non empty sequence
	assert(a.set(3, "thisShouldn'tWork") == false);
	assert(a.set(-1, "thisShouldn'tWork") == false);
	

	//** TESTING FIND **//

	// testing absent values
	assert(a.find("YouCantFindThis") == -1);
	// finding from beginning, mid, end
	assert(a.find("test0") == 0);
	assert(a.find("test1") == 1);
	assert(a.find("test2") == 2);
	// finding when multiple nodes have matching values
	// setup:
	assert(a.insert("test0") == 0);
	assert(a.insert("test1") == 2);
	assert(a.insert("test2") == 4);
	assert(a.find("test0") == 0);
	assert(a.find("test1") == 2);
	assert(a.find("test2") == 4);


	//** TESTING SWAP **//

	// setup:
	assert(b.insert("testString2") == 0);
	assert(b.insert("testString1") == 0);
	assert(b.insert("testString0") == 0);
	assert(b.size() == 3);
	assert(a.size() == 6);
	// Actual Swapping
	a.swap(b);
	assert(b.size() == 6);
	assert(a.size() == 3);
	assert(b.find("test0") == 0);
	assert(b.find("test1") == 2);
	assert(b.find("test2") == 4);
	assert(a.find("testString2") == 2);
	assert(a.find("testString1") == 1);
	assert(a.find("testString0") == 0);
	// Swapping with an empty Sequence
	Sequence c;
	c.swap(a);
	assert(a.size() == 0);
	assert(c.size() == 3);
	assert(c.find("testString2") == 2);
	assert(c.find("testString1") == 1);
	assert(c.find("testString0") == 0);
	// Self Swap
	c.swap(c);
	assert(c.size() == 3);
	assert(c.find("testString2") == 2);
	assert(c.find("testString1") == 1);
	assert(c.find("testString0") == 0);
	// Self Swap empty sequence
	a.swap(a);
	assert(a.size() == 0);
	assert(a.empty());


	//** TESTING Copy Constructor **//

	Sequence d(c);
	assert(d.size() == 3);
	assert(d.find("testString2") == 2);
	assert(d.find("testString1") == 1);
	assert(d.find("testString0") == 0);
	assert(d.remove("testString0") == 1);
	assert(d.remove("testString1") == 1);
	assert(d.remove("testString2") == 1);
	assert(d.size() == 0);
	// C should NOT be affected
	assert(c.size() == 3);
	assert(c.find("testString2") == 2);
	assert(c.find("testString1") == 1);
	assert(c.find("testString0") == 0);

	// Copying empty Sequence should be ok
	Sequence e;
	Sequence f(e);
	assert(f.size() == 0);
	assert(f.insert("testString2") == 0);
	assert(f.insert("testString1") == 0);
	assert(f.insert("testString0") == 0);
	assert(f.size() == 3);


	//** TESTING Assignment Operator **//
	Sequence g;
	g = f;
	assert(g.find("testString2") == 2);
	assert(g.find("testString1") == 1);
	assert(g.find("testString0") == 0);
	assert(g.size() == 3);
	assert(f.find("testString2") == 2);
	assert(f.find("testString1") == 1);
	assert(f.find("testString0") == 0);
	assert(f.size() == 3);
	Sequence h;
	// check if a Sequence reference is returned
	h = g = f;
	assert(h.find("testString2") == 2);
	assert(h.find("testString1") == 1);
	assert(h.find("testString0") == 0);
	assert(h.size() == 3);


	//** TESTING Destructor  **//
	Sequence* l = new Sequence(a);
	delete l;
	l = new Sequence(b);
	delete l;
	assert(b.size() == 6);
	assert(b.find("test0") == 0);
	assert(b.find("test1") == 2);
	assert(b.find("test2") == 4);
	// b should not be affected
	destructorTest();
	// If destructor didn't have errors, this point will be reached

	//** TEST SUBSEQUENCE **//
	// setup::
	g.erase(0);
	g.erase(0);
	g.erase(0);
	// test if subsequences of length 1 can be found in beginning, mid & edn
	assert(h.insert("testString3") == 3);
	assert(h.insert("testString4") == 4);
	assert(h.insert("testString5") == 5);
	assert(g.insert("testString0") == 0);
	assert(g.insert("testString1") == 1);
	assert(subsequence(h, g) == 0);
	// middle
	assert(g.remove("testString0") == 1);
	assert(g.remove("testString1") == 1);
	assert(g.insert("testString3") == 0);
	assert(subsequence(h, g) == 3);
	// end
	assert(g.remove("testString3") == 1);
	assert(g.insert("testString5") == 0);
	assert(subsequence(h, g) == 5);

	// Test if subsequences returns "smallest k" if multiple
	// copies of the subsequence is present
	assert(h.insert(6, "testString3"));
	assert(h.insert(7, "testString4"));
	assert(h.insert(8, "testString5"));
	assert(g.remove("testString5") == 1);
	assert(g.insert("testString5") == 0);
	assert(g.insert("testString4") == 0);
	assert(g.insert("testString3") == 0);
	assert(subsequence(h, g) == 3);


	//** TEST INTERLEAVE **//
	Sequence i;
	// normal use with empty result sequence and 2 non empty sequence 1 & 2
	interleave(h, g, i);
	assert(i.size() == (g.size() + h.size()));
	// non empty result
	i.insert("increasesLengthBy1");
	interleave(h, g, i);
	assert(i.size() == (g.size() + h.size()));
	// empty sequence 1 or 2
	Sequence j, k;
	interleave(h, j, i);
	assert(i.size() == h.size());
	interleave(j, k, i);
	assert(i.empty());
	// Aliasing test
	g = h;
	interleave(h, h, i);
	assert(i.size() == g.size() * 2);
	interleave(g, h, h);
	assert(h.size() == g.size() * 2);
	h = g;
	interleave(g, g, g);
	assert(g.size() == h.size() * 2);

	//show(g);
	//show(h);
	//show(i);
}