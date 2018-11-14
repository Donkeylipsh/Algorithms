/***************************************************
 * Author: Colin Van Overschelde
 * Date: 4/27/2018
 * Assignment: Homework 4
 * Description: greedySelect.cpp accepts a file of formatted text containing
 * 		a list of activity sets, and displays the optimum selection
 * 		of activities using a greedy selection algorithm
*****************************************************************************/

#include <iostream>
#include <fstream>
#include <string>
#include <assert.h>
#include <stack>

using namespace std;

/*********************************************************************
 * Description: Contains data that describes an Activity
 * Parameters: id is the unique identifier of the Activity
 * 	       start is the start time of the Activity
 * 	       end is the end time of the Activity
 *********************************************************************/
struct Activity{
    int id;
    int start;
    int end;
};

void processActivityLists();
void mergeSort(struct Activity[], int, int);
void merge(struct Activity[], int, int, int);
void printActivities(struct Activity[], int);
void greedy(struct Activity[], int);

/*********************************************************************
 * Description: main() Displays the welcome message, calls the function to process the
 * 		Activity lists stored in "act.txt"
 * Pre-conditions: n/a
 * Post-conditions: Program completes and exits with status code 0
 * Parameters: n/a
 *********************************************************************/
int main(){
    // Display welcome messages
    cout << endl << "Welcome to Greedy Select" << endl;
    cout << "Programmed by Colin Van Overschelde" << endl << endl;

    processActivityLists();

    // Display farewell message
    cout << endl << "Thank you for using Greedy Select, goodbye." << endl;

    return 0;
}

/*********************************************************************
 * Description: processActivityLists() opens the Activity lists stored in "act.txt",
 * 		parses the each Activity List into an array of Activities, sorts the array
 *		by start time in descending order, and then uses a Greedy algorithm to
 *		select the optimal Activities to maximize the count of Activities selected
 * Pre-conditions: "act.txt" contains one or more formatted lists of Activities
 * Post-conditions: The optimal Activity selection is displayed for each Activity list in "act.txt"
 * Parameters: n/a
 *********************************************************************/
void processActivityLists(){

    //cout << endl << "processActivityLists() started..." << endl;

    //cout << "* Creating input stream" << endl;
    // Create the input stream
    ifstream inputFile("act.txt");

    // Check if the file was opened
    if (inputFile){
	// If yes, read the entire contents of the file
	cout << "Reading from \"act.txt\"..." << endl;
	
	string nextLine;	// Varible to hold the line read from file
	bool isNewList = true;
	int listSize = 0;
	int setNum = 1;

	// Loop through each line of the file
	while( getline(inputFile, nextLine) ){
	    // Read each characher from the line
	    //cout << "* New list, line read: " << nextLine << endl;
	    // Reset loop control variable
	    listSize = 0;
	    for(int i = 0; i < nextLine.length(); i++){
		//cout << "Character read: " << nextLine[i] << endl;
		// Check if the character read is a number
		if( isdigit(nextLine[i]) ){
		    // Check if first number of list size
		    if(listSize == 0){
		        // If yes, assign it to listSize
		        listSize = nextLine[i] - 48;
		    }
		    else{
		        // If not, add the number as a new digit
		        listSize *= 10;
		        listSize += (nextLine[i] - 48);
		    }
		}
	    }
	    //cout << "* List size read: " << listSize << endl;

	    // Create a new array of size listSize
	    struct Activity someSet[listSize];
	    // Add each activity in the list to the array
	    for(int i = 0; i < listSize; i++){
		// Reset control variables
		int numType = 0;
		bool whiteSpace = true;

		// Initialize activity values
		someSet[i].id = 0;
		someSet[i].start = 0;
		someSet[i].end = 0;

		// Read the activity line from the file
		getline(inputFile, nextLine);
		//cout << "** New activity, line read: " << nextLine << endl;
		// Read each character from the activity line
		for(int j = 0; j < nextLine.length(); j++){
		    // Read each character from the line
		    if( isdigit(nextLine[j]) ){
			// Update whitespace value
			whiteSpace = false;
			// Assign the read value to the appropriate activity variable
			if(numType == 0){
			    // Add the value to the activity id
			    if(someSet[i].id == 0){
				someSet[i].id = nextLine[j] - 48;
			    }
			    else{
				someSet[i].id *= 10;
				someSet[i].id += (nextLine[j] - 48);
			    }
			}
			else if(numType == 1){
			    // Add the value to the activity start
			    if(someSet[i].start == 0){
				someSet[i].start = nextLine[j] - 48;
			    }
			    else{
				someSet[i].start *= 10;
				someSet[i].start += (nextLine[j] - 48);
			    }
			}
			else if(numType == 2){
			    // Add the value to the activity end
			    if(someSet[i].end == 0){
				someSet[i].end = nextLine[j] - 48;
			    }
			    else{
				someSet[i].end *= 10;
				someSet[i].end += (nextLine[j] -48);
			    }
			}
		    }
		    else{
			// Check if whitespace flag is false
			if(!whiteSpace){
			    // If yes, increment numType
			    numType++;
			    whiteSpace = true;
			}
		    }
		}
		//cout << "** Activity stored:" << endl;
		//cout << "*** id = " << someSet[i].id << endl;
		//cout << "*** start = " << someSet[i].start << endl;
		//cout << "*** end = " << someSet[i].end << endl;
	    }
	    mergeSort(someSet, 0, (listSize - 1));
	    //printActivities(someSet, listSize);
	    cout << endl << "Set " << setNum << endl;
	    greedy(someSet, listSize);
	    setNum++;
	}
    }
    else{
	// If not, display error message
	cout << "* Unable to open \"act.txt\"" << endl;
    }

}

