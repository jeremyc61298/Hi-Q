// ---------------------------------------------------
// hiq.cpp
// Jeremy Campbell
// Plays the game Hi-Q 
// ---------------------------------------------------
#include <iostream>
#include <fstream>
#include <string>

using std::cerr;
using std::endl;
using std::ifstream;
using std::ofstream;
using std::string;

class HiQ
{
public:
	void play(char vacantHole);
	void printResults(char newVacantHole);

private:
	string solution;
};

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
		game.playHiQ(vacantHole);
		fin >> vacantHole;
		game.printResults(vacantHole);	
	}	
}