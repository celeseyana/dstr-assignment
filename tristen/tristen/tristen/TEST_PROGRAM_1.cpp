#include "arrays.hpp"
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