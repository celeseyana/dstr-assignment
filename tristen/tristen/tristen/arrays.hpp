#ifndef ARRAYS
#define ARRAYS

#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
using namespace std;
using namespace chrono;

// Struct to store reviews in an array
struct Review {
    string review;
    int rating; // User's rating
};

// Function declarations
int loadWords(const char* filename, string* wordArray, int maxWords);
int loadReviews(const char* filename, Review* reviewsArray, int start, int batchSize, int maxReviews);
void insertionSort(string* arr, int size);
void swapStrings(string& a, string& b);
int binarySearch(string* arr, int size, const string& word);
void countSentimentWords(const string& review, string* positiveWords, int positiveCount,
    string* negativeWords, int negativeCount,
    int* positiveWordFrequency, int* negativeWordFrequency);
double calculateSentimentScore(int positiveCount, int negativeCount, int maxCount);
void insertionSortByFrequency(string words[], int freq[], int count);
void analyzeReviewsInBatches(const char* filename, string* positiveWords, int positiveCount, string* negativeWords, int negativeCount, int batchSize, int maxReviews);
void analyzeBatch(Review* reviewsArray, int batchSize, string* positiveWords, int positiveCount,
    string* negativeWords, int negativeCount, int& totalPositiveWords,
    int& totalNegativeWords, int* positiveWordFrequency, int* negativeWordFrequency);  // Added declaration
int countTotalReviews(const char* filename);
void analyzeReviews(Review* reviewsArray, int totalReviews, string* positiveWords, int positiveCount, string* negativeWords, int negativeCount);
#endif // ARRAYS
