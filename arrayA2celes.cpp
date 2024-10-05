#include "arrays.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
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

void FileReader::readNegative(const std::string &filename)
{
    std::ifstream file("D:/Github/dstr-assignment/required/negative-words.txt");

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

void FileReader::readPositive(const std::string &filename)
{
    std::ifstream file("D:/Github/dstr-assignment/required/positive-words.txt");

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

int countColumns(const std::string &line)
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

std::string **readCSV(const std::string &filename, int &numRows, int &numCols)
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

    std::string **data = new std::string *[numRows];
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

void printCSV(std::string **data, int numRows, int numCols)
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

void countWordsInRow(const std::string &row, std::string *positiveWords, int positiveSize, std::string *negativeWords, int negativeSize, int &positiveCount, int &negativeCount) // THIS GUY IS THE LINEAR SEARCH
{
    std::istringstream stream(row);
    std::string word;

    while (stream >> word)
    {
        for (int i = 0; i < positiveSize; ++i) // THIS GUY IS THE LINEAR SEARCH
        {
            if (word == positiveWords[i])
            {
                positiveCount++;
                break; // kill process
            }
        }

        for (int i = 0; i < negativeSize; ++i) // THIS GUY IS THE LINEAR SEARCH
        {
            if (word == negativeWords[i])
            {
                negativeCount++;
                break; // kill process
            }
        }
    }
}

void trim(std::string &str)
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

void analyzeCSV(std::string **data, int numRows, int numCols, FileReader &reader)
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
                catch (const std::invalid_argument &)
                {
                    std::cerr << "Invalid score at row " << i + 1 << ": " << scoreStr << std::endl;
                    actualScore = 0;
                }
                catch (const std::out_of_range &)
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

int totalCount(const std::string &filename)
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
void countWordFrequencies(std::string **data, int numRows, FileReader &reader, int positiveCounts[], int negativeCounts[])
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

        std::istringstream stream(row);
        std::string word;

        while (stream >> word)
        {
            // pos check
            for (int j = 0; j < reader.positiveRead; ++j)
            {
                if (word == reader.positiveWords[j])
                {
                    positiveCounts[j]++;
                }
            }

            // neg check
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

void FileReader::countWordMatches(int &totalpositiveCount, int &totalnegativeCount)
{
    std::ifstream csvFile("D:/Github/dstr-assignment/required/tripadvisor_hotel_reviews.csv");
    std::string line;

    while (std::getline(csvFile, line))
    {
        std::istringstream ss(line);
        std::string word;

        while (ss >> word)
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

int main()
{
    int numRows = 0;
    int numCols = 0;

    FileReader reader;

    reader.readNegative("D:/Github/dstr-assignment/required/negative-words.txt");
    reader.readPositive("D:/Github/dstr-assignment/required/positive-words.txt");
    int *positiveCounts = new int[reader.positiveRead];
    int *negativeCounts = new int[reader.negativeRead];

    std::string filename;

    filename = "D:/Github/dstr-assignment/required/tripadvisor_hotel_reviews.csv";
    int reviewNumber = totalCount(filename);

    filename = "D:/Github/dstr-assignment/required/positive-words.txt";
    int totalPos = totalCount(filename);

    filename = "D:/Github/dstr-assignment/required/negative-words.txt";
    int totalNeg = totalCount(filename);

    std::string **csvData = readCSV("D:/Github/dstr-assignment/required/tripadvisor_hotel_reviews.csv", numRows, numCols);
    countWordFrequencies(csvData, numRows, reader, positiveCounts, negativeCounts);

    int choice = 0;

    int totalpositiveCount = 0;
    int totalnegativeCount = 0;
    reader.countWordMatches(totalpositiveCount, totalnegativeCount);

    while (true)
    {
        displayMenu();
        std::cout << "Enter your choice (1-5): ";
        std::cin >> choice;

        while (std::cin.fail() || choice < 1 || choice > 6)
        {
            std::cout << "Invalid choice. Please enter a number between 1 and 6: ";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cin >> choice;
        }

        switch (choice)
        {
        case 1:
            std::cout << "Negative Words:\n";
            for (int i = 0; i < reader.negativeRead; i++)
            {
                std::cout << reader.negativeWords[i] << std::endl;
            }
            break;
        case 2:
            std::cout << "\nPositive Words:\n";
            for (int i = 0; i < reader.positiveRead; i++)
            {
                std::cout << reader.positiveWords[i] << std::endl;
            }
            break;
        case 3:
            if (csvData != nullptr)
            {
                std::cout << "CSV Data (" << numRows << " rows, " << numCols << " columns):\n";
                printCSV(csvData, numRows, numCols);
                analyzeCSV(csvData, numRows, numCols, reader);
            }
            break;
        case 4:
            std::cout << "\nTotal Review Count: " << reviewNumber << std::endl;
            std::cout << "\nTotal Positive Words: " << reader.positiveRead << std::endl;
            std::cout << "\nTotal Negative Words: " << reader.negativeRead << std::endl;
            std::cout << "\nTotal flagged positive word in reviews: " << totalpositiveCount << std::endl;
            std::cout << "\nTotal flagged negative word reviews: " << totalnegativeCount << std::endl;

            bubbleSort(reader.positiveWords, positiveCounts, reader.positiveRead);
            std::cout << "\nTop 10 Positive Words:\n";
            displayTopWords(reader.positiveWords, positiveCounts, std::min(10, reader.positiveRead));

            bubbleSort(reader.negativeWords, negativeCounts, reader.negativeRead);
            std::cout << "\nTop 10 Negative Words:\n";
            displayTopWords(reader.negativeWords, negativeCounts, std::min(10, reader.negativeRead));
            break;
        case 5:
            std::cout << "Exiting.\n";
            return 0;
        default:
            break;
        }

        std::cout << "\n";
    }
    return 0;
}
