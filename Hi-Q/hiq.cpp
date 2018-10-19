// ---------------------------------------------------
// hiq.cpp
// Jeremy Campbell
// Plays the game Hi-Q 
// ---------------------------------------------------
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>

using std::cerr;
using std::endl;
using std::ifstream;
using std::ofstream;
using std::string;
using std::vector;
using std::map;

class HiQ
{
public:
	HiQ();
	void play(char vacantHole);
	void printResults(char newVacantHole);

private:
	const int NUMHOLES = 15;
	string solution;
	vector<vector<char>> between;
	// Because of the unorthodox indexing for this 
	// game board (A - Y without O represent 10-33) 
	// a map is used to access the between matrix. 
	map<char, short> index;

	void initializeIndex();
	void setBetween(char first, char last, char mid);
	void initializeBetween();
};

HiQ::HiQ() 
{
	initializeIndex();
	initializeBetween();
}

void HiQ::initializeIndex()
{
	char letter;
	for (int i = 10; i <= 33; i++)
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
}

void HiQ::initializeBetween()
{
	// At between[i][j], the value for the hole "between" i and j is found. 
	// If you cannot jump from i to j, then the value is 0
	between.resize(NUMHOLES + 1, vector<char>(NUMHOLES + 1, 0));

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

}

// Need the new vacantHole to know if the endl should be printed
void HiQ::printResults(char newVacantHole)
{

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
		fin >> vacantHole;
		game.printResults(vacantHole);	
	}	
}