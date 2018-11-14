/********************************************************************************
 * Author: Colin Van Overschelde
 * Date: 4/22/2018
 * Description: readFile.cpp provides functions to read input data in a specific format
 * 		related to the makeChange algorithm, and then sends the read data
 * 		to the makeChange function, to calculate the optimal solution for the input
 ********************************************************************************/

#include "readFile.hpp"

using namespace std;

/*********************************************************************************
 * Description: readAmount() reads data from input file "amount.txt", parses it into
 * 		a vector usable by makeChange(), and then calls makeChange() for each
 * 		set of valid data in the file
 * Parameters: n/a
 * Pre-conditions: "amount.txt" is formated to allow readAmount to parse data
 * Post-coditions: All entries in "amount.txt" have been read, and their optimal solution
 * 		   is calculated and saved to a file called "change.txt"
 *********************************************************************************/
void readAmount(){

    //cout << "readAmount started" << endl;

    // Create input stream
    ifstream inputFile("amount.txt");

    // If file was opened, read teh contents
    if (inputFile) {

	cout << "Reading from \"amount.txt\"..." << endl;

	cout << "Calculating minimum number of coins..." << endl;

	// Variables to parse data
	string nextLine;	// Holds the nextLine read form teh file
	char nextChar;		// Holds the next character read from nextLine
	bool isCoinLine = true;		// Indicates the line's data type/format
	int currentNum = 0;		// The current value being parsed from the file
	
	// Variables to calculate optimal solution
	int total = 0;		// Amount to calculate solution for
	vector<int> coins;	// Vecotr to hold coin denominations

	// Loop through the file, reading each line and saving it to nextLine
	while( getline(inputFile, nextLine) ){
	    // Check the data type of the current line
	    if(isCoinLine){
	        //cout << "Reading characters from coin line: ";
	    }
	    else{
		//cout << "Reading total value line: ";
	 	total = 0;
	    }
	    //cout << nextLine << endl;

	    // Parse each character from nextLine
	    for(int i = 0; i < nextLine.length(); i++){

		// Set nextChar equal to the next character in nextLine
		nextChar = nextLine[i];
		// Check if the character read is a digit
		if( isdigit(nextChar) ){
	
		    //cout << "Number read: " << nextChar << endl;

		    // Check if first digit of a new number
		    if( currentNum == 0 ){
			// If yes, add the numeric value of the character to currentNum
			currentNum = nextChar - 48;
		    }
		    else{
			// If not, add currentNum as the next digit in the existing currentNum
			currentNum *= 10;
			currentNum += (nextChar - 48);
		    }

		}
		else{
		    // If not, then a number has been fully read and can be stored
		    // Check if the number should be added to the denominations vector
		    if(isCoinLine){
			//cout << "Adding coin to vector..." << endl;
			coins.push_back(currentNum);
		    }
		    // Reset currentNum
		    currentNum = 0;
		}	
	    }
	    //cout << "Last number read: " << currentNum << endl;
	    // Check if currentNum needs to be added to vector
	    if(isCoinLine){
		//cout << "Adding coin to vector..." << endl;
		// If yes, add the the number to the coin vector
		coins.push_back(currentNum);
		//printVector(coins);
	    }
	    else{
		// Set total = currentNum
		total = currentNum;

		// Print input array
		printVector(coins);
		//cout << "* Total coins to find optimal solution: " << total << endl;
		writeFile(to_string(total));	
	
		makeChange(total, coins);
		coins.clear();
	    }


	    // Calculate optimal change
	    //makeChange(total, coins);

	    // Print results to file
	    //printVector(coins);
	    //cout << "* Total coins to find optimal solution: " << total << endl;

	    // Reset currentNum
	    currentNum = 0;

	    // Update isCoinLine for nextLine
	    isCoinLine = !isCoinLine;
	}

    }
    else {
	// If not, display error
	cout << "Unable to open \"amount.txt\"" << endl;

    }


}

/****************************************************************
 * Description: writeFile accepts a string and saves it to "change.txt"
 * Parameters: someString is the string to be saved
 * Pre-conditions: n/a
 * Post-conditions: someString has been added to "change.txt"
 ***************************************************************/
void writeFile(string someString){

    ofstream outfile;
    outfile.open("change.txt", ofstream::out | std::ofstream::app);

    outfile << someString << endl;

    outfile.close();

}


/*************************************************************************
 * Description: printVector accepts a vector of integers, converts it to a
 * 		single line string, with values separated by a ' ', and then
 * 		saves that string to the file "change.txt"
 * Parameters: nums is a vector of integers to save
 * Pre-conditions: n/a
 * Post-conditions: nums has been saved to "change.txt"
 * ***********************************************************************/
void printVector(vector<int> nums){

    //cout << "Printing vector:";
    //cout << "* size = " << nums.size() << endl;
    // Convert vector to string
    vector<int>::iterator it = nums.begin();
    string output = to_string(*it);
    ++it;
    for(; it != nums.end(); ++it){
	output = output + ' ' + to_string(*it);
    }
    writeFile(output);
}
