#include "provided.h"
#include "MyMap.h"
#include <iostream>
#include <string>
#include <vector>
#include <cassert>
using namespace std;

int main()
{
	Navigator navObj;
	navObj.loadMapData("mapdata.txt");
	vector<NavSegment> xdLUL;
	navObj.navigate("1061 Broxton Avenue", "Headlines!", xdLUL);


	string x;
	do {
		cout << "Enter two custom locations to navigate to and from:";
		string st, en;
		getline(cin, st);
		getline(cin, en);

		navObj.navigate(st, en, xdLUL);
		cout << "Enter y to try again :";
		getline(cin, x);
	} while (x == "y");
	/*
		Riviera Country Club | Allenford Avenue
		Antietam Avenue | Antietam Avenue
		The Annenberg Space for Photography | Avenue of the Stars
		Hyatt Regency Century Plaza | Avenue of the Stars
		2000 Avenue of the Stars | Avenue of the Stars
		Fox Plaza | Avenue of the Stars
		Twentieth Century Fox Film Corporation | Avenue of the Stars
		Century City Heliport | Avenue of the Stars
		1160 Barry Avenue | Barry Avenue
		1252 Barry Avenue 
	*/

	return 0;
}