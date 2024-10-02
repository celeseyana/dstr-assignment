#include "arrays.hpp"

// load words from text file into an array
int loadWords(const char* filename, string* wordArray, int maxWords)
{
	ifstream file(filename);
	if (!file.is_open()) {
		cout << "Error: Unable to open " << filename << endl;
		return 0;
	}

	string word;
	int count = 0;
	while (file >> word && count < maxWords)
	{
		wordArray[count] = word;
		count++;
	}
	file.close();

	selectionSort(wordArray, count);
	return count;
}

ReviewNode* loadReviews(const char* filename)
{
	ifstream file(filename);
	if (!file.is_open())
	{
		cout << "Error: Unable to open " << filename << endl;
		return nullptr;
	}

	string line;
	ReviewNode* head = nullptr;
	ReviewNode* tail = nullptr;

	while (getline(file, line))
	{
		size_t commaPos = line.find_last_of(',');
		string review = line.substr(0, commaPos);
		int rating = stoi(line.substr(commaPos + 1));

		ReviewNode* newNode = new ReviewNode{ review, rating, nullptr };

		if (head == nullptr)
		{
			head = newNode;
			tail = newNode;
		}
		else
		{
			tail->next = newNode;
			tail = newNode;
		}
	}
	file.close();
	return head;
}
void swapStrings(string& a, string& b)
{
	string temp = a;
	a = b;
	b = temp;
}
void selectionSort(string* arr, int size)
{
	for (int i = 0; i < size - 1; i++)
	{
		int minIndex = i;
		for (int j = i + 1; j < size; j++)
		{
			if (arr[j] < arr[minIndex])
			{
				minIndex = j;
			}
		}
		// swap elements
		swapStrings(arr[i], arr[minIndex]);
	}
}

int binarySearch(string* arr, int size, const string& word)
{
	int low = 0, high = size - 1;
	while (low <= high)
	{
		int mid = (low + high) / 2;
		if (arr[mid] == word)
		{
			return mid;
		}
		else if (arr[mid] < word)
		{
			low = mid + 1;
		}
		else
		{
			high = mid - 1;
		}
	}
	return -1;
}
// count occurrences of positive and negative words ina reivew

void countSentimentWords(const string& review, string* positiveWords, int positiveCount, string* negativeWords, int negativeCount, int& positiveCountInReview, int& negativeCountInReview)
{
	positiveCountInReview = 0;
	negativeCountInReview = 0;

	size_t pos = 0;
	string word;

	while (pos < review.length())
	{
		size_t spacePos = review.find(' ', pos);
		if (spacePos == string::npos) spacePos = review.length();
		word = review.substr(pos, spacePos - pos);

		if (binarySearch(positiveWords, positiveCount, word) != 1)
		{
			positiveCountInReview++;
		}

		if (binarySearch(negativeWords, negativeCount, word) != 1)
		{
			negativeCountInReview++;
		}
	}
}