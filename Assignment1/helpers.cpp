#include "helpers.hpp"

/********************************************************************************************
 * Description: Reads values from input file 'data.txt', storing each line in an array, then
 *		sorting the values in the array in ascending order, and finally writes the 
 *		array as a new line to either "merge.out" or "insert.out"
 * Parameters: int sortType contains an integer value corresponding to the sorting algorithm 
 *		to be used. (0 = insertion sort, 1 = merge sort)
 * Pre-conditions: A file named 'data.txt' is the same folder as this program
 * Returns: n/a
 *********************************************************************************************/
void readInputFile(int sortType) {
    cout << "Reading from file..." << endl;
    // Open the input file
    //string inputName = INPUT_NAME;
    ifstream inputFile("data.txt");

    // If file was opened, read the contents
    if (inputFile) {
	string nextLine;
	char nextChar;			// Holds the value of the most recent character read
	int currentNum = 0;		// Holds the value of the current number being read from file
	bool newLine = true;	// Indicates if a newLine has been read from file
	int *myNumbers = NULL;	// Array to hold the values of each line
	int numCount = 0;		// Number of values read from the current line

	// Loop through each character until end of file
	while ( getline(inputFile, nextLine) ) {
	    //cout << "Line read from file: " << nextLine << endl;

	    for(int i = 0; i < nextLine.length() - 1; i++){
			// Set nextChar equal to the next character in the string
			nextChar = nextLine[i];
	        
			// Check if nextChar = ' '
			if (nextChar == ' ') {
				// Check if first number of newLine
				if (newLine) {
					// Reallocate myNumbers with an arrat of size currentNum
					myNumbers = new int[currentNum];

					//cout << "Creating new array of size: " << currentNum << endl;

   					// Reset array variables
					currentNum = 0;
					newLine = false;
				}
				else {
					// Add currentNum to myNumbers at index numCount
					myNumbers[numCount] = currentNum;

					//cout << "* myNumbers[" << numCount << "]: " << currentNum << endl;

					// increment numCount
					numCount++;
					// reset currentNum to 0
					currentNum = 0;
					// Reset newLine to false
					newLine = false;
				}
			}
			else {
				// Check if currentNum = 0
				if (currentNum == 0) {
					// set currentNum = nextChar - 48
					currentNum = nextChar - 48;
					}
				else {
					// set currentNum *= 10;
					currentNum *= 10;
					// set currentNum += (nextChar - 48)
					currentNum += (nextChar - 48);
				}
            }
        }
	    // Add currentNum to myNumbers at index numCount
	    myNumbers[numCount] = currentNum;
	    // increment numCount
	    numCount++;
	    // reset currentNum to 0
	    currentNum = 0;
	    // Sort the array
	    if (sortType == 0) {
	        // Use insertion sor to sort the array
	        insertionSort(myNumbers, numCount);
	        // Save the sorted array to the output file
	        saveArray(sortType, myNumbers, numCount, true);
	    }
	    else if (sortType == 1) {
	        // Merge sort
	        mergeSort(myNumbers, 0, (numCount - 1));

	        // Print the array
	        //printArray(myNumbers, numCount);

	        // Save the sorted array to the output file
	        saveArray(sortType, myNumbers, numCount, true);
	    }
	    
	    // Release the array from memory
	    if(myNumbers != NULL){
	        delete[] myNumbers;
	        myNumbers = NULL;
	    }
	    numCount = 0;
	    newLine = true;
        }
    }
    else {
	// Display error
	cout << "Unable to open file" << endl;
    }
    // Close the input file
    inputFile.close();
}

/***************************************************************************************
 * Description: Populates an integer array with 'count' random values in the range (0 - 10,000)
 * Parameters: int *nums is the integer array to populate
 *		int count is the number of random values to generate
 * Pre-conditions: nums exists
 * Post-Condidtions: nums is populated with new random numbers
***************************************************************************************/
void generateNums(int *nums, int count) {
    // Validate parameters
    assert(nums != NULL);

    // Generate random numbers
    for (int i = 0; i < count; i++) {
	int someNum = rand() % 10001;
	nums[i] = someNum;
    }
}

/***************************************************************************************
 * Description: Prints the values of an array to the console
 * Parameters: int *nums is the integer array to be printed
 *			   int size is the size of nums
 * Pre-conditions: nums exists
 * Post-conditions: the array is printed to the console
***************************************************************************************/
void printArray(int *nums, int size) {
    // Validate parameters
    assert(nums != NULL);
    // Print each elemnt of the array
    for (int i = 0; i < size; i++) {
	cout << nums[i] << ' ';
    }
    // End the display
    cout << endl;
}

