#include "arrays.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cctype>

using namespace std;

FileReader::FileReader()
{
	negativeRead = 0;
	positiveRead = 0;
	reviewsRead = 0;
}

void FileReader::readNegative(const string& filename)
{
	ifstream file("D:/Github/dstr-assignment/required/negative-words.txt");

	if (file.is_open())
	{
		string line;

		while (getline(file, line) && negativeRead < array_size)
		{
			negativeWords[negativeRead] = line;
			negativeRead++;
		}
		file.close();
	}
	else
	{
		cerr << "Error, file cannot be opened." << endl;
	}
}
void FileReader::readPositive(const string& filename)
{
	ifstream file("D:/Github/dstr-assignment/required/postiive-words.txt");

	if (file.is_open())
	{
		string line;

		while (getline(file, line) && positiveRead < array_size)
		{
			positiveWords[positiveRead] = line;
			positiveRead++;
		}
		file.close();
	}
	else
	{
		cerr << "Error, file cannot be opened." << endl;
	}
}
int main() {
	FileReader reader;
	reader.readNegative("D:/Github/dstr-assignment/required/negative-words.txt");
	reader.readPositive("D:/Github/dstr-assignment/required/postiive-words.txt");
	return 0;
}