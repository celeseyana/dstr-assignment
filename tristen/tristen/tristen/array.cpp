#include "header.hpp"
#include <cctype> // for isalpha
// Arrays for positive and negative words
string positiveWords[MAX_WORDS];
string negativeWords[MAX_WORDS];
int posWordCount = 0, negWordCount = 0;

// Dynamically allocated arrays for reviews and ratings
string* reviews = nullptr;
int* ratings = nullptr;
int reviewCount = 0;

// Frequency tracking arrays
string frequencyWords[MAX_FREQUENCY_WORDS];
int frequencyCount[MAX_FREQUENCY_WORDS] = { 0 }; // Initialize counts to 0
int frequencySize = 0;  // Initialize the number of unique words found

// Total counts for positive and negative words
int totalPositiveWords = 0;
int totalNegativeWords = 0;

string sanitizeWord(const string& word) {
    string sanitized;
    for (char c : word) {
        // Check if the character is an ASCII character
        if (static_cast<unsigned char>(c) <= 127) { // ASCII range
            // Allow alphabetic characters and apostrophes
            if (isalpha(c) || c == '\'') {
                sanitized += c;
            }
        }
    }
    return sanitized;
}

void loadWords(const string& filename, string arr[], int& count) {
    ifstream file(filename);
    if (!file) {
        cerr << "Could not open the file: " << filename << endl;
        return;
    }

    string word;
    while (file >> word && count < MAX_WORDS) {
        string sanitizedWord = sanitizeWord(word);
        if (sanitizedWord.empty()) {
            continue; // Skip empty sanitized words
        }
        if (isValidWord(sanitizedWord)) {
            arr[count++] = sanitizedWord;
        }
        else {
            cout << "Invalid word detected: " << sanitizedWord << endl; // Debugging line
        }
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


bool isValidWord(const string& word) {
    for (char c : word) {
        // Check if the character is an ASCII character
        if (static_cast<unsigned char>(c) <= 127) { // ASCII range
            // Check if the character is alphabetic or an apostrophe
            if (!isalpha(c) && c != '\'') {
                return false;
            }
        }
        else {
            return false; // Invalid character detected
        }
    }
    return true;
}



double calculateSentimentScore(string review) {
    int positiveCount = 0, negativeCount = 0;
    char* next_token = nullptr;
    char* token = strtok_s(&review[0], " ,.-", &next_token);

    while (token != nullptr) {
        if (isValidWord(token)) {
            updateWordFrequency(token);  // Update frequency
            if (binarySearch(token, positiveWords, posWordCount)) {
                positiveCount++;
            }
            if (binarySearch(token, negativeWords, negWordCount)) {
                negativeCount++;
            }
        }
        token = strtok_s(nullptr, " ,.-", &next_token);
    }

    totalPositiveWords += positiveCount; // Update the total counts
    totalNegativeWords += negativeCount; // Update the total counts

    int rawScore = positiveCount - negativeCount;
    int N = positiveCount + negativeCount;

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

// Function to update word frequency
void updateWordFrequency(const string& word) {
    // Check if the word is a positive word
    if (binarySearch(word, positiveWords, posWordCount)) {
        totalPositiveWords++;  // Increment the total positive words count
    }
    // Check if the word is a negative word
    else if (binarySearch(word, negativeWords, negWordCount)) {
        totalNegativeWords++;  // Increment the total negative words count
    }

    // Track word frequencies only for positive and negative words
    for (int i = 0; i < frequencySize; i++) {
        if (frequencyWords[i] == word) {
            frequencyCount[i]++;
            return;
        }
    }

    // New unique word found
    if (frequencySize < MAX_FREQUENCY_WORDS) {
        frequencyWords[frequencySize] = word;
        frequencyCount[frequencySize] = 1;
        frequencySize++;
    }
}


// Function to analyze and display reviews and their sentiment scores
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

    displayOverallSentiment(); // This should show total counts
    displayTotalCounts(); // This should show frequencies
}


// Function to display the overall sentiment and frequency data
void displayOverallSentiment() {
    cout << "Total Positive Words: " << totalPositiveWords << endl;
    cout << "Total Negative Words: " << totalNegativeWords << endl;
}
//void displayWordFrequency() {
//    // Create temporary arrays to hold indices for sorting positive and negative word frequencies
//    int positiveSortedIndices[MAX_FREQUENCY_WORDS];
//    int negativeSortedIndices[MAX_FREQUENCY_WORDS];
//
//    int posCount = 0;
//    int negCount = 0;
//
//    // Separate the words into positive and negative frequencies
//    for (int i = 0; i < frequencySize; i++) {
//        if (binarySearch(frequencyWords[i], positiveWords, posWordCount)) {
//            positiveSortedIndices[posCount++] = i;
//        }
//        else if (binarySearch(frequencyWords[i], negativeWords, negWordCount)) {
//            negativeSortedIndices[negCount++] = i;
//        }
//    }
//
//    // Sort positive frequencies
//    for (int i = 0; i < posCount - 1; i++) {
//        for (int j = 0; j < posCount - i - 1; j++) {
//            if (frequencyCount[positiveSortedIndices[j]] > frequencyCount[positiveSortedIndices[j + 1]]) {
//                // Swap the indices
//                int temp = positiveSortedIndices[j];
//                positiveSortedIndices[j] = positiveSortedIndices[j + 1];
//                positiveSortedIndices[j + 1] = temp;
//            }
//        }
//    }
//
//    // Sort negative frequencies
//    for (int i = 0; i < negCount - 1; i++) {
//        for (int j = 0; j < negCount - i - 1; j++) {
//            if (frequencyCount[negativeSortedIndices[j]] > frequencyCount[negativeSortedIndices[j + 1]]) {
//                // Swap the indices
//                int temp = negativeSortedIndices[j];
//                negativeSortedIndices[j] = negativeSortedIndices[j + 1];
//                negativeSortedIndices[j + 1] = temp;
//            }
//        }
//    }
//
//    // Output the sorted positive word frequencies
//    cout << "Positive Word Frequencies (Ascending Order):" << endl;
//    for (int i = 0; i < posCount; i++) {
//        int index = positiveSortedIndices[i];
//        cout << frequencyWords[index] << " = " << frequencyCount[index] << " times" << endl;
//    }
//
//    cout << endl; // Separator between positive and negative frequencies
//
//    // Output the sorted negative word frequencies
//    cout << "Negative Word Frequencies (Ascending Order):" << endl;
//    for (int i = 0; i < negCount; i++) {
//        int index = negativeSortedIndices[i];
//        cout << frequencyWords[index] << " = " << frequencyCount[index] << " times" << endl;
//    }
//}

int findIndex(const std::string& word, const std::string frequencyWords[], int size) {
    for (int i = 0; i < size; i++) {
        if (frequencyWords[i] == word) {
            return i;  // Return the index if the word is found
        }
    }
    return -1;  // Return -1 if the word is not found
}

void countWords(const std::string& review) {
    std::istringstream iss(review);
    std::string word;
    while (iss >> word) {
        // Convert word to lowercase or perform other normalizations if necessary

        // Check if the word is in the positive or negative word list
        if (binarySearch(word, positiveWords, posWordCount)) {
            int index = findIndex(word, frequencyWords, frequencySize); // Function to find index
            if (index != -1) {
                frequencyCount[index]++;
            } else {
                // If not found, add it to frequencyWords and initialize count
                frequencyWords[frequencySize] = word;
                frequencyCount[frequencySize] = 1;
                frequencySize++;
            }
        } else if (binarySearch(word, negativeWords, negWordCount)) {
            int index = findIndex(word, frequencyWords, frequencySize); // Function to find index
            if (index != -1) {
                frequencyCount[index]++;
            } else {
                // If not found, add it to frequencyWords and initialize count
                frequencyWords[frequencySize] = word;
                frequencyCount[frequencySize] = 1;
                frequencySize++;
            }
        }
    }
}

void displayTotalCounts() {
    int totalPositive = 0;
    int totalNegative = 0;

    for (int i = 0; i < frequencySize; i++) {
        if (binarySearch(frequencyWords[i], positiveWords, posWordCount)) {
            totalPositive += frequencyCount[i];
        } else if (binarySearch(frequencyWords[i], negativeWords, negWordCount)) {
            totalNegative += frequencyCount[i];
        }
    }

    cout << "Total Positive Words: " << totalPositive << endl;
    cout << "Total Negative Words: " << totalNegative << endl;
}