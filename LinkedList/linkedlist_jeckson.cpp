#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>
#include <cctype>

using namespace std;

const int MAX_WORDS = 7000; // Total maximum of 7000 words in positive and negative lists

void loadWords(const char* filename, char words[MAX_WORDS][20], int& count) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Could not open file " << filename << endl;
        return;
    }
    count = 0;
    while (file >> words[count] && count < MAX_WORDS) {
        count++;
    }
    file.close();
}

void toLowerCase(string& word) {
    for (char& c : word) {
        c = tolower(c);
    }
}

bool isWordInList(const char* word, char words[MAX_WORDS][20], int count) {
    for (int i = 0; i < count; i++) {
        if (strcmp(word, words[i]) == 0) {
            return true;
        }
    }
    return false;
}

void removePunctuation(string& word) {
    word.erase(remove_if(word.begin(), word.end(), ::ispunct), word.end());
}

float calculateSentimentScore(int positiveCount, int negativeCount) {
    int N = positiveCount + negativeCount; // Total count of positive and negative words
    int rawScore = positiveCount - negativeCount;
    int minRawScore = -N;
    int maxRawScore = N;

    // Calculate normalized score
    float normalizedScore = static_cast<float>(rawScore - minRawScore) / (maxRawScore - minRawScore);

    // Final sentiment score (1 to 5 scale)
    float sentimentScore = 1 + (4 * normalizedScore);
    return sentimentScore;
}

void analyzeReview(const char* csvFilename, char positiveWords[MAX_WORDS][20], int posCount, char negativeWords[MAX_WORDS][20], int negCount) {
    ifstream file(csvFilename);
    if (!file.is_open()) {
        cerr << "Error: Could not open file " << csvFilename << endl;
        return;
    }

    string line;
    int rowNumber;

    cout << "Enter the row number you want to read (starting from 1): ";
    cin >> rowNumber;
    cout << endl;

    if (rowNumber <= 0) {
        cout << "Invalid row number!" << endl;
        return;
    }

    int currentRow = 0;
    while (getline(file, line)) {
        currentRow++;

        if (currentRow == rowNumber) {
            stringstream ss(line);
            string review, ratingStr;
            int userRating = 0;

            // Extract the review text
            size_t firstQuote = line.find('\"');
            size_t lastQuote = line.rfind('\"');
            if (firstQuote != string::npos && lastQuote != string::npos && firstQuote != lastQuote) {
                review = line.substr(firstQuote + 1, lastQuote - firstQuote - 1);
                ratingStr = line.substr(lastQuote + 2); // Assuming there is a comma and then the rating
                userRating = stoi(ratingStr); // Convert rating to integer
            }
            else {
                cout << "Failed to parse the review and rating correctly." << endl;
                return;
            }

            // Display the entire review and rating
            cout << "Review: " << review << endl << endl;
            cout << "Rating given by user: " << userRating << endl << endl;

            // Analyze the review for positive and negative words
            stringstream reviewStream(review);
            string word;
            int positiveCount = 0, negativeCount = 0;

            cout << "\nAnalyzing review for positive and negative words...\n" << endl << endl;

            while (reviewStream >> word) {
                removePunctuation(word);
                toLowerCase(word);

                if (isWordInList(word.c_str(), positiveWords, posCount)) {
                    positiveCount++;
                }
                else if (isWordInList(word.c_str(), negativeWords, negCount)) {
                    negativeCount++;
                }
            }

            // Display positive and negative words
            if (positiveCount > 0) {
                cout << "Positive Words = " << positiveCount << ":\n";
                reviewStream.clear();
                reviewStream.str(review);
                while (reviewStream >> word) {
                    removePunctuation(word);
                    toLowerCase(word);
                    if (isWordInList(word.c_str(), positiveWords, posCount)) {
                        cout << "- " << word << endl;
                    }
                }
                cout << endl;
            }
            else {
                cout << "\"Positive words\": none" << endl;
            }

            if (negativeCount > 0) {
                cout << "Negative Words = " << negativeCount << ":\n";
                reviewStream.clear();
                reviewStream.str(review);
                while (reviewStream >> word) {
                    removePunctuation(word);
                    toLowerCase(word);
                    if (isWordInList(word.c_str(), negativeWords, negCount)) {
                        cout << "- " << word << endl;
                    }
                }
                cout << endl;
            }
            else {
                cout << "\"Negative words\": none" << endl;
            }

            // Calculate and display the sentiment score
            float sentimentScore = calculateSentimentScore(positiveCount, negativeCount);
            cout << "Sentiment Score (1 - 5) = " << sentimentScore << endl;
            cout << "Rating given by user = " << userRating << endl;

            // Compare with user rating and provide analysis
            cout << "\nAnalysis Output:\n";
            if (userRating == static_cast<int>(round(sentimentScore))) {
                cout << "User's subjective evaluation matches the sentiment score provided by the analysis." << endl;
            }
            else {
                cout << "User's subjective evaluation does not match the sentiment score provided by the analysis. There is an inconsistency between the sentiment score generated by the analysis and the user's evaluation of the sentiment." << endl;
            }

            break; // Exit after processing the selected row
        }
    }

    if (currentRow < rowNumber) {
        cout << "Row number out of range!" << endl;
    }

    file.close();
}

int main() {
    char positiveWords[MAX_WORDS][20];
    char negativeWords[MAX_WORDS][20];
    int posCount, negCount;

    // Load positive and negative words into arrays
    loadWords("positive-words.txt", positiveWords, posCount);
    loadWords("negative-words.txt", negativeWords, negCount);

    // Analyze a specific review and its rating based on user input
    analyzeReview("tripadvisor_hotel_reviews.csv", positiveWords, posCount, negativeWords, negCount);

    return 0;
}
