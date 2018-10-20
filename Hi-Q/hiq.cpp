// ---------------------------------------------------
// hiq.cpp
// Jeremy Campbell
// Solves the game HiQ using a Depth-First Search
// Logic is encapsulated in the HiQ class.
// ---------------------------------------------------
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <bitset>
#include <stack>
#include <set>

using std::cerr;
using std::endl;
using std::ifstream;
using std::ofstream;
using std::string;
using std::vector;
using std::map;
using std::bitset;
using std::stack;
using std::set;

#define NUMHOLES 33

class HiQ
{
public:
	HiQ();
	void play(char vacantHole);
	void printResults(ofstream &fout, char vacantHole);

private:
	/* ---- VARIABLES ---- */
	vector<vector<char>> between;
	map<char, short> index;
	map<short, char> rindex;
	typedef bitset<NUMHOLES + 1> BoardState;
	BoardState initialBoardState;
	BoardState solutionBoardState;
	struct jumpType { char from; char to; };
	stack<jumpType> jumpStack;
	set< unsigned long long > markedSet;

	/* ---- MEHTODS ---- */
	void initializeIndices();
	void setBetween(char first, char last, char mid);
	void initializeBetween();
	void initializeBoardStates(char initialHole);
	bool findSolution(BoardState currentBoardState);
	void mark(BoardState currentBoardState);
	bool marked(BoardState currentBoardState);
	BoardState findNewBoardState(jumpType jump, BoardState currentBoardState);
	bool validJump(jumpType jump, BoardState currentBoardState);

	template <typename T>
	void print2dVector(string name, const vector<vector<T>> &v);
};


HiQ::HiQ() 
{
	initializeIndices();
	initializeBetween();
}

// Because of the unorthodox indexing for this 
// game board (1-9, A - Y without O represent 10-33) 
// a map is used to access the between matrix. 
void HiQ::initializeIndices()
{
	char letter;
	for (int i = 0; i <= 33; i++)
	{
		// The index is the actual number (1-9)
		if (i < 10)
			letter = i + '0';

		// The index is a letter (A-Y)
		if (i == 10)
			letter = 'A';

		// Skip 'O'
		if (letter == 'O')
			++letter;
		
		index[letter++] = i;
	}

	// rindex is the reverse of index, with the values of index
	// as the keys of rindex.
	for (auto entry : index)
	{
		rindex[entry.second] = entry.first;
	}
}

void HiQ::initializeBetween()
{
	// At between[i][j], the value for the hole "between" i and j is found. 
	// If you cannot jump from i to j, then the value is 0
	between.resize(NUMHOLES + 1, vector<char>(NUMHOLES + 1, '0'));

	setBetween('1', '9', '4');
	setBetween('1', '3', '2');
	setBetween('2', 'A', '5');
	setBetween('3', 'B', '6');
	setBetween('4', '6', '5');
	setBetween('4', 'G', '9');
	setBetween('5', 'H', 'A');
	setBetween('6', 'I', 'B');
	setBetween('7', 'L', 'E');
	setBetween('7', '9', '8');
	setBetween('8', 'A', '9');
	setBetween('8', 'M', 'F');
	setBetween('9', 'B', 'A');
	setBetween('9', 'N', 'G');
	setBetween('A', 'C', 'B');
	setBetween('A', 'P', 'H');
	setBetween('B', 'D', 'C');
	setBetween('B', 'Q', 'I');
	setBetween('C', 'R', 'J');
	setBetween('D', 'S', 'K');
	setBetween('E', 'G', 'F');
	setBetween('F', 'H', 'G');
	setBetween('G', 'I', 'H');
	setBetween('G', 'T', 'N');
	setBetween('H', 'J', 'I');
	setBetween('H', 'U', 'P');
	setBetween('I', 'K', 'J');
	setBetween('I', 'V', 'Q');
	setBetween('L', 'N', 'M');
	setBetween('M', 'P', 'N');
	setBetween('N', 'Q', 'P');
	setBetween('N', 'W', 'T');
	setBetween('P', 'R', 'Q');
	setBetween('P', 'X', 'U');
	setBetween('Q', 'S', 'R');
	setBetween('Q', 'Y', 'V');
	setBetween('T', 'V', 'U');
	setBetween('W', 'Y', 'X');

	//print2dVector("Between", between);
}

// At between[i][j], the value for the hole "between" i and j is found. 
// If you cannot jump from i to j, then the value is 0
// This uses the "index" to map the vector subscripts to the board locations
void HiQ::setBetween(char first, char last, char mid) 
{
	between[index.at(first)][index.at(last)] = between[index.at(last)][index.at(first)] = mid;
}

