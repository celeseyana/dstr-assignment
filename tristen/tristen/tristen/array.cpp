#include "header.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>
#include <iomanip>
#include <map>
// Arrays for positive and negative words
string positiveWords[MAX_WORDS];
string negativeWords[MAX_WORDS];
int posWordCount = 0, negWordCount = 0;

// Dynamically allocated arrays for reviews and ratings
string* reviews = nullptr;
int* ratings = nullptr;
int reviewCount = 0;
string uniqueWords[MAX_WORDS];
int wordFrequencies[MAX_WORDS];
int uniqueWordCount = 0;

// Function to load words from a file
void loadWords(const string& filename, string arr[], int& count) {
    ifstream file(filename);
    string word;
    while (getline(file, word) && count < MAX_WORDS) {
        arr[count++] = word;
    }
    file.close();
}

// Function to dynamically allocate and load all reviews and ratings from CSV
void loadReviews(const string& filename) {
    ifstream file(filename);
    string line;

    // Skip the header
    if (!getline(file, line)) {
        cout << "Error: Could not read header from file." << endl;
        return;
    }

    // Start with a capacity of 1000 reviews
    int capacity = 1000;
    reviews = new string[capacity];
    ratings = new int[capacity];
    reviewCount = 0;

    // Read each line and store the review and rating
    while (getline(file, line)) {
        // Find the last comma to separate the rating from the review
        size_t lastCommaPos = line.find_last_of(',');

        // Check if there's a comma in the line
        if (lastCommaPos == string::npos) {
            cout << "Error: No rating found in line: " << line << endl;
            continue;  // Skip this line
        }

        // Extract the review and rating
        string review = line.substr(0, lastCommaPos);
        string ratingStr = line.substr(lastCommaPos + 1);

        // Trim spaces from rating string
        ratingStr.erase(remove_if(ratingStr.begin(), ratingStr.end(), ::isspace), ratingStr.end());

        // Check if the rating string is not empty
        if (ratingStr.empty()) {
            cout << "Error: Empty rating found in line: " << line << endl;
            continue;  // Skip this line
        }

        try {
            int rating = stoi(ratingStr);  // Convert the rating string to an integer
            reviews[reviewCount] = review;
            ratings[reviewCount] = rating;
            reviewCount++;

            // Resize arrays if capacity is reached
            if (reviewCount == capacity) {
                capacity *= 2;
                string* tempReviews = new string[capacity];
                int* tempRatings = new int[capacity];

                // Copy old data
                for (int i = 0; i < reviewCount; i++) {
                    tempReviews[i] = reviews[i];
                    tempRatings[i] = ratings[i];
                }

                // Delete old arrays
                delete[] reviews;
                delete[] ratings;

                reviews = tempReviews;
                ratings = tempRatings;
            }
        }
        catch (const invalid_argument& e) {
            cout << "Error: Invalid rating found in line: " << line << endl;
            continue;  // Skip this line
        }
        catch (const out_of_range& e) {
            cout << "Error: Rating out of range in line: " << line << endl;
            continue;  // Skip this line
        }
    }

    file.close();
}

// Binary search for word lookup
bool binarySearch(const string& word, const string arr[], int size) {
    int low = 0, high = size - 1;
    while (low <= high) {
        int mid = (low + high) / 2;
        if (arr[mid] == word) return true;
        if (arr[mid] < word) low = mid + 1;
        else high = mid - 1;
    }
    return false;
}

// Function to calculate sentiment score
double calculateSentimentScore(string review) {
    int positiveCount = 0, negativeCount = 0;
    char* next_token = nullptr;
    char* token = strtok_s(&review[0], " ,.-", &next_token);

    while (token != nullptr) {
        if (binarySearch(token, positiveWords, posWordCount)) {
            positiveCount++;
        }
        if (binarySearch(token, negativeWords, negWordCount)) {
            negativeCount++;
        }
        token = strtok_s(nullptr, " ,.-", &next_token);
    }

    int rawScore = positiveCount - negativeCount;
    int N = positiveCount + negativeCount;

    // Prevent division by zero and NaN
    if (N == 0) {
        return 3.0; // Default to neutral if no words found
    }

    int minRawScore = -N, maxRawScore = N;
    double normalizedScore = (double)(rawScore - minRawScore) / (maxRawScore - minRawScore);

    return 1 + 4 * normalizedScore;
}

