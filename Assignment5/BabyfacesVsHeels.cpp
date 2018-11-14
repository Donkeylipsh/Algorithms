/*********************************************************************
 * Author: Colin Van Overschelde
 * Date: 5/13/2018
 * Title: Babyfaces vs. Heels
 * Description: Babyfaces vs. Heels demonstrates using Breadth First Search
 * 	to determine if it is possible to select Babyfaces and Heels such
 * 	that each rivalry is between a Babyface and a Heel. If yes, the
 * 	combination is displayed
 *********************************************************************/

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <queue>

using namespace std;

struct Wrestler{
    string name;	// Name of the wrestler
    int color;    // 0 = Unprocessed, 1 = In queue, 2 = Processed
    int d;	  // Odd values are Babyfaces, even values are Heels
    struct WrestlerList *rivals;	// Linked list representing Rival Wrestlers
};

struct WrestlerNode{
    struct WrestlerNode *next;		// Pointer to next node in list
    struct WrestlerNode *prev;		// Pointer to previous node in list
    struct Wrestler *someWrestler;	// Point to related Wrestler
};

struct WrestlerList{
    struct WrestlerNode *front;		// Pointer to the front of the list
    struct WrestlerNode *back;		// Pointer to the back of the list
};

// Function declarations
void readMatchesFile(struct WrestlerList *, char *);
void addWrestler(struct WrestlerList *, struct Wrestler *);
int getInputCount(string);
struct Wrestler * getWrestler(struct WrestlerList *, string);
bool hasValidRivals(struct WrestlerList *);
void BFS(struct WrestlerList *, struct Wrestler *);


int main(int argc, char* argv[]){

    // Initialize the roster list
    struct WrestlerList *roster = new WrestlerList;
    struct WrestlerNode *frontSentinel = new WrestlerNode;
    struct WrestlerNode *backSentinel = new WrestlerNode;
    frontSentinel->next = backSentinel;
    frontSentinel->prev = NULL;
    backSentinel->next = NULL;
    backSentinel->prev = frontSentinel;
    roster->front = frontSentinel;
    roster->back = backSentinel;

    // Read match card from file
    readMatchesFile(roster, argv[1]);

/*
    cout << "Wrestlers read from file: " << endl;
    struct WrestlerNode *current = roster->front;
    while(current->next != roster->back){
	current = current->next;
	struct Wrestler *someWrestler = current->someWrestler;
	//current->someWrestler->name.push_back('\0');
	cout << current->someWrestler->name << endl;
	struct WrestlerNode *someNode = someWrestler->rivals->front;
	cout << "Matches: ";
	while( someNode->next != someWrestler->rivals->back ){
	    someNode = someNode->next;
	    cout << someNode->someWrestler->name << ", ";
	}
	cout << endl;
    }
*/

    // Find all Babyface/Heel combinations
    // Perform breadth first search on each Wrestler
    struct WrestlerNode *currentWrestler = roster->front;
    bool validRivals = false;
    //cout << "Performing BFS..." << endl;
    while( currentWrestler->next != roster->back && !validRivals ){
	// Move to next Wrestler
	currentWrestler = currentWrestler->next;

	//cout << "On wrestler: " << currentWrestler->someWrestler->name << endl;

	// Perform breadth first search on currentWrestler
        BFS(roster, currentWrestler->someWrestler);

	// Check if solution is valid
        validRivals = hasValidRivals(roster);
        
//	cout << "Result: " << validRivals << endl;
    }

    // Check if rivalries can be made
    if(validRivals){
	// If yes, display Rivalries
	cout << "Yes Possible" << endl;
	// Build babyfaces and heels lists
	currentWrestler = roster->front;
	queue<struct Wrestler *> babyfaces;
	queue<struct Wrestler *> heels;
	while( currentWrestler->next != roster->back ){
	    currentWrestler = currentWrestler->next;
	    if( currentWrestler->someWrestler->d % 2 == 0 ){
		babyfaces.push(currentWrestler->someWrestler);
	    }else{
		heels.push(currentWrestler->someWrestler);
	    }
	}
	// Display babyfaces
        cout << "Babyfaces: ";
	int count = babyfaces.size();
	for(int i = 0; i < count; i++){
	    struct Wrestler *someWrestler = babyfaces.front();
	    cout << someWrestler->name;
	    if( i < count - 1 ){
		cout << " ";
	    }
	    babyfaces.pop();
	}
	// Display Heels
	cout << endl << "Heels: ";
	count = heels.size();
	for(int i = 0; i < count; i++){
	    struct Wrestler *someWrestler = heels.front();
	    cout << someWrestler->name;
	    if( i < count - 1 ){
		cout << " ";
	    }
	    heels.pop();
	}
	cout << endl;
    }else{
	// If not, display result
	cout << "No Impossible" << endl;
    }

    // Release memory
    // Release each WrestlerNode in roster
    struct WrestlerNode *nodes = roster->front;
    while(nodes->next != roster->back){
	nodes = nodes->next;
	struct Wrestler *cur = nodes->someWrestler;
	delete cur->rivals;
	delete cur;
	delete nodes->prev;
    }
    delete nodes->next;
    delete nodes;
    delete roster;

    return 0;
}

