#include "header.hpp"


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

//----------------------------------------------ONG ZI YANG-----------------------------------------------------------
#include "arrays.hpp"
#include <iostream>
#include <fstream>
#include <cctype>
#include <limits>
#include <string>
#include <cstring>

FileReader::FileReader()
{
    negativeRead = 0;
    positiveRead = 0;
    reviewsRead = 0;
}

void displayMenu()
{
    std::cout << "Select an option to display:\n";
    std::cout << "1. Display Negative Words\n";
    std::cout << "2. Display Positive Words\n";
    std::cout << "3. Display CSV Data\n";
    std::cout << "4. Display Review Data\n";
    std::cout << "5. Exit\n";
}

void FileReader::readNegative(const std::string& filename)
{
    std::ifstream file("D:/Github/dstr-assignment/dstr-assignment/tristen/tristen/tristen/required/negative-words.txt");

    if (file.is_open())
    {
        std::string line;

        while (std::getline(file, line) && negativeRead < array_size)
        {
            negativeWords[negativeRead] = line;
            negativeRead++;
        }
        file.close();
    }
    else
    {
        std::cerr << "Error, file cannot be opened." << std::endl;
    }
};

void FileReader::readPositive(const std::string& filename)
{
    std::ifstream file("D:/Github/dstr-assignment/dstr-assignment/tristen/tristen/tristen/required/positive-words.txt");

    if (file.is_open())
    {
        std::string line;

        while (std::getline(file, line) && positiveRead < array_size)
        {
            positiveWords[positiveRead] = line;
            positiveRead++;
        }
        file.close();
    }
    else
    {
        std::cerr << "Error, file cannot be opened." << std::endl;
    }
};

// start node

int countColumns(const std::string& line)
{
    int count = 0;
    bool insideQuotes = false;

    for (char ch : line)
    {
        if (ch == '\"')
        {
            insideQuotes = !insideQuotes;
        }
        else if (ch == ',' && !insideQuotes)
        {
            count++;
        }
    }
    return count + 1;
}

std::string** readCSV(const std::string& filename, int& numRows, int& numCols)
{
    std::ifstream file(filename);
    std::string line;

    if (file.is_open())
    {
        while (std::getline(file, line))
        {
            numRows++;
            if (numRows == 1)
            {
                numCols = countColumns(line);
            }
        }
        file.clear();
        file.seekg(0);
    }
    else
    {
        std::cerr << "Unable to open file." << std::endl;
        return nullptr;
    }

    std::string** data = new std::string * [numRows];
    for (int i = 0; i < numRows; ++i)
    {
        data[i] = new std::string[numCols];
    }

    int rowIndex = 0;
    if (file.is_open())
    {
        while (std::getline(file, line))
        {
            std::string cell;
            bool insideQuotes = false;
            std::string value;
            int colIndex = 0;

            for (char ch : line)
            {
                if (ch == '\"')
                {
                    insideQuotes = !insideQuotes;
                }
                else if (ch == ',' && !insideQuotes)
                {
                    data[rowIndex][colIndex++] = value;
                    value.clear();
                }
                else
                {
                    value += ch;
                }
            }

            data[rowIndex][colIndex] = value;
            rowIndex++;
        }
        file.close();
    }

    return data;
}

void printCSV(std::string** data, int numRows, int numCols)
{
    for (int i = 0; i < numRows; ++i)
    {
        for (int j = 0; j < numCols; ++j)
        {
            std::cout << data[i][j];
            if (j < numCols - 1)
                std::cout << ", ";
        }
        std::cout << std::endl;
    }
}

void countWordsInRow(const std::string& row, std::string* positiveWords, int positiveSize, std::string* negativeWords, int negativeSize, int& positiveCount, int& negativeCount)
{
    std::string word;

    for (char c : row)
    {
        if (isalnum(static_cast<unsigned char>(c)))
        {
            word += c;
        }
        else if (!word.empty())
        {
            for (int i = 0; i < positiveSize; ++i)
            {
                if (word == positiveWords[i])
                {
                    positiveCount++;
                    break;
                }
            }

            for (int i = 0; i < negativeSize; ++i)
            {
                if (word == negativeWords[i])
                {
                    negativeCount++;
                    break;
                }
            }

            word = "";
        }
    }

    if (!word.empty())
    {
        for (int i = 0; i < positiveSize; ++i)
        {
            if (word == positiveWords[i])
            {
                positiveCount++;
                break;
            }
        }

        for (int i = 0; i < negativeSize; ++i)
        {
            if (word == negativeWords[i])
            {
                negativeCount++;
                break;
            }
        }
    }
}

void trim(std::string& str)
{
    size_t start = 0;
    while (start < str.size() && std::isspace(static_cast<unsigned char>(str[start])))
    {
        start++;
    }
    str.erase(0, start);

    size_t end = str.size();
    while (end > 0 && std::isspace(static_cast<unsigned char>(str[end - 1])))
    {
        end--;
    }
    str.erase(end);
}

