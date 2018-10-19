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
	// TODO: Might go back to the vector<vector> here, as discovering 
	// that an item is non-existent is possibly less efficient with a map
	map<char, map<char, char>> between;
	// Because of the unorthodox indexing for this 
	// game board (A - Y without O represent 10-33) 
	// a map is used to access the between matrix. 
	map<char, int> index;

	void initializeIndex();
	void initializeBetween();
};

HiQ::HiQ() 
{
	initializeIndex();
	initializeBetween();
}

void HiQ::initializeIndex()
{
	char letter = 'A';
	for (int i = 10; i <= 33; i++)
	{
		if (letter == 'O')
			++letter;
		
		index[letter++] = i;
	}
}

void HiQ::initializeBetween()
{
	// At between[i][j], the value for the hole "between" i and j is found. 
	// If you cannot jump from i to j, then the value is 0
	


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