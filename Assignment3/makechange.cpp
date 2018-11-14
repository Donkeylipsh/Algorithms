/***********************************************************************************
 * Author: Colin Van Overschelde
 * Date: 4/22/2018
 * Description: makechange.cpp demonstrates reading values from a file called "amount.txt"
 * 		and using dynamic programming to calculate the minimum number of coins 
 * 		to make change for a value, and saves the results to a file called "change.txt"
 ***********************************************************************************/

#include "makechange.hpp"
#include <iostream>
#include <stdlib.h>
#include "readFile.hpp"
#include <vector>
#include <string>
#include <time.h>

using namespace std;

int getNumber();
int* getCoinValues(int *);
void displayArray(int *, int);
void printResults(int *, int);
void runtimeAnalysis();

/******************************************************************************
 * Description: main() calls the required functions to execute makechange.cpp
 ******************************************************************************/
int main(){
    // Greet user
    cout << endl << "Welcome to makechange" << endl;
    cout << "Programmed by Colin Van Overschelde" << endl << endl;

    // Prepare output file
    remove("change.txt");

    // Read values from file, calculate minimum coins and save output to a file
//    vector<int> myIntVector;
    readAmount();
    
    cout << "Results have been written to \"change.txt\"" << endl;

    cout << endl << "Thank you for using makechange, goodbye" << endl;

    //runtimeAnalysis();

    return 0;
}
/*
int getNumber(){
    cout << "Enter the total value you want to make change for: ";
    int number;
    cin >> number;
    return number;
}

int* getCoinValues(int *coinCount){
    cout << "How many different coins are there? ";
    cin >> *coinCount;

    int* coins = new int[*coinCount];

    for(int i = 0; i < *coinCount; i++){
	cout << "Enter a value for the next coin: ";
	cin >> coins[i];
    }

    return coins;
}

void displayArray(int *arr, int size){
    for(int i =0; i < size; i++){
	cout << "i[" << i << "] = " << arr[i] << endl;
    }
}

void runtimeAnalysis(){
    cout << endl << "* Runtime Analysis *" << endl << endl;
    vector<int> denominations = { 1, 5, 10, 25 };

    cout << "Fixed denominations, increasing n" << endl;

    int n = 100000;
    for( int i = 0; i < 10; i++ ){
	clock_t timer;
	timer = clock();
	makeChange(n, denominations);
	timer = clock() - timer;
	float dur = static_cast<float>(timer) / CLOCKS_PER_SEC * 1000;
	cout << "n[" << n << "] = " << dur << "ms" << endl;
	n += 100000;
    }

    denominations.clear();

    cout << "Increasing denominations, fixed n" << endl;
    n = 100000;
    for( int i = 1; i < 100; i += 2 ){
	denominations.push_back(i);
	clock_t timer;
	timer = clock();
	makeChange(n, denominations);
	timer = clock() - timer;
	float dur = static_cast<float>(timer) / CLOCKS_PER_SEC * 1000;
	if( i % 5 == 0 ){
	    cout << "Coins[" << denominations.size() << "] = " << dur << endl;
    	}
    }

    denominations.clear();

    cout << "Increasing denominations, increasing n" << endl;
    n = 100000;
    for( int i = 1; i < 100; i += 2 ){
	denominations.push_back(i);
	clock_t timer;
	timer = clock();
	makeChange(n, denominations);
	timer = clock() - timer;
	float dur = static_cast<float>(timer) / CLOCKS_PER_SEC * 1000;
	if( i % 5 == 0 ){
	    cout << "Coins[" << denominations.size() << "], n[" << n << "] = " << dur << endl;
	}
	n += 50000;
    }
}
*/

/**********************************************************************************
 * Description: makeChange() calculates the minimum number of coins used to make change
 * 		for a given value, total.  And also calculates and saves the coins used
 * 		in the optimal solution
 * Parameters: total is the value to make change for
 * 	       coinType is a vector of coin denominations, starting from 1, in increasing order
 * Pre-conditions: n/a
 * Post-conditions: Output is written to "change.txt"
 *************************************************************************************/
void makeChange(int total, vector<int> coinType){
    //cout << "********************************************" << endl;
    //cout << "makeChange() started..." << endl;
    //cout << "********************************************" << endl;

    //cout << "* Total value: " << total << endl;

    // Create arrays to hold results
    int *leastCoins = new int[total + 1];
    int *lastCoins = new int[total + 1];

    // Initialize results arrays
    for( int i = 0; i <= total; i++ ){
	leastCoins[i] = 0;
	lastCoins[i] = 0;
    }

    // For each amount less than total amount
    for( int i = 1; i <= total; i++){
	//cout << "* Calculating minCoins for: " << i << endl;
	int minCoins = total + 1;

	int j = coinType.size();
	// For each coin denomination, starting from the largest
	for(vector<int>::reverse_iterator it = coinType.rbegin(); it != coinType.rend(); ++it){
	    
	    //cout << "** Checking coin: " << *it << endl;
	    //cout << "** Denomination Index: " << j << endl;
	    // Check if the coin can be part of the solution
	    if( *it <= i ){
		// If yes, calculate least coins for solutions including current coin
		int nextMinCoins = leastCoins[i - *it] + 1;
		
		if( nextMinCoins < minCoins ){
		   // cout << "***** New min coins" << endl;
		    //cout << "***** Last coin: " << *it << endl;
		    minCoins = nextMinCoins;
		    lastCoins[i] = j;
		}
	    }
	    j--;
	}
	leastCoins[i] = minCoins;
    }

    // Write coin counts to file
    // Initialize coin counts array
    int change[coinType.size()];
    for( int i = 0; i < coinType.size(); i++ ){
	change[i] = 0;
    }
    // Create change array
    int curIndex = total;
    while( curIndex > 0 ){
	// Increment the coin count at index curIndex - 1
	change[lastCoins[curIndex] - 1]++;

	// Update curIndex
	curIndex = curIndex - coinType.at( lastCoins[curIndex] - 1 );
    }
    string coinCounts = to_string(change[0]);
    // Convert array to string
    for( int i = 1; i < coinType.size(); i++ ){
	coinCounts = coinCounts + " " + to_string(change[i]);
    }
    writeFile(coinCounts);


    //cout << "Change Array:" << endl;
    //displayArray(change, coinType.size() );

    //cout << "Least coin array:" << endl;
    //displayArray(leastCoins, total + 1);

    writeFile(to_string(leastCoins[total]));

    //cout << "Minimum coins: " << leastCoins[total] << endl;
    //cout << endl << "**************************************" << endl << endl;

    delete[] leastCoins;
    delete[] lastCoins;
}
