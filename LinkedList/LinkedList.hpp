#pragma once
#ifndef LINKEDLIST_HPP
#define LINKEDLIST_HPP
using namespace std;
#include <string>

struct WordNode
{
	string word;
	int count;
	WordNode* next;
};

class LinkedList
{
	public:
		void loadWords(const string& filename, WordNode*& head);
		void insertIntoLinkedList(WordNode*& head, const string& word);
		int countWordsInReview(const string& review, WordNode*& reviewNode, WordNode*& wordListHead);
		void displayWordList(const string& label, WordNode* head);
		int convertStringRating(string ratingStr);
		double calculateSentimentScore(int positiveCount, int negativeCount);;
		string levelOfSentiment(double sentimentScore);
		void compareRatings(int userRating, double sentimentScore);
		WordNode* mergeLists(WordNode* positiveHead, WordNode* negativeHead);
		void selectionSort(WordNode*& head);
		void displaySentimentResult(int totalReviews, int totalPositive, int totalNegative, WordNode* positiveListHead, WordNode* negativeListHead);
};

#endif // !LINKEDLIST_HPP
