#ifndef READFILE_HPP
#define READFILE_HPP
using namespace std;
#include <string>

class FileReader
{
public:
    static const int array_size = 5000;
    static const int csv_array_size = 21000;
    std::string negativeWords[array_size];
    std::string positiveWords[array_size];
    std::string reviews[csv_array_size];
    int ratings[csv_array_size];

    int negativeRead = 0;
    int positiveRead = 0;
    int reviewsRead = 0;

    FileReader();

    void readNegative(const std::string &filename);
    void readPositive(const std::string &filename);

    std::string **readCSV(const std::string &filename, int &numRows, int &numCols);
};

#endif