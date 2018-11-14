/********************************************************************
 * Author: Colin Van Overschelde
 * Date: 4/8/2018
 * Description: insertsort.cpp demonstrates using the insertion sort algorithm
 * 		to read integers from a file, add them to an array and
 * 		sort the array in ascending order using insertion sort
 **********************************************************************/

#include "helpers.hpp"
#include <iostream>

using namespace std;

const int SORT_ID = 0;	// Indicates the sorting algorithm to use
//const int TEST_COUNT = 10;	// Indicates the number of test sizes for n

int main(){
    // Display welcome message
    cout << "Welcome to insertsort" << endl;
    cout << "Developed by Colin Van Overschelde" << endl << endl;

    // Clear the existing contents of the output file
    clearOutputFile(SORT_ID);

    // Use readInputFile() to read from the input file and sort the arrays
    readInputFile(SORT_ID);

    cout << "Sorted arrays have been saved to 'insert.out'" << endl;

    // Display exit message
    cout << endl << "Thank you for using insertsort, goodbye." << endl;
    
    return 0;
}