void HiQ::play(char vacantHole)
{
	initializeBoardStates(vacantHole);
	markedSet.clear(); // Make sure the set is ready for another solution
	findSolution(initialBoardState);
}

void HiQ::initializeBoardStates(char initialHole)
{
	initialBoardState.set();
	initialBoardState.flip(0);
	initialBoardState.flip(index[initialHole]);

	solutionBoardState.reset();
	solutionBoardState.flip(index[initialHole]);
}

// This function was given by Dr. Baber in class.
// I have modified it slightly to fit with my program structure.
// His function was not a part of a class
bool HiQ::findSolution(BoardState currentBoardState)
{
	bool solutionFound = false;
	if (currentBoardState == solutionBoardState) { solutionFound = true;  }
	else
	{
		mark(currentBoardState);
		for (short from = 1; !solutionFound && from <= NUMHOLES; from++)
		{
			for (short to = 1; !solutionFound && to <= NUMHOLES; to++)
			{
				// Must translate the jump from shorts to chars
				jumpType jump = { rindex.at(from), rindex.at(to) };
				BoardState newBoardState = findNewBoardState(jump, currentBoardState);
				if (!marked(newBoardState))
				{
					solutionFound = findSolution(newBoardState);
					if (solutionFound)
						jumpStack.push(jump);
				}
			}
		}
	}

	return solutionFound;
}

// Given by Dr. Baber in class
void HiQ::mark(BoardState currentBoardState)
{
	markedSet.insert(currentBoardState.to_ullong());
}

bool HiQ::marked(BoardState currentBoardState)
{
	bool found = false;
	found = markedSet.count(currentBoardState.to_ullong()) == 1;
	return found;
}

// Given by Dr. Baber in class, modified slightly (different names)
HiQ::BoardState HiQ::findNewBoardState(jumpType jump, BoardState currentBoardState)
{
	BoardState newBoardState = currentBoardState;

	if (validJump(jump, currentBoardState))
	{
		// Change the board to represent the valid jump has occured
		newBoardState.flip(index.at(jump.from));
		newBoardState.flip(index.at(jump.to));
		newBoardState.flip(index.at(between[index.at(jump.from)][index.at(jump.to)]));
	}

	return newBoardState;
}

// Given by Dr. Baber, modified slightly (using my index map)
bool HiQ::validJump(jumpType jump, BoardState currentBoardState)
{
	// This function returns true if
	// (1) there is a pegin the 'jump.from' hole AND
	// (2) there is no peg in the 'jump.to' hole AND
	// (3) there is a peg in the hole between 'jump.from' and 'jump.to'
	// otherwise, the function returns false
	bool valid = false;
	if (currentBoardState.test(index.at(jump.from)) &&
		!currentBoardState.test(index.at(jump.to)) &&
		currentBoardState.test(index.at(between[index.at(jump.from)][index.at(jump.to)])))
	{
		valid = true;
	}

	return valid;
}

// Debug function
template <typename T>
void HiQ::print2dVector(string name, const vector<vector<T>> &v)
{
	cerr << name << ": " << endl;
	cerr << "---------------------------------" << endl;
	for (vector<T> row : v)
	{
		for (T col : row)
		{
			cerr << col << " ";
		}
		cerr << endl;
	}
	cerr << "---------------------------------" << endl;
}

// Need the new vacantHole to know if the endl should be printed
void HiQ::printResults(ofstream &fout, char vacantHole)
{
	if (jumpStack.empty())
		fout << "No solution for hole at " << vacantHole;
	else
	{
		// There is a solution
		while (!jumpStack.empty())
		{
			jumpType jump = jumpStack.top(); jumpStack.pop();
			fout << jump.from << "->" << jump.to;

			if (!jumpStack.empty())
				fout << " ";
		}
	}
}

int main()
{
	ifstream fin("hiq.in");

	if (!fin.is_open())
	{
		cerr << "Problem opening input file. " << endl;
		return -1;
	}

	ofstream fout("hiq.out");

	HiQ game;
	char vacantHole;
	fin >> vacantHole;

	while (vacantHole != '0')
	{
		game.play(vacantHole);
		game.printResults(fout, vacantHole);	
		cerr << "Solved at " << vacantHole << endl;
		fin >> vacantHole;
		if (vacantHole != '0')
			fout << endl;
	}	
}