// Insertion sort function to sort words (if needed)
void insertionSort(string arr[], int size) {
    for (int i = 1; i < size; i++) {
        string key = arr[i];
        int j = i - 1;
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}

void analyzeReviews() {
    for (int i = 0; i < reviewCount; i++) {
        double sentimentScore = calculateSentimentScore(reviews[i]);

        // Output the review index, review text, user rating, and sentiment score
        cout << "Review Index: " << i + 1 << endl; // 1-based index
        cout << "Review: " << reviews[i] << endl; // Full review text
        cout << "User Rating: " << ratings[i] << endl; // User rating
        cout << "Sentiment Score: " << fixed << setprecision(2) << sentimentScore << endl; // Formatted sentiment score
        cout << endl; // Additional space between reviews

        // Sentiment score analysis
        cout << "Sentiment Score Analysis: ";
        if (abs(ratings[i] - sentimentScore) <= 0.5) {
            cout << "Rating matches sentiment analysis." << endl;
        }
        else {
            cout << "Rating does not match sentiment analysis." << endl;
        }
        cout << string(40, '-') << endl; // Separator line for readability
    }
}

int findWordIndex(const string& word) {
    for (int i = 0; i < uniqueWordCount; i++) {
        if (uniqueWords[i] == word) {
            return i;
        }
    }
    return -1; // Word not found
}

// Function to calculate and display the overall sentiment of the reviews
void calculateOverallSentiment() {
    int totalPositiveCount = 0;
    int totalNegativeCount = 0;

    // Traverse through all reviews to count positive/negative words and track their frequencies
    for (int i = 0; i < reviewCount; i++) {
        char* next_token = nullptr;
        char* token = strtok_s(&reviews[i][0], " ,.-", &next_token);

        while (token != nullptr) {
            string word(token);

            // Count positive words
            if (binarySearch(word, positiveWords, posWordCount)) {
                totalPositiveCount++;

                // Find the word in uniqueWords array or add it if not present
                int wordIndex = findWordIndex(word);
                if (wordIndex != -1) {
                    wordFrequencies[wordIndex]++;
                }
                else {
                    uniqueWords[uniqueWordCount] = word;
                    wordFrequencies[uniqueWordCount] = 1;
                    uniqueWordCount++;
                }
            }

            // Count negative words
            if (binarySearch(word, negativeWords, negWordCount)) {
                totalNegativeCount++;

                // Find the word in uniqueWords array or add it if not present
                int wordIndex = findWordIndex(word);
                if (wordIndex != -1) {
                    wordFrequencies[wordIndex]++;
                }
                else {
                    uniqueWords[uniqueWordCount] = word;
                    wordFrequencies[uniqueWordCount] = 1;
                    uniqueWordCount++;
                }
            }

            token = strtok_s(nullptr, " ,.-", &next_token);
        }
    }

    // Output total counts of positive and negative words
    cout << "Total Reviews: " << reviewCount << endl;
    cout << "Total Positive Word Count: " << totalPositiveCount << endl;
    cout << "Total Negative Word Count: " << totalNegativeCount << endl;

    // Sort words by frequency (insertion sort)
    for (int i = 1; i < uniqueWordCount; i++) {
        string keyWord = uniqueWords[i];
        int keyFreq = wordFrequencies[i];
        int j = i - 1;
        while (j >= 0 && wordFrequencies[j] > keyFreq) {
            uniqueWords[j + 1] = uniqueWords[j];
            wordFrequencies[j + 1] = wordFrequencies[j];
            j--;
        }
        uniqueWords[j + 1] = keyWord;
        wordFrequencies[j + 1] = keyFreq;
    }

    // Display word frequencies
    cout << "Word frequencies (ascending):" << endl;
    for (int i = 0; i < uniqueWordCount; i++) {
        cout << uniqueWords[i] << " = " << wordFrequencies[i] << " times" << endl;
    }

    // Display maximum and minimum used words
    if (uniqueWordCount > 0) {
        int maxFrequency = wordFrequencies[uniqueWordCount - 1];
        int minFrequency = wordFrequencies[0];

        // Maximum used word(s)
        cout << "Maximum used word(s): " << endl;
        for (int i = uniqueWordCount - 1; i >= 0 && wordFrequencies[i] == maxFrequency; i--) {
            cout << uniqueWords[i] << " (" << wordFrequencies[i] << " times)" << endl;
        }

        // Minimum used word(s)
        cout << "Minimum used word(s): " << endl;
        for (int i = 0; i < uniqueWordCount && wordFrequencies[i] == minFrequency; i++) {
            cout << uniqueWords[i] << " (" << wordFrequencies[i] << " times)" << endl;
        }
    }
}