/***************************************************************************************
 * Description: Prints the values of the results array to the console
 * Parameters: int *nums is the integer array to be printed
 *			   int size is the size of nums
 * Pre-conditions: nums exists
 * Post-conditions: the array is printed to the console
 ***************************************************************************************/
void printResults(float *nums, int size) {
    // Validate parameters
    assert(nums != NULL);

    int n = 500;	// Test size
    // Print each elemnt of the array
    for (int i = 0; i < size; i++) {
	// Increment test size
	n = n * 2;
	// Display the element value
	cout << "n = " << n << ": " << nums[i] << "ms" << endl;
    }
}

/***************************************************************************************
 * Description: Saves the values of an integer array 'nums', to output file 'output'
 * Parameters: string output is the name of the file to save to
 *			  int *nums is the array to be saved
 *			  int size is the size of nums
 *			  bool newLine indicates if a newLine should be added to the end of the line
 * Pre-conditions: nums exists
 * Post-conditions: the array is saved to the output file
***************************************************************************************/
void saveArray(int sortId, int *nums, int size, bool newLine) {
    // Validate parameters
    assert(nums != NULL);

    // Create an ofstream object
    ofstream outFile;
    // Open the output file
    if ( sortId == 0){ 
        outFile.open("insert.out", ios::app);
    }
    else{
	outFile.open("merge.out", ios::app);
    }
    // Loop through the array, writing each element to file
    for (int i = 0; i < size; i++) {
	outFile << nums[i];
	if (i < (size - 1)) {
	    outFile << " ";
	}
    }
    // Check if the newLine parameter was set
    if (newLine) {
	// Print a new line
	outFile << '\n';
    }

    // Close the output file
    outFile.close();
}

/***************************************************************************************
 * Description: Clears the contents of the output file, preparing it for use
 * Parameters: int fileId indicates the output file to clear
 * Pre-conditions: n/a
 * Post-conditions: The contents of the output file has been cleared
***************************************************************************************/
void clearOutputFile(int fileId) {
    // Clear all existing contents from the output file
    // Check which file name to remove
    if (fileId == 0) {
	remove("insert.out");
    }
    else {
	remove("merge.out");
    }
}

/***************************************************************************************
 * Description: Performs merge sort on integer array 'nums'.  Merge sort recursively breaks 
 *			   the input array into two sub-arrays until there is only 1 element, and then
 *			   merges the arrays back into a sorted array
 * Parameters: int *nums is the array to be sorted
 *			  int start is the index of the first element of the array
 *			  int end is the index of the last element of the array
 * Pre-conditions: nums exists
 * Post-conditions: The elements of nums are sorted in ascending order
 ***************************************************************************************/
void mergeSort(int *nums, int start, int end) {
    // Validate parameters
    assert(nums != NULL);

    // Check if size > 1
    if (start < end) {
	// Find the middle index
	int mid = start + ((end - start) / 2);
	// Recursively sort the first half of the array
	mergeSort(nums, start, mid);
	// Recursively sort the second half of the array
	mergeSort(nums, mid + 1, end);
	// Return the merged sorted halves
	merge(nums, start, mid, end);
    }
}

