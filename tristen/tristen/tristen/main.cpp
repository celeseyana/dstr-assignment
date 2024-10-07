#include "header.hpp"
#include <iostream>

int main() {
    // Load positive and negative word lists
    loadWords("D:/Github/dstr-assignment/dstr-assignment/tristen/tristen/tristen/required/positive-words.txt", positiveWords, posWordCount);
    loadWords("D:/Github/dstr-assignment/dstr-assignment/tristen/tristen/tristen/required/negative-words.txt", negativeWords, negWordCount);

    // Sort the word lists using insertion sort for binary search efficiency
    insertionSort(positiveWords, posWordCount);
    insertionSort(negativeWords, negWordCount);

    // Load the reviews and ratings from the CSV file
    loadReviews("D:/Github/dstr-assignment/dstr-assignment/tristen/tristen/tristen/required/tripadvisor_hotel_reviews.csv");

    // Call the analyzeReviews function
    analyzeReviews();

    // Clean up dynamic arrays
    delete[] reviews;
    delete[] ratings;

    return 0;
}
