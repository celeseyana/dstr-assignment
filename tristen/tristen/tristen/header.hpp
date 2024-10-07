#ifndef HOTEL_FEEDBACK_H
#define HOTEL_FEEDBACK_H

#include <string>
using namespace std;

// Constants
const int MAX_WORDS = 5000; // Max number of words for positive/negative lists

// Global variables
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
#endif // HOTEL_FEEDBACK_H