/******************************************************************************************
 * Description: Creates two sub-arrays based on the indecies calculated by mergeSort, and
 *			   merges the arrays in sorted order back into 'nums'
 * Parameters: int *nums is the array to be sorted
 *			  int start is the index of the first element of the left sub-array
 *			  int mid is the last element of the left sub-array
 *			  int end is the index of the last element of the right sub-array
 * Pre-conditions: nums exists
 * Post-conditions: The elements of nums are sorted in ascending order
***************************************************************************************/
void merge(int *nums, int start, int mid, int end) {
    // Validate parameters
    assert(nums != NULL);

    // Calculate the size of each sub-array
    int leftSize = mid - start + 1;
    int rightSize = end - mid;

    // Allocate new sub-arrays
    int *leftArray = new int[leftSize];
    int *rightArray = new int[rightSize];

    // Copy the values to the temp arrays
    for (int i = 0; i < leftSize; i++) {
	leftArray[i] = nums[start + i];
    }
    for (int i = 0; i < rightSize; i++) {
	rightArray[i] = nums[mid + 1 + i];
    }

    // Merge the two sub-arrays
    int currentIndex = start;
    int leftIndex = 0;
    int rightIndex = 0;
    // While both arrays are not empty, add the lowest value between both arrays to nums
    while ((leftIndex < leftSize) && (rightIndex < rightSize)) {
	// Check which array to pull from
	if (leftArray[leftIndex] <= rightArray[rightIndex]) {
	    // Copy the value of the active index to nums
	    nums[currentIndex] = leftArray[leftIndex];
	    // Increment active index of left array
	    leftIndex++;
	}
	else {
	    // Copt the value of the active index to nums
	    nums[currentIndex] = rightArray[rightIndex];
	    // Increment active index of right array
	    rightIndex++;
	}
	// Increment index of nums
	currentIndex++;
    }

    // Add the remaining values of left sub-array to nums
    while (leftIndex < leftSize) {
	// Copy the value to nums
	nums[currentIndex] = leftArray[leftIndex];
	// Increment the index of nums and leftArray
	currentIndex++;
	leftIndex++;
    }

    // Add the remaining values of right sub-array to nums
    while (rightIndex < rightSize) {
	// Copy the value to nums
	nums[currentIndex] = rightArray[rightIndex];
	// Increment the index of nums and rightArray
	currentIndex++;
	rightIndex++;
    }

    // Release the memory allocated to the temporary arrays
    delete[] leftArray;
    delete[] rightArray;
}

/***************************************************************************************
 * Description: Performs insertion sort on integer array 'nums'.  Insertion sort starts at
 *			   index 1, and moves the element to the left until a smaller value is encountered
 * Parameters: int *nums is the array to be sorted
 *			  int size is the number of elements in 'nums'
 * Pre-conditions: nums exists
 * Post-conditions: The elements of nums are sorted in ascending order
 ***************************************************************************************/
void insertionSort(int *nums, int size) {
    // Validate parameters
    assert(nums != NULL);

    //Loop through each index of nums, starting at index 1
    for (int i = 1; i < size; i++) {
	// Assign the value of i to currentIndex
	int currentIndex = i;
	// Create loop control variable to hold the index of the element before currentIndex
	int j = currentIndex - 1;

	// Swap value in nums[currentIndex] with value in nums[j] until either j < 0, or nums[currentIndex] > nums[j]
	while ((j >= 0) && (nums[currentIndex] < nums[j])) {
	    // Swap values in i and j
	    int temp = nums[j];
	    nums[j] = nums[currentIndex];
	    nums[currentIndex] = temp;

	    // Decrement currentIndex and j
	    currentIndex--;
	    j--;
	}
    }
}

/***************************************************************************************
 * Description: Performs 'testCount' sort tests, using the specified testing algorithm, and
 *			   stores the results in float array 'results'
 * Parameters: float *results is the array to store the results of each test
 *			  int testCount is the number of tests to perform
 *			  int testId indicates the sorting algorithm to use
 * Pre-conditions: 'results' exists
 * Post-conditions: 'results' contains the results of each test
 ***************************************************************************************/
void runtimeTest(float *results, int testCount, int testId) {
    // Validate parameters
    assert(results != NULL);

    // Seed the random number generator
    srand(time(NULL));

    // Set the initial value of n to 500
    int n = 500;

    // Test sort algorithm 'testCount' times, doubling n with each test
    for (int i = 0; i < testCount; i++) {
	// Increment n for the next test
	n = n * 2;
	// Create array of size n
	int *nums = new int[n];
	// Create array to hold result of each test
	float tests[5];

	// Perform 10 sort algorithm tests on an array of size n
	for (int j = 0; j < 5; j++) {
	    // Generate new numbers for array
	    generateNums(nums, n);

	    // Start the timer
	    clock_t startTime;
	    startTime = clock();

	    // Sort the input array
	    if (testId == 0) {
		// Sort the array with insertion sort
		insertionSort(nums, n);
	    }
	    else {
		// Use merge sort to test the array
		mergeSort(nums, 0, n - 1);
	    }

	    // Stop the timer
	    startTime = clock() - startTime;
	    // Convert the test result to miliseconds
	    float difference = (static_cast<float>(startTime)) / CLOCKS_PER_SEC * 1000;

	    // Save the result of the test
	    tests[j] = difference;
	}

	// Average the tests
	float average = 0;
	for (int j = 0; j < 5; j++) {
	    average += tests[j];
	}
	average = average / 5;

	// Save the average of the tests to results
	results[i] = average;

	// Release memory
	delete[] nums;
	nums = NULL;
    }
}
