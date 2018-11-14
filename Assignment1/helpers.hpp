#ifndef HELPERS_HPP
#define HELPERS_HPP

#include <cstdlib>
#include "time.h"
#include <iostream>
#include "assert.h"
#include <string>
#include <fstream>

using namespace std;

const string INPUT_NAME = "data.txt";

void readInputFile(int);
void generateNums(int *, int);
void printArray(int *, int);
void printResults(float *, int);
void saveArray(int, int *, int, bool);
void clearOutputFile(int);
void runtimeTest(float *, int, int);
void mergeSort(int *, int, int);
void merge(int *, int, int, int);
void insertionSort(int *, int);

#endif
