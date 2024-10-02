// test case 1
// conducted by Joachim Wong Chi Lik (TP065120)
#include "LinkedList.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <iomanip>

using namespace std;


int main()
{
	LinkedList testcase1;
	// declare variables needed
	string line;
	int totalReviews = 0;
	int totalPositive = 0;
	int totalNegative = 0;
	int reviewIndex = 1;

	WordNode* positiveListHead = nullptr;
	WordNode* negativeListHead = nullptr;

	WordNode* reviewPositiveHead = nullptr;
	WordNode* reviewNegativeHead = nullptr;

	// load +ve and -ve words into linked list
	testcase1.loadWords("required/positive-words.txt", positiveListHead);
	testcase1.loadWords("required/negative-words.txt", negativeListHead);

	// load review csv file
	ifstream file("required/tripadvisor_hotel_reviews.csv");
	if (!file.is_open())
	{
		cerr << "Error opening review file" << endl;
		return 1;
	}

	// "skip" the first line (header / title)
	getline(file, line);

	while (getline(file, line))
	{
		// split user review and rating in separate variables
		size_t lastComma = line.find_last_of(',');
		string review = line.substr(0, lastComma - 1);
		string ratingStr = line.substr(lastComma + 1);

		ratingStr.erase(ratingStr.find_last_not_of(" \n\r\t") + 1);

		int userRating = testcase1.convertStringRating(ratingStr);

		// count number of +ve and -ve words in the current review
		int positiveCount = testcase1.countWordsInReview(review, reviewPositiveHead, positiveListHead);
		int negativeCount = testcase1.countWordsInReview(review, reviewNegativeHead, negativeListHead );

		double sentimentScore = testcase1.calculateSentimentScore(positiveCount, negativeCount);

		// Display the details of the current review
		cout << string(50, '-') << endl << endl;
		cout << "Review Index: " << reviewIndex << endl << endl;
		cout << "User Rating: " << userRating << endl;
		cout << "Review: " << review << endl << endl;
		cout << "Positive Words Count = " << positiveCount << endl;
		testcase1.displayWordList("Positive Words", reviewPositiveHead);
		cout << endl;

		cout << "Negative Words Count = " << negativeCount << endl;
		testcase1.displayWordList("Negative Words", reviewNegativeHead);
		cout << endl;

		cout << setprecision(2) << "Sentiment Score (1 - 5) is " << sentimentScore;
		cout << ", thus the rating should be equal to " << round(sentimentScore) << " (" << testcase1.levelOfSentiment(sentimentScore) << ")" << endl;

		cout << endl;
		testcase1.compareRatings(userRating, sentimentScore);
		cout << endl;

		// Clean up temporary linked lists which is
		// storing the +ve and -ve words of the current review
		while (reviewPositiveHead != nullptr) {
			WordNode* temp = reviewPositiveHead;
			reviewPositiveHead = reviewPositiveHead->next;
			delete temp;
		}
		while (reviewNegativeHead != nullptr) {
			WordNode* temp = reviewNegativeHead;
			reviewNegativeHead = reviewNegativeHead->next;
			delete temp;
		}

		totalPositive += positiveCount;
		totalNegative += negativeCount;
		reviewIndex++;
	}

	file.close();

	cout << string(80, '=') << endl << endl;
	testcase1.displaySentimentResult(reviewIndex, totalPositive, totalNegative, positiveListHead, negativeListHead);

	return 0;
}