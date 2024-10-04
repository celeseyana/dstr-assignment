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
// Function to load reviews from CSV file into a linked list
ReviewNode* loadReviews(const char* filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Error: Unable to open " << filename << endl;
        return nullptr;
    }

    string line;

    // Skip the first line (header)
    getline(file, line);

    ReviewNode* head = nullptr;
    ReviewNode* tail = nullptr;

    while (getline(file, line)) {
        size_t commaPos = line.find_last_of(',');
        string review = line.substr(0, commaPos);
        int rating = stoi(line.substr(commaPos + 1));

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
void insertionSortByFrequency(string words[], int freq[], int count) {
    for (int i = 1; i < count; ++i) {
        int keyFreq = freq[i];
        string keyWord = words[i];
        int j = i - 1;

        // Sort by frequency in ascending order
        while (j >= 0 && freq[j] > keyFreq) {
            freq[j + 1] = freq[j];
            words[j + 1] = words[j];
            --j;
        }
        freq[j + 1] = keyFreq;
        words[j + 1] = keyWord;
    }
}
// Function to compare sentiment score with user rating and output analysis
void analyzeReviews(ReviewNode* reviews, string* positiveWords, int positiveCount, string* negativeWords, int negativeCount) {
    ReviewNode* current = reviews;
    int totalReviews = 0;
    int totalPositiveWords = 0;
    int totalNegativeWords = 0;

    // Dynamically allocate memory for word frequencies
    int* positiveWordFrequency = new int[positiveCount](); // Initialized to 0
    int* negativeWordFrequency = new int[negativeCount](); // Initialized to 0

    while (current != nullptr) {
        int posCount, negCount;

        // Track words found in each review
        string* foundPositiveWords = new string[positiveCount]();
        string* foundNegativeWords = new string[negativeCount]();

        countSentimentWords(current->review, positiveWords, positiveCount, negativeWords, negativeCount, posCount, negCount, foundPositiveWords, foundNegativeWords);

        // For every word in the current review
        size_t pos = 0;
        while (pos < current->review.length()) {
            size_t spacePos = current->review.find(' ', pos);
            if (spacePos == string::npos) spacePos = current->review.length();
            string word = current->review.substr(pos, spacePos - pos);

            // Check if word is positive
            int posIndex = binarySearch(positiveWords, positiveCount, word);
            if (posIndex != -1) {
                positiveWordFrequency[posIndex]++;
                foundPositiveWords[posIndex] = word;
            }

            // Check if word is negative
            int negIndex = binarySearch(negativeWords, negativeCount, word);
            if (negIndex != -1) {
                negativeWordFrequency[negIndex]++;
                foundNegativeWords[negIndex] = word;
            }

            pos = spacePos + 1;
        }

        totalPositiveWords += posCount;
        totalNegativeWords += negCount;
        totalReviews++;

        // Display analysis for the current review
        cout << "Review: " << current->review << endl << endl;
        cout << "User Rating: " << current->rating << endl;

        // Calculate sentiment score and print results as before
        int maxCount = posCount + negCount;
        if (maxCount == 0) {
            cout << "No positive or negative words found." << endl << endl;
        }
        else {
            double sentimentScore = calculateSentimentScore(posCount, negCount, maxCount);
            cout << "Calculated Sentiment Score: " << sentimentScore << endl;
            if (int(sentimentScore) == current->rating) {
                cout << "The sentiment matches the user's rating." << endl << endl;
            }
            else {
                cout << "There is a mismatch between the sentiment and the user's rating." << endl << endl;
            }
        }

        cout << "Positive words in this review: ";
        bool isFirst = true;
        for (int i = 0; i < positiveCount; ++i) {
            if (!foundPositiveWords[i].empty()) {
                if (!isFirst)
                {
                    cout << ", ";
                }
                cout << foundPositiveWords[i];
                isFirst = false;
            }
        }
        cout << endl;

        cout << "Negative words in this review: ";
        for (int i = 0; i < negativeCount; ++i) {
            if (!foundNegativeWords[i].empty()) {
                if (!isFirst) 
                {
                    cout << ", ";
                }
                cout << foundNegativeWords[i];
                isFirst = false;  
            }
        }

        cout << endl;
        cout << "----------------------------------------------------------------------" << endl << endl;

        // Clean up memory for the current review's word tracking
        delete[] foundPositiveWords;
        delete[] foundNegativeWords;

        current = current->next;
    }

    // Print total reviews, positive, and negative words
    cout << "Total Reviews Analyzed: " << totalReviews << endl;
    cout << "Total Positive Words Found: " << totalPositiveWords << endl;
    cout << "Total Negative Words Found: " << totalNegativeWords << endl;

    // Print word frequencies in ascending order
    cout << "Frequency of Positive Words (in ascending order):" << endl;
    for (int i = 0; i < positiveCount; ++i) {
        if (positiveWordFrequency[i] > 0) {
            for (int j = i + 1; j < positiveCount; ++j) {
                if (positiveWordFrequency[j] < positiveWordFrequency[i] && positiveWordFrequency[j] > 0) {
                    // Swap frequencies and words
                    int tempFreq = positiveWordFrequency[i];
                    positiveWordFrequency[i] = positiveWordFrequency[j];
                    positiveWordFrequency[j] = tempFreq;

                    string tempWord = positiveWords[i];
                    positiveWords[i] = positiveWords[j];
                    positiveWords[j] = tempWord;
                }
            }
            cout << positiveWords[i] << " = " << positiveWordFrequency[i] << " times" << endl;
        }
    }
    cout << endl;

    // Print word frequencies in ascending order for negative words
    cout << "Frequency of Negative Words (in ascending order):" << endl;
    for (int i = 0; i < negativeCount; ++i) {
        if (negativeWordFrequency[i] > 0) {
            for (int j = i + 1; j < negativeCount; ++j) {
                if (negativeWordFrequency[j] < negativeWordFrequency[i] && negativeWordFrequency[j] > 0) {
                    // Swap frequencies and words
                    int tempFreq = negativeWordFrequency[i];
                    negativeWordFrequency[i] = negativeWordFrequency[j];
                    negativeWordFrequency[j] = tempFreq;

                    string tempWord = negativeWords[i];
                    negativeWords[i] = negativeWords[j];
                    negativeWords[j] = tempWord;
                }
            }
            cout << negativeWords[i] << " = " << negativeWordFrequency[i] << " times" << endl;
        }
    }
    cout << endl;
    cout << endl;
    // Clean up memory
    delete[] positiveWordFrequency;
    delete[] negativeWordFrequency;
}


// Function to delete the linked list and free memory
void deleteReviews(ReviewNode* head) {
    while (head != nullptr) {
        ReviewNode* temp = head;
        head = head->next;
        delete temp;
    }

}


