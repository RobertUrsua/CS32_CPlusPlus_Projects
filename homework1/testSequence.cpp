#include "Sequence.h"
#include <iostream>
#include <cassert>

using namespace std;


int main()
{
	// TESTING BASE FUNCTIONS
	Sequence s;
	assert(s.empty());
	assert(s.insert(0, 20));
	assert(s.insert(0, 1));
	assert(s.insert(0, 3));
	assert(s.insert(0, 63));
	assert(s.insert(4, 10));
	assert(s.insert(-1, 10) == false);
	assert(s.insert(6, 63) == false);
	// Sequence is now 63 3 1 20 10
	assert(s.size() == 5);

	// check one argument insert
	assert(s.insert(100) == 5);
	assert(s.insert(200) == 6);
	assert(s.insert(90) == 5);

	// check GET function
	ItemType x = 999;
	assert(s.get(0, x) && x == 63);
	assert(s.get(4, x) && x == 10);
	assert(s.get(5, x) && x == 90);
	assert(s.get(7, x) && x == 200);
	assert(s.get(8, x) == false);
	assert(s.get(-1, x) == false);
	assert(s.size() == 8);

	// check erase function
	assert(s.erase(0));
	assert(s.erase(6));
	assert(s.erase(-1) == false);
	assert(s.erase(6) == false);
	// 3 1 20 10 90 100
	assert(s.size()==6);
	
	// check remove() function
	assert(s.remove(443) == 0);
	assert(s.remove(1) == 1);
	assert(s.insert(20));
	assert(s.remove(20) == 2);
	assert(s.size() == 4);
	// 3 10 90 100

	assert(s.set(2, 10));
	assert(!s.set(-1, 10));
	assert(!s.set(4, 10));
	assert(s.find(2) == -1);
	assert(s.find(10) == 1);

	Sequence a;
	assert(a.empty());

	//test swap
	s.swap(a);

	// a: 3 10 10 100
	// s: 

	//test s after sawp
	assert(s.empty());
	assert(s.size() == 0);

	s.swap(s);
	assert(s.empty());
	assert(s.size() == 0);

	//test a 
	assert(a.get(0, x) && x == 3);
	assert(a.get(1, x) && x == 10);
	assert(a.get(2, x) && x == 10);
	assert(a.get(3, x) && x == 100);
	assert(a.get(-1, x) == false);
	assert(a.get(4, x) == false);

	assert(a.insert(5, 4) == false);
	assert(a.insert(4, 3) == true);

	assert(!a.set(5, 3));

	//aliasing test
	a.swap(a);
	assert(a.get(0, x) && x == 3);
	assert(a.get(1, x) && x == 10);
	assert(a.get(2, x) && x == 10);
	assert(a.get(3, x) && x == 100);
	assert(a.get(-1, x) == false);

	assert(a.insert(6, 4) == false);
	assert(a.insert(4, 3) == true);

	assert(!a.set(8, 3));

	cout << "Passed all tests" << endl;
}
