#ifndef READFILE_HPP
#define READFILE_HPP
using namespace std;
#include <string>

class FileReader
{
public:
    static const int array_size = 5000;
    std::string negativeWords[array_size];
    std::string positiveWords[array_size];
    int negativeRead = 0;
    int positiveRead = 0;

    FileReader();

    void readNegative(const std::string &filename);
    void readPositive(const std::string &filename);
};

#endif