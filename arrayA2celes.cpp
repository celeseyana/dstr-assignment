#include "arrays.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cctype>

FileReader::FileReader()
{
    negativeRead = 0;
    positiveRead = 0;
    reviewsRead = 0;
}

void FileReader::readNegative(const std::string &filename)
{
    std::ifstream file("C:/Github/dstr-assignment/required/negative-words.txt");

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
    std::ifstream file("C:/Github/dstr-assignment/required/positive-words.txt");

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
                break; // kill this mf process
            }
        }

        for (int i = 0; i < negativeSize; ++i) // THIS GUY IS THE LINEAR SEARCH
        {
            if (word == negativeWords[i])
            {
                negativeCount++;
                break; // kill this mf process
            }
        }
    }
}

void trim(std::string &str)
{
    str.erase(str.begin(), std::find_if(str.begin(), str.end(), [](unsigned char ch)
                                        { return !std::isspace(ch); }));
    str.erase(std::find_if(str.rbegin(), str.rend(), [](unsigned char ch)
                           { return !std::isspace(ch); })
                  .base(),
              str.end());
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

int main()
{
    int numRows = 0;
    int numCols = 0;

    FileReader reader;

    reader.readNegative("C:/Github/dstr-assignment/required/negative-words.txt");
    reader.readPositive("C:/Github/dstr-assignment/required/positive-words.txt");

    std::cout << "Negative Words:" << std::endl;
    for (int i = 0; i < reader.negativeRead; i++)
    {
        std::cout << reader.negativeWords[i] << std::endl;
    }

    std::cout << "\nPositive Words:" << std::endl;
    for (int i = 0; i < reader.positiveRead; i++)
    {
        std::cout << reader.positiveWords[i] << std::endl;
    }

    std::string **csvData = readCSV("C:/Github/dstr-assignment/required/tripadvisor_hotel_reviews.csv", numRows, numCols);

    if (csvData != nullptr)
    {
        std::cout << "CSV Data (" << numRows << " rows, " << numCols << " columns):" << std::endl;
        printCSV(csvData, numRows, numCols);

        analyzeCSV(csvData, numRows, numCols, reader);

        for (int i = 0; i < numRows; ++i)
        {
            delete[] csvData[i];
        }
        delete[] csvData;
    }

    return 0;
}
