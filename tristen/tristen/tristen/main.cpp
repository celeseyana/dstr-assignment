//#include "header.hpp"
//
//int main()
//{
//    int numRows = 0;
//    int numCols = 0;
//
//    FileReader reader;
//
//    reader.readNegative("D:/Github/dstr-assignment/dstr-assignment/tristen/tristen/tristen/required/negative-words.txt");
//    cout << "Loading..." << endl;
//    reader.readPositive("D:/Github/dstr-assignment/dstr-assignment/tristen/tristen/tristen/required/positive-words.txt");
//    cout << "More loading..." << endl;
//    int* positiveCounts = new int[reader.positiveRead];
//    int* negativeCounts = new int[reader.negativeRead];
//
//    std::string filename;
//
//    filename = "D:/Github/dstr-assignment/dstr-assignment/tristen/tristen/tristen/required/tripadvisor_hotel_reviews.csv";
//    int reviewNumber = totalCount(filename);
//    cout << "Loading.." << endl;
//
//    filename = "D:/Github/dstr-assignment/dstr-assignment/tristen/tristen/tristen/required/positive-words.txt";
//    int totalPos = totalCount(filename);
//    cout << "More loading.." << endl;
//
//    filename = "D:/Github/dstr-assignment/dstr-assignment/tristen/tristen/tristen/required/negative-words.txt";
//    int totalNeg = totalCount(filename);
//    cout << "Almost done..." << endl;
//
//    std::string** csvData = readCSV("D:/Github/dstr-assignment/dstr-assignment/tristen/tristen/tristen/required/tripadvisor_hotel_reviews.csv", numRows, numCols);
//    countWordFrequencies(csvData, numRows, reader, positiveCounts, negativeCounts);
//    cout << "Surely.." << endl;
//
//    int choice = 0;
//
//    int totalpositiveCount = 0;
//    int totalnegativeCount = 0;
//    reader.countWordMatches(totalpositiveCount, totalnegativeCount);
//    cout << "Please.." << endl;
//
//    while (true)
//    {
//        displayMenu();
//        std::cout << "Enter your choice (1-5): ";
//        std::cin >> choice;
//
//        while (std::cin.fail() || choice < 1 || choice > 6)
//        {
//            std::cout << "Invalid choice. Please enter a number between 1 and 6: ";
//            std::cin.clear();
//            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
//            std::cin >> choice;
//        }
//
//        switch (choice)
//        {
//        case 1:
//            std::cout << "Negative Words:\n";
//            for (int i = 0; i < reader.negativeRead; i++)
//            {
//                std::cout << reader.negativeWords[i] << std::endl;
//            }
//            break;
//        case 2:
//            std::cout << "\nPositive Words:\n";
//            for (int i = 0; i < reader.positiveRead; i++)
//            {
//                std::cout << reader.positiveWords[i] << std::endl;
//            }
//            break;
//        case 3:
//            if (csvData != nullptr)
//            {
//                std::cout << "CSV Data (" << numRows << " rows, " << numCols << " columns):\n";
//                printCSV(csvData, numRows, numCols);
//                analyzeCSV(csvData, numRows, numCols, reader);
//            }
//            break;
//        case 4:
//            std::cout << "\nTotal Review Count: " << reviewNumber << std::endl;
//            std::cout << "\nTotal Positive Words: " << reader.positiveRead << std::endl;
//            std::cout << "\nTotal Negative Words: " << reader.negativeRead << std::endl;
//            std::cout << "\nTotal flagged positive word in reviews: " << totalpositiveCount << std::endl;
//            std::cout << "\nTotal flagged negative word reviews: " << totalnegativeCount << std::endl;
//
//            bubbleSort(reader.positiveWords, positiveCounts, reader.positiveRead);
//            std::cout << "\nTop 10 Positive Words:\n";
//            displayTopWords(reader.positiveWords, positiveCounts, std::min(10, reader.positiveRead));
//
//            bubbleSort(reader.negativeWords, negativeCounts, reader.negativeRead);
//            std::cout << "\nTop 10 Negative Words:\n";
//            displayTopWords(reader.negativeWords, negativeCounts, std::min(10, reader.negativeRead));
//            break;
//        case 5:
//            std::cout << "Exiting.\n";
//            return 0;
//        default:
//            break;
//        }
//
//        std::cout << "\n";
//    }
//    return 0;
//}