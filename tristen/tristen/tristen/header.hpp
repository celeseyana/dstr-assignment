#ifndef HOTEL_FEEDBACK_H
#define HOTEL_FEEDBACK_H

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>
#include <iomanip>
#include <chrono>
#include <cctype>

using namespace std;
using namespace chrono;

// Constants
const int MAX_WORDS = 5000; // Max number of words for positive/negative lists
const int MAX_FREQUENCY_WORDS = 1000; // Max number of unique words for frequency tracking (define only once)

// Global variables
extern string positiveWords[MAX_WORDS];
extern string negativeWords[MAX_WORDS];
extern int posWordCount;
extern int negWordCount;
extern string* reviews; // Dynamically allocated array for reviews
extern int* ratings;    // Dynamically allocated array for ratings
extern int reviewCount;

// Global variables for frequency tracking
extern int totalPositiveWords;
extern int totalNegativeWords;
extern string frequencyWords[MAX_FREQUENCY_WORDS]; // Array to store unique words
extern int frequencyCount[MAX_FREQUENCY_WORDS];    // Array to store corresponding word frequencies
extern int frequencySize; // Track how many unique words are in frequencyWords array

// Function prototypes
void loadWords(const string& filename, string arr[], int& count);
void loadReviews(const string& filename);
bool binarySearch(const string& word, const string arr[], int size);
double calculateSentimentScore(string review);
void insertionSort(string arr[], int size);
void analyzeReviews();
void updateWordFrequency(const string& word);
void displayOverallSentiment();
//void displayWordFrequency();
bool isValidWord(const string& word);
string sanitizeWord(const string& word);
void countWords(const std::string& review);
void displayTotalCounts();
int findIndex(const std::string& word, const std::string frequencyWords[], int size);

#endif // HOTEL_FEEDBACK_H
