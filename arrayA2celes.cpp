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

// void FileReader::readReviews(const std::string &filename)
// {
//     std::ifstream file("C:/Github/dstr-assignment/required/tripadvisor_hotel_reviews.csv");
// }

int main()
{
    FileReader reader;

    reader.readNegative("C:/Github/dstr-assignment/required/negative-words.txt");
    reader.readPositive("C:/Github/dstr-assignment/required/positive-words.txt");
    reader.readReviews("C:/Github/dstr-assignment/required/tripadvisor_hotel_reviews.csv");

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

    std::cout << "\nReviews:" << std::endl;
    for (int i = 0; i < reader.reviewsRead; i++)
    {
        std::cout << "Reviews: " << reader.reviews[i] << ", Rating: " << reader.ratings[i] << std::endl;
    }

    return 0;
}
