#include "LinkedList.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <iomanip>
#include <ctime>
#include <chrono>
using namespace std;
using namespace std::chrono;

// WORKED
// load positive and negative words into linked list
void LinkedList :: loadWords(const string& filename, WordNode*& head)
{
	string word;

	ifstream file(filename);
	if (!file.is_open())
	{
		cerr << "Error: Could not open file - " << filename << endl;
		return;
	}

	WordNode* current = nullptr;

	while (file >> word)
	{
		// Create a new WordNode
		WordNode* newWordNode = new WordNode{ word, 0, nullptr };

		if (head == nullptr)
		{
			head = newWordNode;
			current = head;
		}
		else
		{
			current->next = newWordNode;
			current = newWordNode;
		}
	}

	file.close();
}

// WIP
// load all reviews into a linked list first
void LinkedList :: loadReviews(const string& filename, ReviewNode*& reviewHead)
{
	ifstream file(filename);
	if (!file.is_open())
	{
		cerr << "Error - Could not open review file - " << filename << endl;
		return;
	}

	string line;
	getline(file, line); // skip "Review" and "Rating" header

	ReviewNode* current = nullptr;

	while (getline(file, line))
	{
		size_t lastComma = line.find_last_of(",");
		string review = line.substr(0, lastComma - 1);
		int rating = stoi(line.substr(lastComma + 1));

		ReviewNode* newNode = new ReviewNode{ review, rating, nullptr };

		if (reviewHead == nullptr)
		{
			reviewHead = newNode;
			current = reviewHead;
		}
		else
		{
			current->next = newNode;
			current = newNode;
		}
	}

	file.close();
}


// WORKED
// insert word into linked list
void LinkedList :: insertIntoLinkedList(WordNode*& head, const string& word)
{
	WordNode* temp = head;

	// check if the word exist in the linked list
	while (temp != nullptr)
	{
		if (temp->word == word)
		{
			temp->count++;
			return;
		}
		temp = temp->next;
	}
	WordNode* newNode = new WordNode{ word, 1, head };
	head = newNode;
}

// WORKED
// count positive and negative words in review
int LinkedList::countWordsInReview(const string& review, WordNode*& reviewList, WordNode*& wordListHead)
{
	int count = 0;
	stringstream ss(review);
	string word;

	while (ss >> word) {
		for (WordNode* node = wordListHead; node != nullptr; node = node->next) {
			if (word == node->word) {
				count++; // total +ve or -ve word count in review
				insertIntoLinkedList(reviewList, word); // insert the word and its count into review list
				node->count++; // update word count in main list
				break;
			}
		}
	}
	return count;
}

// process the reviews and count num of +ve and -ve words
void LinkedList::processReviews(ReviewNode* reviewHead, WordNode*& positiveListHead, WordNode*& negativeListHead)
{
	int totalReviews = 0, totalPositive = 0, totalNegative = 0;
	const string SEPARATOR(60, '-');

	for (ReviewNode* reviewNode = reviewHead; reviewNode != nullptr; reviewNode = reviewNode->next)
	{
		WordNode* reviewPositiveHead = nullptr;
		WordNode* reviewNegativeHead = nullptr;

		int positiveCount = countWordsInReview(reviewNode->review, reviewPositiveHead, positiveListHead);
		int negativeCount = countWordsInReview(reviewNode->review, reviewNegativeHead, negativeListHead);

		totalPositive += positiveCount;
		totalNegative += negativeCount;

		double sentimentScore = calculateSentimentScore(positiveCount, negativeCount);

		// Display the details of the current review
		cout << SEPARATOR << "\n\n"
			<< "Index: " << totalReviews + 1 << "\n\n"
			<< "User Rating: " << reviewNode->rating << "\n"
			<< "Review: " << reviewNode->review << "\n\n"
			<< "Positive Count = " << positiveCount << "\n";

		displayWordList("Positive Words", reviewPositiveHead);

		cout << "\nNegative Count = " << negativeCount << "\n";

		displayWordList("Negative Words", reviewNegativeHead);

		cout << setprecision(2) << "Sentiment Score (1 - 5) is " << sentimentScore
			<< ", thus the rating should be equal to " << round(sentimentScore) << " ("
			<< levelOfSentiment(sentimentScore) << ")" << "\n\n";

		compareRatings(reviewNode->rating, sentimentScore);

		while (reviewPositiveHead != nullptr)
		{
			WordNode* temp = reviewPositiveHead;
			reviewPositiveHead = reviewPositiveHead->next;
			delete temp;
		}

		while (reviewNegativeHead != nullptr)
		{
			WordNode* temp = reviewNegativeHead;
			reviewNegativeHead = reviewNegativeHead->next;
			delete temp;
		}

		totalReviews++;
	}

	cout << string(80, '=') << endl << endl;
	displaySentimentResult(totalReviews, totalPositive, totalNegative, positiveListHead, negativeListHead);
	cout << endl;
}

// WORKED
// display words in the linked list
void LinkedList::displayWordList(const string& label, WordNode* head)
{
	cout << label << ":" << endl;
	for (WordNode* node = head; node != nullptr; node = node->next)
	{
		cout << " - " << node->word << " (" << node->count << " times)\n";
	}
}

