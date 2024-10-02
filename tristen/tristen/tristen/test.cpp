#include "arrays.hpp"

// Function to load words from the text file into an array
int loadWords(const char* filename, string* wordArray, int maxWords) {
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Error: Unable to open " << filename << endl;
        return 0;
    }

    string word;
    int count = 0;
    while (file >> word && count < maxWords) {
        wordArray[count] = word;
        count++;
    }
    file.close();

    // Sort the array using selection sort
    insertionSort(wordArray, count);

    return count;
}

// Function to load reviews from CSV file into a linked list
ReviewNode* loadReviews(const char* filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Error: Unable to open " << filename << endl;
        return nullptr;
    }

    string line;
    ReviewNode* head = nullptr;
    ReviewNode* tail = nullptr;

    while (getline(file, line)) {
        size_t commaPos = line.find_last_of(',');
        if (commaPos == string::npos) {
            cout << "Warning: Malformed line found, skipping." << endl;
            continue;
        }

        string review = line.substr(0, commaPos);
        string ratingStr = line.substr(commaPos + 1);
        int rating;
        try {
            rating = stoi(ratingStr);  // Convert the rating part to an integer
        }
        catch (exception& e) {
            cout << "Error: Invalid rating in line: " << line << endl;
            continue;  // Skip lines with invalid ratings
        }

        ReviewNode* newNode = new ReviewNode{ review, rating, nullptr };

        if (head == nullptr) {
            head = newNode;
            tail = newNode;
        }
        else {
            tail->next = newNode;
            tail = newNode;
        }
    }

    file.close();
    return head;
}

// Self-built function to swap two strings
void swapStrings(string& a, string& b) {
    string temp = a;
    a = b;
    b = temp;
}

