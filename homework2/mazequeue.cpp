#include <iostream>
#include <string>
#include <queue>

using namespace std;

class Coord;

bool pathExists(string maze[], int nRows, int nCols, int sr, int sc, int er, int ec);
// Return true if there is a path from (sr,sc) to (er,ec)
// through the maze; return false otherwise

void checkCell(string maze[], int nRows, int nCols, int r, int c, queue<Coord>& cstack);
// checks if a cell is within the maze and is a walkway
// if it is, mark on the maze, push onto queue

class Coord
{
public:
	Coord(int rr, int cc) : m_r(rr), m_c(cc) {}
	int r() const { return m_r; }
	int c() const { return m_c; }
private:
	int m_r;
	int m_c;
};

void checkCell(string maze[], int nRows, int nCols, int r, int c, queue<Coord>& cqueue) {
	if (r < 0 || r >= nRows || c < 0 || c >= nCols)
		return;
	if (maze[r][c] == '.') {
		Coord nextCell(r, c);
		cqueue.push(nextCell);
		maze[r][c] = '+';
	}
}

bool pathExists(string maze[], int nRows, int nCols, int sr, int sc, int er, int ec)
{
	queue<Coord> cqueue;
	Coord current(sr, sc); // For keeping track of current cell
	cqueue.push(current);

	maze[current.r()][current.c()] = '+';

	while (!cqueue.empty()) {
		current = cqueue.front();

		//show(maze, 10);
		//cout << "(" << current.r() << "," << current.c() << ")" << endl;
		
		if (current.r() == er&&current.c() == ec)
			return true;

		cqueue.pop();
		checkCell(maze, nRows, nCols, current.r()-1, current.c(), cqueue);
		checkCell(maze, nRows, nCols, current.r(), current.c()+1, cqueue);
		checkCell(maze, nRows, nCols, current.r()+1, current.c(), cqueue);
		checkCell(maze, nRows, nCols, current.r(), current.c()-1, cqueue);

		
	}

	return false;
}


int main()
{
	string maze[10] = {
		"XXXXXXXXXX",
		"X........X",
		"XX.X.XXXXX",
		"X..X.X...X",
		"X..X...X.X",
		"XXXX.XXX.X",
		"X.X....XXX",
		"X..XX.XX.X",
		"X...X....X",
		"XXXXXXXXXX"
	};

	if (pathExists(maze, 10, 10, 6, 4, 1, 1))
		cout << "Solvable!" << endl;
	else
		cout << "Out of luck!" << endl;

	return 0;
}