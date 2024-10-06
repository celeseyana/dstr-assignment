#include "arrays.hpp"

int main() {
    const int MAX_WORDS = 2000;
    const int MAX_REVIEWS = 20500; // Total reviews in the file
    const int BATCH_SIZE = 5000; // Process reviews in batches of 5000

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

    // Perform sentiment analysis on reviews in batches
    analyzeReviewsInBatches("D:/Github/dstr-assignment/dstr-assignment/tristen/tristen/tristen/required/tripadvisor_hotel_reviews.csv", positiveWords, positiveCount, negativeWords, negativeCount, BATCH_SIZE, MAX_REVIEWS);

    auto end = high_resolution_clock::now();
    auto duration = duration_cast<seconds>(end - start);

    cout << "Time taken: " << duration.count() << " seconds" << endl;

    return 0;
}
