#include "arrays.hpp"

#include "arrays.hpp"

// Function to load words from a file into an array
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

    // Sort the array using insertion sort
    insertionSort(wordArray, count);

    return count;
}

// Function to load reviews from a CSV file into an array
int loadReviews(const char* filename, Review* reviewsArray, int start, int batchSize, int maxReviews) {
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Error: Unable to open " << filename << endl;
        return 0;
    }

    string line;
    int count = 0;

    // Skip the first line (header)
    getline(file, line);

    // Skip lines up to the start position
    for (int i = 0; i < start; i++) {
        if (!getline(file, line)) {
            return count; // Return early if end of file is reached
        }
    }

    // Load the reviews for this batch
    while (getline(file, line) && count < batchSize && start + count < maxReviews) {
        size_t commaPos = line.find_last_of(',');
        if (commaPos == string::npos) {
            cout << "Error parsing line: " << line << endl; // Error handling for bad lines
            continue; // Skip malformed lines
        }
        reviewsArray[count].review = line.substr(0, commaPos);
        reviewsArray[count].rating = stoi(line.substr(commaPos + 1));
        count++;
    }

    file.close();
    return count;
}

void analyzeReviewsInBatches(const char* filename, string* positiveWords, int positiveCount, string* negativeWords, int negativeCount, int batchSize, int maxReviews) {
    Review* reviewsArray = new Review[batchSize]; // Dynamically allocate memory for each batch

    int start = 0;  // Start index for reading reviews
    int reviewsProcessed = 0;  // Counter for total reviews processed

    while (true) {
        int reviewsLoaded = loadReviews(filename, reviewsArray, start, batchSize, maxReviews);

        if (reviewsLoaded == 0) {
            break; // Stop if no more reviews are loaded
        }

        // Analyze the batch of reviews, passing the total processed count
        analyzeReviews(reviewsArray, reviewsLoaded, positiveWords, positiveCount, negativeWords, negativeCount);

        reviewsProcessed += reviewsLoaded; // Increment total processed reviews
        start += reviewsLoaded; // Move to the next batch
    }

    cout << "Total reviews processed: " << reviewsProcessed << endl;

    delete[] reviewsArray; // Free the dynamically allocated memory
}



// Function to swap two strings (avoiding algorithm)
void swapStrings(string& a, string& b) {
    string temp = a;
    a = b;
    b = temp;
}

// Function to sort an array using insertion sort
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

// Function to perform binary search on an array
int binarySearch(string* array, int size, const string& target) {
    int left = 0;
    int right = size - 1;
    while (left <= right) {
        int mid = left + (right - left) / 2;
        if (array[mid] == target) {
            return mid; // Found
        }
        else if (array[mid] < target) {
            left = mid + 1;
        }
        else {
            right = mid - 1;
        }
    }
    return -1; // Not found
}


// Function to count positive and negative words in a review
void countSentimentWords(const string& review, string* positiveWords, int positiveCount,
    string* negativeWords, int negativeCount,
    int* positiveWordFrequency, int* negativeWordFrequency) {
    size_t pos = 0;
    while (pos < review.length()) {
        size_t spacePos = review.find(' ', pos);
        if (spacePos == string::npos) spacePos = review.length();
        string word = review.substr(pos, spacePos - pos);

        // Check for positive words
        int posIndex = binarySearch(positiveWords, positiveCount, word);
        if (posIndex != -1) {
            positiveWordFrequency[posIndex]++;
        }

        // Check for negative words
        int negIndex = binarySearch(negativeWords, negativeCount, word);
        if (negIndex != -1) {
            negativeWordFrequency[negIndex]++;
        }

        pos = spacePos + 1;
    }
}
void printFrequencies(string* words, int count, int* frequencies) {
    cout << "Frequency of Words (in ascending order):" << endl;
    for (int i = 0; i < count; ++i) {
        if (frequencies[i] > 0) {
            cout << words[i] << " = " << frequencies[i] << " times" << endl;
        }
    }
    cout << endl;
}



