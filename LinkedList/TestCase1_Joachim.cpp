// test case 1
// conducted by Joachim Wong Chi Lik (TP065120)
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


int main()
{
	LinkedList testcase1;

	WordNode* positiveListHead = nullptr;
	WordNode* negativeListHead = nullptr;
	ReviewNode* reviewHead = nullptr;

	WordNode* reviewPositiveHead = nullptr;
	WordNode* reviewNegativeHead = nullptr;

	auto start = high_resolution_clock::now(); // start the timer

	// load +ve and -ve words into linked list
	testcase1.loadWords("required/positive-words.txt", positiveListHead);
	testcase1.loadWords("required/negative-words.txt", negativeListHead);
	testcase1.loadReviews("required/tripadvisor_hotel_reviews.csv", reviewHead);

	testcase1.processReviews(reviewHead, positiveListHead, negativeListHead);

	auto stop = high_resolution_clock::now(); // stop the timer

	auto durationMili = duration_cast<seconds>(stop - start);
	auto duration = duration_cast<minutes>(stop - start);

	cout << "Time Execution Took: " << durationMili.count() << " seconds." << endl;
	cout << "Time Execution Took: " << duration.count() << " minutes." << endl;

	return 0;
}