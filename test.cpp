// import libraries
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;
const int MAX_POS_WORDS = 2006;
const int MAX_NEG_WORDS = 4783;
const int MAX_WORDS = 5000;

// functions
// load and read the positive and negative words into an array
void loadWords(const string & filename, string words[], int& wordCount)
{
    ifstream file(filename);
    string word;
    wordCount = 0;

    while (file >> word)
    {
        words[wordCount++] = word;
    }

    cout << "Number of Positive / Negative Words: " << wordCount << endl;
    file.close();
}


// main
int main()
{
    string positiveWords[MAX_POS_WORDS], negativeWords[MAX_NEG_WORDS], review[MAX_WORDS];
    int posCount, negCount, reviewCount;

    loadWords("files/positive-words.txt", positiveWords, posCount);
    loadWords("files/negative-words.txt", negativeWords, negCount);
}