// WORKED
// convert user rating from string to integer
int LinkedList::convertStringRating(string ratingStr)
{
	try
	{
		return stoi(ratingStr);
	}
	catch (invalid_argument& e)
	{
		cerr << "Invalid rating format: " << ratingStr << endl;
		return -1;
	}
	catch (out_of_range& e)
	{
		cerr << "Rating out of range: " << ratingStr << endl;
		return -1;
	}
}

// WORKED
// calculate the sentiment score of the review
// based on number of +ve and -ve words
double LinkedList::calculateSentimentScore(int positiveCount, int negativeCount)
{
	int rawScore = positiveCount - negativeCount;
	int maxScore = positiveCount + negativeCount;
	int minScore = -(maxScore);

	double normalizedScore = (rawScore - minScore) / double(maxScore - minScore);

	return 1 + (4 * normalizedScore);
}

// WORKED
// determine the level of sentiment based on its score
string LinkedList::levelOfSentiment(double sentimentScore)
{
	switch (int(round(sentimentScore)))
	{
	case 1:	case 2:
		return "Negative";
	case 3:
		return "Neutral";
	case 4: case 5:
		return "Positive";
	default:
		return "Unknown";
	}
}

// WORKED
// compare rating given by user with sentiment calculation
void LinkedList::compareRatings(int userRating, double sentimentScore)
{
	cout << "Sentiment Score (1 - 5) = " << round(sentimentScore) << "\n";
	cout << "Rating given by user = " << userRating << "\n";
	cout << "Analysis Output: \n";

	if (round(sentimentScore) == userRating)
	{
		cout << "User's subjective evaluation MATCHES the sentiment score provided by the analysis. There is a consistency between the sentiment score generated by the analysis and the user's evaluation of the sentiment.\n";
	}
	else
	{
		cout << "User's subjective evaluation DOES NOT MATCHES the sentiment score provided by the analysis. There is an inconsistency between the sentiment score generated by the analysis and the user's evaluation of the sentiment.\n";
	}
}

// WORKED
// merge both +ve and -ve linked list
WordNode* LinkedList :: mergeLists(WordNode* positiveHead, WordNode* negativeHead)
{
	WordNode* combineHead = nullptr;
	WordNode** tail = &combineHead;

	// combine all nodes from the +ve linked list
	while (positiveHead != nullptr)
	{
		*tail = new WordNode{ positiveHead->word, positiveHead->count, nullptr };
		tail = &((*tail)->next);
		positiveHead = positiveHead->next;
	}

	// combine all nodes from the -ve linked list
	while (negativeHead != nullptr)
	{
		*tail = new WordNode{ negativeHead->word, negativeHead->count, nullptr };
		tail = &((*tail)->next);
		negativeHead = negativeHead->next;
	}

	return combineHead;
}

// selection sort function to sort the combined linked list
// according to the word counts in ascending order
void LinkedList :: selectionSort(WordNode*& head)
{
	for (WordNode* i = head; i != nullptr; i = i->next)
	{
		// find the node with the lowest count in the usorted linked list
		WordNode* minNode = i;
		for (WordNode* j = i->next; j != nullptr; j = j->next)
		{
			if (j->count < minNode->count)
			{
				minNode = j;
			}
		}

		// swap if a node with lower count is found
		if (minNode != i)
		{
			string tempWord = i->word;
			int tempCount = i->count;

			i->word = minNode->word;
			i->count = minNode->count;

			minNode->word = tempWord;
			minNode->count = tempCount;
		}
	}
}

// WORKED
// display overall sentiment results
void LinkedList :: displaySentimentResult(int totalReviews, int totalPositive, int totalNegative, WordNode* positiveListHead, WordNode* negativeListHead)
{
	cout << "Total Reviews = " << totalReviews << endl;
	cout << "Total Positive Words = " << totalPositive << endl;
	cout << "Total Negative Words = " << totalNegative << endl;

	cout << endl;

	cout << "Positive Words Frequency:\n";
	for (WordNode* node = positiveListHead; node != nullptr; node = node->next)
	{
		cout << setw(15) << node->word << " = " << node->count << " times\n";
	}

	cout << endl;

	cout << "Negative Words Frequency:\n";
	for (WordNode* node = negativeListHead; node != nullptr; node = node->next)
	{
		cout << setw(15) << node->word << " = " << node->count << " times\n";
	}

	// combine +ve and -ve linked list
	WordNode* combinedListHead = mergeLists(positiveListHead, negativeListHead);

	// sort the combined list 
	selectionSort(combinedListHead);

	// display result
	cout << "Frequency of each word used in overall reviews (ascending order)\n\n";

	for (WordNode* node = combinedListHead; node != nullptr; node = node->next)
	{
		if (node->count > 0) {
			cout << node->word << " = " << node->count << " times\n";
		}
	}

	// clean up the combined list
	while (combinedListHead != nullptr)
	{
		WordNode* temp = combinedListHead;
		combinedListHead = combinedListHead->next;
		delete temp;
	}
}