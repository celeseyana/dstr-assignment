#include "header.hpp"

int main()
{
    int choice = 0;
    reviews = new string[reviewCount];
    ratings = new int[reviewCount];

    while (true) {
        cout << "1. Binary search & Selection Sort " << endl;
        cout << "2. Linear search & Bubble Sort" << endl;
        cout << "Enter your selection here: ";
        cin >> choice;
        while (cin.fail() || choice < 1 || choice > 6) {
            cout << "Invalid choice. Please enter a number between 1 and 2: ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cin >> choice;
        }

        switch (choice) {
        case 1: {
            // Start measuring time for Case 1
            auto start = high_resolution_clock::now();

            // Load positive and negative word lists
            loadWords("D:/Github/dstr-assignment/dstr-assignment/tristen/tristen/tristen/required/positive-words.txt", positiveWords, posWordCount);
            loadWords("D:/Github/dstr-assignment/dstr-assignment/tristen/tristen/tristen/required/negative-words.txt", negativeWords, negWordCount);

            // Sort the word lists using selection sort for binary search efficiency
            selectionSort(positiveWords, posWordCount);
            selectionSort(negativeWords, negWordCount);

            // Load the reviews and ratings from the CSV file
            loadReviews("D:/Github/dstr-assignment/dstr-assignment/tristen/tristen/tristen/required/tripadvisor_hotel_reviews.csv");

            // Call the analyzeReviews function
            analyzeReviews();

            // Calculate overall sentiment
            calculateOverallSentiment();

            // End measuring time for Case 1
            auto end = high_resolution_clock::now();
            auto duration = duration_cast<seconds>(end - start).count();
            cout << "Case 1 (Binary search & Selection Sort) execution time: " << duration << " milliseconds." << endl;

            // Clean up dynamic arrays
            delete[] reviews;
            delete[] ratings;

            return 0;
        }
        case 2: {
            // Start measuring time for Case 2
            auto start = high_resolution_clock::now();

            int numRows = 0;
            int numCols = 0;

            FileReader reader;
            reader.readNegative("D:/Github/dstr-assignment/dstr-assignment/tristen/tristen/tristen/required/negative-words.txt");
            cout << "Loading..." << endl;
            reader.readPositive("D:/Github/dstr-assignment/dstr-assignment/tristen/tristen/tristen/required/positive-words.txt");
            cout << "More loading..." << endl;
            int* positiveCounts = new int[reader.positiveRead];
            int* negativeCounts = new int[reader.negativeRead];

            string filename;

            filename = "D:/Github/dstr-assignment/dstr-assignment/tristen/tristen/tristen/required/tripadvisor_hotel_reviews.csv";
            int reviewNumber = totalCount(filename);
            cout << "Loading.." << endl;

            filename = "D:/Github/dstr-assignment/dstr-assignment/tristen/tristen/tristen/required/positive-words.txt";
            int totalPos = totalCount(filename);
            cout << "More loading.." << endl;

            filename = "D:/Github/dstr-assignment/dstr-assignment/tristen/tristen/tristen/required/negative-words.txt";
            int totalNeg = totalCount(filename);
            cout << "Almost done..." << endl;

            string** csvData = readCSV("D:/Github/dstr-assignment/dstr-assignment/tristen/tristen/tristen/required/tripadvisor_hotel_reviews.csv", numRows, numCols);
            countWordFrequencies(csvData, numRows, reader, positiveCounts, negativeCounts);
            cout << "Surely.." << endl;

            int totalpositiveCount = 0;
            int totalnegativeCount = 0;
            reader.countWordMatches(totalpositiveCount, totalnegativeCount);
            cout << "Please.." << endl;

            while (true)
            {
                displayMenu();
                cout << "Enter your choice (1-5): ";
                cin >> choice;

                while (cin.fail() || choice < 1 || choice > 6)
                {
                    cout << "Invalid choice. Please enter a number between 1 and 6: ";
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cin >> choice;
                }

                switch (choice)
                {
                case 1:
                    cout << "Negative Words:\n";
                    for (int i = 0; i < reader.negativeRead; i++)
                    {
                        cout << reader.negativeWords[i] << endl;
                    }
                    break;
                case 2:
                    cout << "\nPositive Words:\n";
                    for (int i = 0; i < reader.positiveRead; i++)
                    {
                        cout << reader.positiveWords[i] << endl;
                    }
                    break;
                case 3:
                    if (csvData != nullptr)
                    {
                        cout << "CSV Data (" << numRows << " rows, " << numCols << " columns):\n";
                        printCSV(csvData, numRows, numCols);
                        analyzeCSV(csvData, numRows, numCols, reader);
                    }
                    break;
                case 4:
                    cout << "\nTotal Review Count: " << reviewNumber << endl;
                    cout << "\nTotal Positive Words: " << reader.positiveRead << endl;
                    cout << "\nTotal Negative Words: " << reader.negativeRead << endl;
                    cout << "\nTotal flagged positive word in reviews: " << totalpositiveCount << endl;
                    cout << "\nTotal flagged negative word reviews: " << totalnegativeCount << endl;

                    bubbleSort(reader.positiveWords, positiveCounts, reader.positiveRead);
                    cout << "\nTop 10 Positive Words:\n";
                    displayTopWords(reader.positiveWords, positiveCounts, min(10, reader.positiveRead));

                    bubbleSort(reader.negativeWords, negativeCounts, reader.negativeRead);
                    cout << "\nTop 10 Negative Words:\n";
                    displayTopWords(reader.negativeWords, negativeCounts, min(10, reader.negativeRead));
                    break;
                case 5:
                    cout << "Exiting.\n";
                    return 0;
                default:
                    break;
                }

                cout << "\n";
            }

            // End measuring time for Case 2
            auto end = high_resolution_clock::now();
            auto duration = duration_cast<seconds>(end - start).count();
            cout << "Case 2 (Linear search & Bubble Sort) execution time: " << duration << " milliseconds." << endl;

            // Clean up dynamic arrays
            delete[] positiveCounts;
            delete[] negativeCounts;
            delete[] csvData;
        }
        }
    }
    return 0;
}
