// Test case 2
// Conducted by Jeckson Liew Khen Siong TP064611
#include "LinkedList.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <iomanip>

using namespace std;

int main()
{
    LinkedList testcase2;
    string line;
    char continueChoice;

    do {
        // Reset variables for each loop iteration
        int totalReviews = 0;
        int totalPositive = 0;
        int totalNegative = 0;
        int reviewIndex = 1;

        WordNode* positiveListHead = nullptr;
        WordNode* negativeListHead = nullptr;
        WordNode* reviewPositiveHead = nullptr;
        WordNode* reviewNegativeHead = nullptr;

        // Measure time to load positive and negative words into linked lists
        testcase2.measureExecutionTime([&]() {
            testcase2.loadWords("required/positive-words.txt", positiveListHead);
            testcase2.loadWords("required/negative-words.txt", negativeListHead);
            });

        int choice;
        // Use displayMenu from LinkedList class
        testcase2.displayMenu();
        cin >> choice;

        // Variable to store processing time
        double processingTime = 0.0;

        // Open the review CSV file for this iteration
        ifstream file("required/tripadvisor_hotel_reviews.csv");
        if (!file.is_open()) {
            cerr << "Error opening review file" << endl;
            return 1;
        }

        // "Skip" the first line (header / title)
        getline(file, line);

        switch (choice) {
        case 1: {
            // Option 1: Process all reviews
            processingTime = testcase2.measureExecutionTime([&]() {
                while (getline(file, line)) {
                    size_t lastComma = line.find_last_of(',');
                    if (lastComma == string::npos) {
                        cerr << "Invalid line format: " << line << endl;
                        continue;
                    }

                    string review = line.substr(0, lastComma - 1);
                    string ratingStr = line.substr(lastComma + 1);

                    ratingStr.erase(ratingStr.find_last_not_of(" \n\r\t") + 1);

                    int userRating = testcase2.convertStringRating(ratingStr);

                    int positiveCount = testcase2.countWordsInReview(review, reviewPositiveHead, positiveListHead);
                    int negativeCount = testcase2.countWordsInReview(review, reviewNegativeHead, negativeListHead);

                    double sentimentScore = testcase2.calculateSentimentScore(positiveCount, negativeCount);

                    // Display the details of the selected review
                    cout << string(50, '-') << endl << endl;
                    cout << "No." << reviewIndex << endl << endl;
                    cout << "User Rating: " << userRating << endl << endl;
                    cout << "Review: " << review << endl << endl;
                    cout << string(50, '-') << endl << endl;
                    cout << "Positive Words Count = " << positiveCount << endl << endl;
                    testcase2.displayWordList("Info", reviewPositiveHead);
                    cout << endl;

                    cout << "Negative Words Count = " << negativeCount << endl << endl;
                    testcase2.displayWordList("Info", reviewNegativeHead);
                    cout << endl;

                    cout << setprecision(2) << "Sentiment Score (1 - 5) is " << sentimentScore;
                    cout << ", thus the rating should be equal to " << round(sentimentScore) << " (" << testcase2.levelOfSentiment(sentimentScore) << ")" << endl;

                    cout << endl;
                    testcase2.compareRatings(userRating, sentimentScore);
                    cout << endl;

                    // Clean up temporary linked lists for this review
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
                });
            break;
        }
        case 2: {
            // Option 2: Process a specific row
            int rowNumber;
            cout << "Enter the row number you want to analyze: ";
            cin >> rowNumber;
            cout << endl;

            processingTime = testcase2.measureExecutionTime([&]() {
                int currentRow = 0;
                bool rowFound = false;  // Debugging flag to check if row was found

                while (getline(file, line)) {
                    currentRow++;
                    if (currentRow == rowNumber) {
                        size_t lastComma = line.find_last_of(',');
                        if (lastComma == string::npos) {
                            cerr << "Invalid line format: " << line << endl;
                            break;
                        }

                        string review = line.substr(0, lastComma - 1);
                        string ratingStr = line.substr(lastComma + 1);

                        ratingStr.erase(ratingStr.find_last_not_of(" \n\r\t") + 1);

                        int userRating = testcase2.convertStringRating(ratingStr);

                        int positiveCount = testcase2.countWordsInReview(review, reviewPositiveHead, positiveListHead);
                        int negativeCount = testcase2.countWordsInReview(review, reviewNegativeHead, negativeListHead);

                        double sentimentScore = testcase2.calculateSentimentScore(positiveCount, negativeCount);

                        // Display the details of the selected review
                        cout << string(50, '-') << endl << endl;
                        cout << "No. " << currentRow << endl << endl;
                        cout << "User Rating: " << userRating << endl << endl;
                        cout << "Review: " << review << endl << endl;
                        cout << string(50, '-') << endl << endl;
                        cout << "Positive Words Count = " << positiveCount << endl << endl;
                        testcase2.displayWordList("Info", reviewPositiveHead);
                        cout << endl;

                        cout << "Negative Words Count = " << negativeCount << endl << endl;
                        testcase2.displayWordList("Info", reviewNegativeHead);
                        cout << endl;

                        cout << setprecision(2) << "Sentiment Score (1 - 5) is " << sentimentScore;
                        cout << ", thus the rating should be equal to " << round(sentimentScore) << " (" << testcase2.levelOfSentiment(sentimentScore) << ")" << endl;

                        cout << endl;
                        testcase2.compareRatings(userRating, sentimentScore);
                        cout << endl;

                        // Clean up temporary linked lists for this review
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
                        rowFound = true;  // Set flag to true since row was found
                        break;  // Exit the loop after processing the specified row
                    }
                }

                if (!rowFound) {
                    cerr << "Row number " << rowNumber << " not found in the file." << endl;
                }
                });
            break;
        }
        case 3:
            cout << "Thank you for using the program!\nSee you again!" << endl;
            file.close();
            return 0;

        default:
            cout << "Invalid choice! Please run the program again and select a valid option." << endl;
            file.close();
            return 1;
        }

        file.close();  // Close the file to reset the stream

        if (choice == 1) {
            cout << string(80, '=') << endl << endl;
            testcase2.displaySentimentResult(reviewIndex, totalPositive, totalNegative, positiveListHead, negativeListHead);
        }

        // Print the processing time
        if (processingTime >= 60) {
            double timeInMinutes = processingTime / 60;
            cout << "\nTime used: " << timeInMinutes << " minutes" << endl << endl;
        }
        else {
            cout << "\nTime used: " << processingTime << " seconds" << endl << endl;
        }

        // Ask the user if they want to continue
        cout << "Do you want to continue? (y/n): ";
        cin >> continueChoice;

    } while (continueChoice == 'y' || continueChoice == 'Y');

 
    cout << "\nProgram ended. Goodbye!" << endl;
    

    return 0;
}