/************************************************************************
 * Description: readMatchesFile reads and parses it into a WrestlerList
 * Pre-conditions: fileName is the name of a file containing a formatted
 * 	list of Wrestlers and matches
 * Post-conditions: someRoster contains the Wrestlers and matches contained
 * 	in fileName
 * Parameters: someRoster is the WrestlerList to store the Wrestlers and
 * 	matches
 * 	fileName is the name of the file to read from
 ***********************************************************************/
void readMatchesFile(struct WrestlerList *someRoster, char *fileName){

    // Create input file stream
    ifstream inputFile(fileName);

    // Check if file was opened
    if (inputFile){
	// If yes, read the entire contents of the file
	//cout << "Reading from \"" << fileName << "\"..." << endl;

	string nextLine;	// Variable to hold each line read from the file
	bool isWrestler = true;
	
	// Get the first line of the file
	getline(inputFile, nextLine);
	int wrestlerCount = getInputCount(nextLine);
	// Get the names of all the wrestlers
	for(int i = 0; i < wrestlerCount; i++){
	    // Read next line from file
	    getline(inputFile, nextLine);
	    // Create new Wrestler object
	    struct Wrestler *newWrestler = new Wrestler;
	    struct WrestlerList *someRivals = new WrestlerList;
	    struct WrestlerNode *frontNode = new WrestlerNode;
	    struct WrestlerNode *backNode = new WrestlerNode;
	    // Remove end line characters from Wrestler name
	    nextLine.pop_back();
	    nextLine.push_back('\0');
	    newWrestler->name = nextLine;
	    // Prepare Rivalries
	    frontNode->prev = NULL;
	    frontNode->next = backNode;
	    backNode->prev = frontNode;
	    backNode->next = NULL;
	    someRivals->front = frontNode;
	    someRivals->back = backNode;
	    newWrestler->rivals = someRivals;
	    newWrestler->d = 0;
	    // Add the new Wrestler to the roster
	    addWrestler(someRoster, newWrestler);
	}
	
	// Get each matchup
	getline(inputFile, nextLine);
	// Get matchup count
	int matchCount = getInputCount(nextLine);
	for(int i = 0; i < matchCount; i++){
	    // Read the next line from the file
	    getline(inputFile, nextLine);
	    // Parse the Wrestlers
	    int spacer = nextLine.find(' ');
	    string w1 = nextLine.substr(0, spacer);
	    w1.push_back('\0');
	    string w2 = nextLine.substr((spacer + 1), (nextLine.length() - spacer - 2));
	    w2.push_back('\0');
	    // Get handle to Wrestlers
	    struct Wrestler *someWrestler1 = getWrestler(someRoster, w1);
	    struct Wrestler *someWrestler2 = getWrestler(someRoster, w2);
	    if( someWrestler1 == NULL || someWrestler2 == NULL ) {
		cout << "ERROR - Unable to find Wrestler" << endl;
	    }else{
		// Add each Wrestler to the others rivals list
		addWrestler(someWrestler1->rivals, someWrestler2);
		addWrestler(someWrestler2->rivals, someWrestler1);
	    }
	    
	}
    }else{
	cout << "Unable to open input file!" << endl;
    }

}

/************************************************************************
 * Description: getInputCount returns the number of Wrestlers/matchups
 * 	in the input section
 * Pre-conditions: nextLine is a Wrestler/Match count string
 * Post-conditions: Integer value of nextLine is returned
 * Parameters: nextLine is the string to read from
 ***********************************************************************/
int getInputCount(string nextLine){
    int wrestlerCount = 0;
    for(int i = 0; i < nextLine.length(); i++){
        // Check if the character read was a digit
        if( isdigit(nextLine[i]) ){
	    // Add the character to wrestlerCount
	    if(wrestlerCount == 0){
	        wrestlerCount = nextLine[i] - 48;
	    }else{
	        wrestlerCount *= 10;
	        wrestlerCount += ( nextLine[i] - 48 );
	    }
	}
    }

    return wrestlerCount;
}