/*********************************************************************
 * Description: mergeSort() performs Merge Sort on an input Array of Activiry structs
 * Pre-conditions: someSet is not NULL
 * Post-conditions: someSet is sorted by start time in descending order
 * Parameters: someSet is an array of Activity structs
 * 	       start is the index of the first Activity to sort
 * 	       end is the index of the lst Activity to sort
 *********************************************************************/
void mergeSort(struct Activity someSet[], int start, int end){
    // Validate parameters
    assert(someSet != NULL);

    //cout << endl << "mergeSort started()..." << endl;
    //cout << "* start = " << start << " | end = " << end << endl;

    // Check if size > 1
    if( start < end ){
	// Find the middle index
	int mid = start + ((end - start) / 2);
	//Recursively sort the first half
	mergeSort(someSet, start, mid);
	// Recursively sort the second half
	mergeSort(someSet, mid + 1, end);
	// Return the merged sorted halves
	merge(someSet, start, mid, end);
    }
}

/*********************************************************************
 * Description: merge() combines two sorted halves of an array into a single
 * 		sorted array
 * Pre-conditions: someSet contains two sorted subsets, one from index start to index mid,
 * 		   and another from index mid + 1 to index end
 * Post-conditions: All elements of someSet, from index start to index end are sorted
 * Parameters: someSet is an array Activity structs
 * 	       start is the index of the first element of the left array
 * 	       mid is the index of the last element of the left array
 * 	       end is the index of the last element of the right array
 *********************************************************************/
void merge(struct Activity someSet[], int start, int mid, int end){
    // Validate parameters
    assert(someSet != NULL);

    //cout << endl << "merge() started..." << endl;
    //cout << "* start: " << start << " | mid: " << mid << " | end: " << end << endl;

    // Calculate the size of each sub-array
    int leftSize = mid - start + 1;
    int rightSize = end - mid;

    // Create new sub-arrays
    struct Activity leftSet[leftSize];
    struct Activity rightSet[rightSize];

    // Copy values into the sub-arrays
    for(int i = 0; i < leftSize; i++){
	leftSet[i] = someSet[start + i];
    }
    for(int i = 0; i < rightSize; i++){
        rightSet[i] = someSet[mid + 1 + i];
    }

    // Merge teh two sub-arrays
    int currentIndex = start;
    int leftIndex = 0;
    int rightIndex = 0;

    // While both arrays are not empty, add the lowest value between both arrays to nums
    while ( (leftIndex < leftSize) && (rightIndex < rightSize) ){
	// Check which array contains the greater start
	if(leftSet[leftIndex].start >= rightSet[rightIndex].start){
	    // Copy the value in leftSet to someSet
	    someSet[currentIndex] = leftSet[leftIndex];
	    // Increment leftIndex
	    leftIndex++;
	}
	else{
	    // Copy the value in rightSet to someSet
	    someSet[currentIndex] = rightSet[rightIndex];
	    // Increment rightIndex
	    rightIndex++;
	}
	// Increment current index
	currentIndex++;
    }

    // Add the remaining values of leftSet to someSet
    while (leftIndex < leftSize){
	// Copy the value to someSet
	someSet[currentIndex] = leftSet[leftIndex];
	// Increment indices
	leftIndex++;
	currentIndex++;
    }

    // Add the remaining values of right set to someSet
    while (rightIndex < rightSize){
	// Copy the value to someSet
	someSet[currentIndex] = rightSet[rightIndex];
	// Increment indices
	rightIndex++;
	currentIndex++;
    }
}

/*********************************************************************
 * Description: greedy() selects the optimal Activities to select from an array
 * 		of Activities in order to maximize the number of Activities
 * Pre-conditions: someSet is an array of Activity structs, sorted by start time 
 * 		   in descending order
 * Post-conditions: The optimal selection of Activities is displayed to the console
 * Parameters: someSet is an array of Activity structs
 * 	       size is the number of Activities in someSet
 *********************************************************************/
void greedy(struct Activity someSet[], int size){
    //cout << endl << "greedy() started..." << endl;
    
    int currentStart = someSet[0].end + 1;
    stack<struct Activity> optimal;

    // Loop through each activity
    for(int i = 0; i < size; i++){
        // Does activity end before currentStart
        if(someSet[i].end <= currentStart){
            // If yes, add to the solution
	    optimal.push(someSet[i]);
	    // Update currentStart value
	    currentStart = someSet[i].start;
        }
            
    }

    // Display results
    cout << "Number of activities selected = " << optimal.size() << endl;
    cout << "Activities:";
    while(optimal.size() > 0){
	struct Activity current = optimal.top();
	cout << " " << current.id;
	optimal.pop();
    }
    cout << endl;
}

/*********************************************************************
 * Description: printActivities() displays each Activity in an array
 * Pre-conditions: n/a
 * Post-conditions: All Activities in the array have been printed to the console
 * Parameters: someSet is an array of Activities
 * 	       size is the number of Activities in someSet
 *********************************************************************/
void printActivities(struct Activity someSet[], int size){
    cout << endl << "printActivities() started..." << endl;

    for(int i = 0; i < size; i++){
	cout << "id: " << someSet[i].id << " | start: " << someSet[i].start << " | end: " << someSet[i].end << endl;
    }

}
