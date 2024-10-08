#ifndef HEADER
#define HEADER

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>
#include <iomanip>
#include <string>
#include <cstring>
#include <limits>
#include <chrono>
using namespace std;
using namespace chrono;

//-----------------------------------------ONG ZI YANG-----------------------------------------------------------------
class FileReader
{
public:
    static const int array_size = 5000;
    static const int csv_array_size = 21000;
    std::string negativeWords[array_size];
    std::string positiveWords[array_size];
    std::string reviews[csv_array_size];
    int ratings[csv_array_size];

    int negativeRead = 0;
    int positiveRead = 0;
    int reviewsRead = 0;

    FileReader();

    void readNegative(const std::string& filename);
    void readPositive(const std::string& filename);
    void countWordMatches(int& positiveCount, int& negativeCount);

};
void displayMenu();
void printCSV(std::string** data, int numRows, int numCols);
void analyzeCSV(std::string** data, int numRows, int numCols, FileReader& reader);
int totalCount(const std::string& filename);
void countWordFrequencies(std::string** data, int numRows, FileReader& reader, int positiveCounts[], int negativeCounts[]);
void bubbleSort(std::string words[], int counts[], int size);
void displayTopWords(std::string words[], int counts[], int size);

string** readCSV(const std::string& filename, int& numRows, int& numCols);
//--------------------------------------------TRISTEN------------------------------------------------------------------
// Constants
const int MAX_WORDS = 5000; // Max number of words for positive/negative lists

extern string uniqueWords[MAX_WORDS];    // Array for unique words
extern int wordFrequencies[MAX_WORDS];   // Array for word frequencies
extern int uniqueWordCount;


extern string positiveWords[MAX_WORDS];
extern string negativeWords[MAX_WORDS];
extern int posWordCount;
extern int negWordCount;
extern string* reviews; // Dynamically allocated array for reviews
extern int* ratings;    // Dynamically allocated array for ratings
extern int reviewCount;

// Function prototypes
void loadWords(const string& filename, string arr[], int& count);
void loadReviews(const string& filename);
bool binarySearch(const string& word, const string arr[], int size);
double calculateSentimentScore(string review);
void insertionSort(string arr[], int size);
void analyzeReviews();
void calculateOverallSentiment();
int findWordIndex(const string& word);
#endif HEADER