/************************************************************************
 * Description: addWrestler adds a Wrestlers to a WrestlerList
 * Pre-conditions: n/a
 * Post-conditions: someWrestler is added to someRoster
 * Parameters: someRoster is the WrestlerList to add teh Wrester to
 * 	someWrestler is the Wrestler to add to the list
 ************************************************************************/
void addWrestler(struct WrestlerList *someRoster, struct Wrestler *someWrestler){
    //cout << "Starting addWrestler()..." << endl; 
    struct WrestlerNode *newNode = new WrestlerNode;
    newNode->someWrestler = someWrestler;

    //cout << "Wrestler received: " << someWrestler->name << endl;
    //cout << "Wrestler assigned: " << newNode->someWrestler->name << endl;

    if(someRoster == NULL){
	cout << "Bad roster" << endl;
    }

    // Add someWrestler to the back of someRoster
    newNode->next = someRoster->back;
    //cout << "newNode->next assigned" << endl;
    newNode->prev = someRoster->back->prev;
    //cout << "newNode->prev assigned" << endl;
    someRoster->back->prev->next = newNode;
    //cout << "someRoster->back->prev->next assigned" << endl;
    someRoster->back->prev = newNode;
    //cout << "Wrestler added: " << someRoster->back->prev->someWrestler->name << endl;
    //cout << "addWrestler() complete" << endl;
}

/************************************************************************
 * Description: getWrestler searches for a Wrestler with a specified name
 * 	within a specified WrestlerList
 * Pre-conditions: n/a
 * Post-conditions: If a Wrestler with the specified list is found, return
 * 	true. False, otherwise
 * Parameters: someRoster is the WrestlerList to search within
 * 	name is the name of the Wrester you are searching for
 ************************************************************************/
struct Wrestler * getWrestler(struct WrestlerList *someRoster, string name){
    //cout << "Starting getWrestler..." << endl;
    struct Wrestler *someWrestler = NULL;

    //cout << "* Name received: " << name << endl;

    struct WrestlerNode *current = someRoster->front;
    while( (current->next != someRoster->back) && (someWrestler == NULL) ){
	current = current->next;
//	cout << "Comparing to: " << current->someWrestler->name << endl;
	if( name.compare(current->someWrestler->name) == 0 ){
//	    cout << "Match found..." << endl;
	    someWrestler = current->someWrestler;
	}
    }

    return someWrestler;
}

/**********************************************************************
 * Description: BFS performs a Breadth First Search on a specified
 * 	WrestlerList, starting at a specified Wrestler
 * Pre-condition: someWrestler is a Wrestler in roster
 * Post-conditions: The distance values of all Wrestlers in roster have
 * 	been assigned
 * Parameters: roster is a linked list of Wrestlers
 * 	someWrestler is the starting node in roster
 **********************************************************************/
void BFS(struct WrestlerList *roster, struct Wrestler *someWrestler){

    // Initialize queue of Wrestlers
    queue<struct Wrestler *> myQueue;
    myQueue.push(someWrestler);

    struct Wrestler *current;

    while( !myQueue.empty() ){
	current = myQueue.front();
	struct WrestlerNode *someNode = current->rivals->front;
	while( someNode->next != current->rivals->back ){
	    // Move to next Wrestler
	    someNode = someNode->next;
	    struct Wrestler *someWrestler = someNode->someWrestler;
	    if( someWrestler->color == 0 ){
		someWrestler->color = 1;
		someWrestler->d = current->d + 1;
		myQueue.push(someWrestler);
	    }
	}
	current->color = 2;
	myQueue.pop();
    } 

}

/************************************************************************
 * Description: hasValidRivals determines if a given roster can be assigned
 * 	valid babyface/heel rivalries
 * Pre-conditions: A BFS has been performed on roster
 * Post-conditions: Returns true is valid babyface/heel assignments can be
 * 	made, false otherwise
 * Parameters: roster is a linked list of Wresters
 ************************************************************************/
bool hasValidRivals(struct WrestlerList *roster){
    // Initialize result
    bool result = true;
    struct WrestlerNode *current = roster->front;
    while( (current->next != roster->back) && (result) ){
	current = current->next;
	int stable = current->someWrestler->d;
	//cout << "Testing: " << current->someWrestler->name << endl;
	//cout << "Stable: " << stable << endl;
	bool allRivals = true;
	struct WrestlerNode *matchup = current->someWrestler->rivals->front;
	while( (matchup->next != current->someWrestler->rivals->back) && (allRivals) ){
	    //cout << "Inside loop..." << endl;
	    matchup = matchup->next;
	    if( stable == matchup->someWrestler->d ){
		//cout << "Stable: " << matchup->someWrestler->d << endl;
		allRivals = false;
	    }
	}
	result = allRivals;
    }
    return result;
}