void analyzeBatch(Review* reviewsArray, int totalReviews, string* positiveWords, int positiveCount,
    string* negativeWords, int negativeCount,
    int& totalPositiveWords, int& totalNegativeWords,
    int* positiveWordFrequency, int* negativeWordFrequency) {
    for (int i = 0; i < totalReviews; i++) {
        int posCount = 0; // To count positive words in the current review
        int negCount = 0; // To count negative words in the current review

        // Track words found in the current review
        string* foundPositiveWords = new string[positiveCount]();
        string* foundNegativeWords = new string[negativeCount]();

        // Count words in the current review
        size_t pos = 0;
        while (pos < reviewsArray[i].review.length()) {
            size_t spacePos = reviewsArray[i].review.find(' ', pos);
            if (spacePos == string::npos) spacePos = reviewsArray[i].review.length();
            string word = reviewsArray[i].review.substr(pos, spacePos - pos);

            // Check for positive words
            int posIndex = binarySearch(positiveWords, positiveCount, word);
            if (posIndex != -1) {
                positiveWordFrequency[posIndex]++;
                foundPositiveWords[posIndex] = word; // Track found positive word
                posCount++; // Increment the positive count
            }

            // Check for negative words
            int negIndex = binarySearch(negativeWords, negativeCount, word);
            if (negIndex != -1) {
                negativeWordFrequency[negIndex]++;
                foundNegativeWords[negIndex] = word; // Track found negative word
                negCount++; // Increment the negative count
            }

            pos = spacePos + 1; // Move to the next word
        }

        // Update the totals
        totalPositiveWords += posCount;
        totalNegativeWords += negCount;

        // Clean up memory for the current review's word tracking
        delete[] foundPositiveWords;
        delete[] foundNegativeWords;
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

// Function to sort the array by frequency using insertion sort
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

void analyzeReviews(Review* reviewsArray, int totalReviews, string* positiveWords, int positiveCount, string* negativeWords, int negativeCount) {
    int totalPositiveWords = 0;
    int totalNegativeWords = 0;
    int totalBatches = 0;  // Initialize totalBatches

    // Dynamically allocate memory for word frequencies
    int* positiveWordFrequency = new int[positiveCount]();  // Initialized to 0
    int* negativeWordFrequency = new int[negativeCount]();  // Initialized to 0

    // Process in batches of 5000 reviews
    for (int batchStart = 0; batchStart < totalReviews; batchStart += 5000) {
        int currentBatchSize = (batchStart + 5000 > totalReviews) ? (totalReviews - batchStart) : 5000;

        // Call the analyzeBatch function to analyze the current batch
        analyzeBatch(&reviewsArray[batchStart], currentBatchSize, positiveWords, positiveCount,
            negativeWords, negativeCount, totalPositiveWords, totalNegativeWords,
            positiveWordFrequency, negativeWordFrequency);

        totalBatches++; // Increment totalBatches for each processed batch
    }

    // After processing all batches, display the overall results
    cout << "Total Batches Processed: " << totalBatches << endl;
    cout << "Total Positive Words Found: " << totalPositiveWords << endl;
    cout << "Total Negative Words Found: " << totalNegativeWords << endl;

    // Print word frequencies for positive words
    cout << "Frequency of Positive Words (in ascending order):" << endl;
    for (int i = 0; i < positiveCount; ++i) {
        if (positiveWordFrequency[i] > 0) {
            cout << positiveWords[i] << " = " << positiveWordFrequency[i] << " times" << endl;
        }
    }
    cout << endl;

    // Print word frequencies for negative words
    cout << "Frequency of Negative Words (in ascending order):" << endl;
    for (int i = 0; i < negativeCount; ++i) {
        if (negativeWordFrequency[i] > 0) {
            cout << negativeWords[i] << " = " << negativeWordFrequency[i] << " times" << endl;
        }
    }
    cout << endl;

    // Clean up memory
    delete[] positiveWordFrequency;
    delete[] negativeWordFrequency;
}







// Function to count total number of reviews in a CSV file
int countTotalReviews(const char* filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Error: Unable to open " << filename << endl;
        return 0;
    }

    string line;
    int count = 0;

    // Skip the first line (header)
    getline(file, line);

    while (getline(file, line)) {
        count++;
    }

    file.close();
    return count;

}

//void analyzeReviewsInBatches(string* positiveWords, int positiveCount, string* negativeWords, int negativeCount, int TOTAL_REVIEWS) {
//    const int BATCH_SIZE = 1000;  // Example batch size
//    Review* reviewsArray = new Review[BATCH_SIZE];  // Array for storing a batch of reviews
//
//    for (int i = 0; i < TOTAL_REVIEWS; i += BATCH_SIZE) {
//        int actualBatchSize = loadReviewsInBatch("D:/Github/dstr-assignment/dstr-assignment/tristen/tristen/tristen/required/tripadvisor_hotel_reviews.csv", reviewsArray, i, BATCH_SIZE);
//
//        if (actualBatchSize == 0) {
//            break;
//        }
//
//        analyzeReviews(reviewsArray, actualBatchSize, positiveWords, positiveCount, negativeWords, negativeCount);
//    }
//
//    delete[] reviewsArray;
//}
