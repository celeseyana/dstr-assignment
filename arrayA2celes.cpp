#include "arrays.hpp"
#include <iostream>
#include <fstream>
#include <sstream>

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
// i hate this section

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

        for (int i = 0; i < numRows; ++i)
        {
            delete[] csvData[i];
        }
        delete[] csvData;
    }

    return 0;
}
