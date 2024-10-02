#ifndef ARRAYS
#define ARRAYS

#include <iostream>
#include <fstream>
#include <cstring>
#include <string>

using namespace std;


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
void selectionSort(string* arr, int size);
void swapStrings(string& a, string& b); // to replace swap from <algorithm>
int binarySearch(string* arr, int size, const string& word);
void countSentimentWords(const string& review, string* positiveWords, int positiveCount, string* negativeWords, int negativeCount, int& positiveCountInReview, int& negativeCountInReview);
double calculateSentimentScore(int positiveCount, int negativeCount, int maxCount);
void analyzeReviews(ReviewNode* reviews, string* positiveWords, int positiveCount, string* negativeWords, int negativeCount);
void deleteReviews(ReviewNode* head);

#endif // !ARRAYS
