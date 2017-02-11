#include "ScoreList.h"
#include <iostream>
#include <cassert>

using namespace std;

int main()
{
	ScoreList a;
	assert(a.maximum() == NO_SCORE);
	assert(a.minimum() == NO_SCORE);

	assert(!a.add(-1));
	assert(a.add(0));
	assert(a.add(100));
	assert(!a.add(101));
	assert(a.size() == 2);

	assert(a.minimum() == 0);
	assert(a.maximum() == 100);

	assert(a.add(100));
	assert(a.add(100));
	assert(a.add(100));
	assert(a.size() == 5);

	ScoreList s = a;
	assert(s.size() == 5);
	assert(s.minimum() == 0);
	assert(s.maximum() == 100);

	ScoreList b;
	assert(b.minimum() == NO_SCORE);
	assert(b.maximum() == NO_SCORE);
	b = a;
	assert(b.size() == 5);
	assert(b.minimum() == 0);
	assert(b.maximum() == 100);

	assert(a.remove(100));
	assert(a.size() == 4);
	assert(a.remove(100));
	assert(a.size() == 3);
	assert(a.remove(100));
	assert(a.size() == 2);
	assert(a.remove(100));
	assert(a.size() == 1);
	assert(a.maximum() == 0);

	assert(!a.remove(100));


	cout << "Passed all tests!" << endl;
}