// Function to sort the array using selection sort
void insertionSort(string* arr, int size) {
    for (int i = 1; i < size; i++) {
        string key = arr[i];
        int j = i - 1;

        // Move elements of arr[0..i-1], that are greater than key, to one position ahead of their current position
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}

// Function to perform binary search on the array
int binarySearch(string* arr, int size, const string& word) {
    int low = 0, high = size - 1;
    while (low <= high) {
        int mid = (low + high) / 2;
        if (arr[mid] == word) {
            return mid;
        }
        else if (arr[mid] < word) {
            low = mid + 1;
        }
        else {
            high = mid - 1;
        }
    }
    return -1;
}

// Function to count occurrences of positive and negative words in a review
void countSentimentWords(const string& review, string* positiveWords, int positiveCount, string* negativeWords, int negativeCount, int& positiveCountInReview, int& negativeCountInReview, string* foundPositiveWords, string* foundNegativeWords) {
    positiveCountInReview = 0;
    negativeCountInReview = 0;

    size_t pos = 0;
    string word;

    while (pos < review.length()) {
        size_t spacePos = review.find(' ', pos);
        if (spacePos == string::npos) spacePos = review.length();
        word = review.substr(pos, spacePos - pos);

        // Check for positive words
        int posIndex = binarySearch(positiveWords, positiveCount, word);
        if (posIndex != -1) {
            foundPositiveWords[positiveCountInReview++] = word;  // Store found positive word
        }

        // Check for negative words
        int negIndex = binarySearch(negativeWords, negativeCount, word);
        if (negIndex != -1) {
            foundNegativeWords[negativeCountInReview++] = word;  // Store found negative word
        }

        pos = spacePos + 1;
    }
}


// Function to calculate and normalize the sentiment score
double calculateSentimentScore(int positiveCount, int negativeCount, int maxCount) {
    int rawScore = positiveCount - negativeCount;
    int minRawScore = -maxCount;
    int maxRawScore = maxCount;

    double normalizedScore = (double)(rawScore - minRawScore) / (maxRawScore - minRawScore);
    double sentimentScore = 1 + (4 * normalizedScore);  // Scale to 1-5
    return sentimentScore;
}

// Function to compare sentiment score with user rating and output analysis
void analyzeReviews(ReviewNode* reviews, string* positiveWords, int positiveCount, string* negativeWords, int negativeCount) {
    ReviewNode* current = reviews;

    int totalReviews = 0;
    int totalPositiveWords = 0;
    int totalNegativeWords = 0;

    // Allocate arrays for found words
    string* foundPositiveWords = new string[positiveCount];
    string* foundNegativeWords = new string[negativeCount];

    while (current != nullptr) {
        int posCount, negCount;
        // Count sentiment words and store found words
        countSentimentWords(current->review, positiveWords, positiveCount, negativeWords, negativeCount, posCount, negCount, foundPositiveWords, foundNegativeWords);

        totalReviews++;
        totalPositiveWords += posCount;
        totalNegativeWords += negCount;

        int maxCount = posCount + negCount;
        if (maxCount == 0) {
            cout << "Review: " << current->review << endl << endl;
            cout << "No positive or negative words found." << endl << endl;
        }
        else {
            double sentimentScore = calculateSentimentScore(posCount, negCount, maxCount);
            cout << "Review: " << current->review << endl << endl;
            cout << "User Rating: " << current->rating << " | Calculated Sentiment Score: " << sentimentScore << endl << endl;

            // Compare the sentiment score with user rating
            if (int(sentimentScore) == current->rating) {
                cout << "The sentiment matches the user's rating." << endl;
            }
            else {
                cout << "There is a mismatch between the sentiment and the user's rating." << endl;
            }

            // Print found positive words
            cout << "Positive Words Found: ";
            for (int i = 0; i < posCount; i++) {
                cout << foundPositiveWords[i] << (i < posCount - 1 ? ", " : "");
            }
            cout << endl <<endl;

            // Print found negative words
            cout << "Negative Words Found: ";
            for (int i = 0; i < negCount; i++) {
                cout << foundNegativeWords[i] << (i < negCount - 1 ? ", " : "");
            }
            cout << endl;
        }
        cout << "---------------------------" << endl<< endl;
        current = current->next;
    }

    // Print total counts
    cout << "Total Reviews Analyzed: " << totalReviews << endl;
    cout << "Total Positive Words: " << totalPositiveWords << endl;
    cout << "Total Negative Words: " << totalNegativeWords << endl;

    // Free the allocated memory
    delete[] foundPositiveWords;
    delete[] foundNegativeWords;
}


// Function to delete the linked list and free memory
void deleteReviews(ReviewNode* head) {
    while (head != nullptr) {
        ReviewNode* temp = head;
        head = head->next;
        delete temp;
    }

}

int main() {
    const int MAX_WORDS = 2000;
    string positiveWords[MAX_WORDS];
    string negativeWords[MAX_WORDS];

    auto start = high_resolution_clock::now();

    // Load positive and negative words
    int positiveCount = loadWords("D:/Github/dstr-assignment/dstr-assignment/tristen/tristen/tristen/required/positive-words.txt", positiveWords, MAX_WORDS);
    int negativeCount = loadWords("D:/Github/dstr-assignment/dstr-assignment/tristen/tristen/tristen/required/negative-words.txt", negativeWords, MAX_WORDS);

    // Validate if words are loaded correctly
    if (positiveCount == 0 || negativeCount == 0) {
        cout << "Error: Failed to load sentiment words." << endl;
        return 1;
    }

    // Load reviews from CSV file
    ReviewNode* reviews = loadReviews("D:/Github/dstr-assignment/dstr-assignment/tristen/tristen/tristen/required/tripadvisor_hotel_reviews.csv");
    if (reviews == nullptr) {
        cout << "Error: Failed to load reviews." << endl;
        return 1;
    }

    // Perform sentiment analysis and comparison with user ratings
    analyzeReviews(reviews, positiveWords, positiveCount, negativeWords, negativeCount);

    // Clean up linked list
    deleteReviews(reviews);

    auto end = high_resolution_clock::now();
    auto duration = duration_cast<seconds>(end - start);

    cout << "Time taken: " << duration.count() << "seconds" << endl;

    return 0;
}