void analyzeCSV(std::string** data, int numRows, int numCols, FileReader& reader)
{
    for (int i = 0; i < numRows; ++i)
    {
        int positiveCount = 0;
        int negativeCount = 0;

        std::string trueRating;
        std::string scoreMatch;

        int actualScore = 0;
        if (numCols >= 2)
        {
            std::string scoreStr = data[i][1];
            trim(scoreStr); // rating column wont run no matter what i do bcz i think theres whitespace but this works so
            if (!scoreStr.empty())
            {
                try
                {
                    actualScore = std::stoi(scoreStr);
                }
                catch (const std::invalid_argument&)
                {
                    std::cerr << "Invalid score at row " << i + 1 << ": " << scoreStr << std::endl;
                    actualScore = 0;
                }
                catch (const std::out_of_range&)
                {
                    std::cerr << "Score out of range at row " << i + 1 << ": " << scoreStr << std::endl;
                    actualScore = 0;
                }
            }
            else
            {
                std::cerr << "Empty score at row " << i + 1 << ", defaulting to 0." << std::endl;
                actualScore = 0;
            }
        }

        std::string row = data[i][0];
        for (int j = 0; j < numCols; ++j)
        {
            row += data[i][j] + " ";
        }

        countWordsInRow(row, reader.positiveWords, reader.positiveRead, reader.negativeWords, reader.negativeRead, positiveCount, negativeCount);

        int rawScore = positiveCount + negativeCount;
        int rawSent = positiveCount - negativeCount;
        int minRawScore = -rawScore;
        int maxRawScore = rawScore;

        double normScore = 0.0;
        if (maxRawScore != minRawScore) // no 0
        {
            normScore = (double)(rawSent - minRawScore) / (maxRawScore - minRawScore);
        }

        int ratedScore = 1 + (4 * normScore);
        double trueSentScore = 1 + (4 * normScore);

        if (ratedScore == 1 || ratedScore == 2)
        {
            trueRating = "Negative";
        }
        else if (ratedScore == 4 || ratedScore == 5)
        {
            trueRating = "Positive";
        }
        else
        {
            trueRating = "Neutral";
        };

        if (ratedScore == actualScore)
        {
            scoreMatch = "Score matches!";
        }
        else
        {
            scoreMatch = "Score does not match!";
        }

        std::cout << "Row " << i + 1 << ": Positive words: " << positiveCount << ", Negative words: " << negativeCount << std::endl;
        std::cout << "Sentiment score ( 1-5 ) is " << trueSentScore << ", Rating is " << ratedScore << "(" << trueRating << ")" << std::endl;
        std::cout << "User's given score is: " << actualScore << std::endl; // broken
        std::cout << scoreMatch << std::endl;
    }
}

int totalCount(const std::string& filename)
{
    std::ifstream file(filename);
    std::string line;
    int rowCount = -1;

    if (file.is_open())
    {
        while (std::getline(file, line))
        {
            rowCount++;
        }
        file.close();
    }
    else
    {
        std::cerr << "error!" << filename << std::endl;
    }

    return rowCount;
}

// bubble sort algo
void countWordFrequencies(std::string** data, int numRows, FileReader& reader, int positiveCounts[], int negativeCounts[])
{
    for (int i = 0; i < reader.positiveRead; ++i)
    {
        positiveCounts[i] = 0;
    }

    for (int i = 0; i < reader.negativeRead; ++i)
    {
        negativeCounts[i] = 0;
    }

    for (int i = 0; i < numRows; ++i)
    {
        std::string row;
        for (int j = 0; j < 2; ++j)
        {
            row += data[i][j] + " ";
        }

        std::string word = "";
        for (char c : row)
        {
            if (isalnum(static_cast<unsigned char>(c)))
            {
                word += c;
            }
            else if (!word.empty())
            {
                for (int j = 0; j < reader.positiveRead; ++j)
                {
                    if (word == reader.positiveWords[j])
                    {
                        positiveCounts[j]++;
                    }
                }

                for (int j = 0; j < reader.negativeRead; ++j)
                {
                    if (word == reader.negativeWords[j])
                    {
                        negativeCounts[j]++;
                    }
                }

                word = "";
            }
        }

        if (!word.empty())
        {
            for (int j = 0; j < reader.positiveRead; ++j)
            {
                if (word == reader.positiveWords[j])
                {
                    positiveCounts[j]++;
                }
            }

            for (int j = 0; j < reader.negativeRead; ++j)
            {
                if (word == reader.negativeWords[j])
                {
                    negativeCounts[j]++;
                }
            }
        }
    }
}

void bubbleSort(std::string words[], int counts[], int size)
{
    for (int i = 0; i < size - 1; ++i)
    {
        for (int j = 0; j < size - i - 1; ++j)
        {
            if (counts[j] < counts[j + 1]) // descending
            {
                int tempCount = counts[j];
                counts[j] = counts[j + 1];
                counts[j + 1] = tempCount;

                std::string tempWord = words[j];
                words[j] = words[j + 1];
                words[j + 1] = tempWord;
            }
        }
    }
}

void displayTopWords(std::string words[], int counts[], int size)
{
    for (int i = 0; i < size; ++i)
    {
        std::cout << words[i] << ": " << counts[i] << std::endl;
    }
}
// bubble SORT END

void FileReader::countWordMatches(int& totalpositiveCount, int& totalnegativeCount)
{
    std::ifstream csvFile("D:/Github/dstr-assignment/dstr-assignment/tristen/tristen/tristen/required/tripadvisor_hotel_reviews.csv");
    std::string line;
    std::string word;

    while (std::getline(csvFile, line))
    {
        word = "";
        for (char c : line)
        {
            if (isalnum(static_cast<unsigned char>(c)))
            {
                word += c;
            }
            else if (!word.empty())
            {
                for (int i = 0; i < positiveRead; i++)
                {
                    if (word == positiveWords[i])
                    {
                        totalpositiveCount++;
                    }
                }

                for (int i = 0; i < negativeRead; i++)
                {
                    if (word == negativeWords[i])
                    {
                        totalnegativeCount++;
                    }
                }

                word = "";
            }
        }

        if (!word.empty())
        {
            for (int i = 0; i < positiveRead; i++)
            {
                if (word == positiveWords[i])
                {
                    totalpositiveCount++;
                }
            }

            for (int i = 0; i < negativeRead; i++)
            {
                if (word == negativeWords[i])
                {
                    totalnegativeCount++;
                }
            }
        }
    }

    csvFile.close();
}