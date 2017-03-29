#include "MyMap.h"
#include <iostream>
#include <string>
#include <cassert>
using namespace std;

int main()
{
	MyMap<int, int> a;
	
	// Size of empty map must be 0 and finding must always fail
	assert(a.size() == 0);
	assert(a.find(1) == nullptr);

	// Testing insertion of new head Node
	// size must be adjusted accordingly and the head node must be locatable
	// Unassociated key values must not be locatable in the map
	a.associate(5, 5);
	assert(a.size() == 1);
	assert(a.find(1) == nullptr);
	assert(*(a.find(5)) == 5);

	// Testing insertion of new child Node
	// size must be adjusted accordingly and the node must be locatable
	a.associate(3, 3);
	assert(a.size() == 2);
	assert(a.find(1) == nullptr);
	assert(*(a.find(3)) == 3);

	// Testing insertion of new child Node
	// size must be adjusted accordingly and the node must be locatable
	a.associate(8, 8);
	assert(a.size() == 3);
	assert(*(a.find(8)) == 8);

	// Testing association of a value to a preexisting key
	// size must not change 
	// Value of the appropiate preexisting node must be adjusted as specified
	a.associate(8, 4);
	assert(a.size() == 3);
	assert(*(a.find(8)) == 4);


	MyMap<string, double> nameToGPA; // maps student name to GPA
									 // add new items to the binary search tree-based map
	nameToGPA.associate("Carey", 3.5); // Carey has a 3.5 GPA
	nameToGPA.associate("David", 3.99); // David beat Carey
	nameToGPA.associate("Abe", 3.2); // Abe has a 3.2 GPA

	assert(*(nameToGPA.find("Carey")) == 3.5);
	assert(*(nameToGPA.find("David")) == 3.99);
	assert(*(nameToGPA.find("Abe")) == 3.2);

	double* davidsGPA = nameToGPA.find("David");
	if (davidsGPA != nullptr)
		*davidsGPA = 1.5; // after a re-grade of David’s exam

	assert(*(nameToGPA.find("David")) == 1.5);

	nameToGPA.associate("Carey", 4.0); // Carey deserves a 4.0
									   // replaces old 3.5 GPA
	assert(*(nameToGPA.find("Carey")) == 4.0);


	double* lindasGPA = nameToGPA.find("Linda");
	if (lindasGPA == nullptr)
		cerr << "Linda is not in the roster!" << endl;
	else
		cerr << "Linda’s GPA is: " << *lindasGPA << endl;

	cerr << "Passed all Test" << endl;
}