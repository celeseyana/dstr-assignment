#ifndef ARRAYS
#define ARRAYS

#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include <chrono>
using namespace std;
using namespace chrono;

// to store reviews and ratings in a linked list
struct ReviewNode
{
	string review;
	int rating; // User's rating
	ReviewNode* next;
};

// function declarations
int loadWords(const char* filename, string* wordArray, int maxWords);
ReviewNode* loadReviews(const char* filename);
void insertionSort(string* arr, int size);
void swapStrings(string& a, string& b); // to replace swap from <algorithm>
int binarySearch(string* arr, int size, const string& word);
void countSentimentWords(const string& review, string* positiveWords, int positiveCount, string* negativeWords, int negativeCount, int& positiveCountInReview, int& negativeCountInReview, string* foundPositiveWords, string* foundNegativeWords);
double calculateSentimentScore(int positiveCount, int negativeCount, int maxCount);
void insertionSortByFrequency(string words[], int freq[], int count);
void analyzeReviews(ReviewNode* reviews, string* positiveWords, int positiveCount, string* negativeWords, int negativeCount);
void deleteReviews(ReviewNode* head);

#endif // !ARRAYS
