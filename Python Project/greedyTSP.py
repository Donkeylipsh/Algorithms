# Group 43
# Nathaniel Villegas, Daniel Gainey, Colin Van Overschelde
# CS 325
# TSP Group Project
import sys
import math
import random 
import copy
import string

class City:
    def _init_(self):
        self.name = None
        self.xCoord = None
        self.yCoord = None

def readInput(fileName, cities):
    # Open the input file
    inputFile = open(fileName, "r")
    # Loop through each line in the file
    for line in inputFile:
	# Split the line read at each white space
	someCity = line.split()
	currentCity = City()	# Object to hold parsed input
	currentCity.name = someCity[0]	
	currentCity.xCoord = int(someCity[1])
	currentCity.yCoord = int(someCity[2])
	cities.append(currentCity)	# Add city read from file to cities array
    # Close the input file
    inputFile.close()

# finds the distance between two cities and returns a rounded int
def euclideanDistance(currentCity, city):
	distance = 0
	distance = pow(( currentCity.xCoord - city.xCoord), 2) + pow(( currentCity.yCoord - city.yCoord), 2)
	return int(round(math.sqrt(distance)))

def NearestNeighbor(someCities):
    startingCity = (random.choice(someCities))
    currentCity = copy.copy(startingCity)
    totalDistance = 0
    route = []
	# copies list to create a list of unvisited cities
    unvisitedCities = copy.copy(someCities)
	
	# add the starting city to the route and remove it from unvisited list
    for z in unvisitedCities:
		if z == startingCity:
			route.append(z)
			unvisitedCities.remove(z)


	# go through the list until all cities are visited
    while len(unvisitedCities) >= 1:
		distances = []
		closestCity = [0, float("inf")]

		# find the distance of each city from the current location
		for city in unvisitedCities:
			cityData = []
			dist = euclideanDistance(currentCity, city)
			cityData.append(city.name)
			cityData.append(dist)
			distances.append(cityData)

		# find the closest city
		for cities in distances:
			if cities[1] < closestCity[1]:
				closestCity = cities

		# update total distance amount
		totalDistance += closestCity[1]

		# add closest city to list and update current city
		for nearestCity in unvisitedCities:
			if nearestCity.name == closestCity[0]:
				currentCity = copy.copy(nearestCity)
				route.append(nearestCity)
				unvisitedCities.remove(nearestCity)

	# find the distance back home
    distToStart = euclideanDistance(currentCity, startingCity)
    totalDistance += distToStart
    
	# prints the total tour distance and the route
    # print('Total Distance: {}'.format(totalDistance))

    # print("End route:")
    # for x in route:
		# print(x.name)

    baseFileName = sys.argv[1]
    outFileName = baseFileName + ".tour"
    print("Output File: " + outFileName)
    
    outFile = open(outFileName,'w')
    outFile.write(str(totalDistance))
    for x in route:
        outFile.write('\n' + x.name)
    outFile.write('\n')
    outFile.close()


# Initialize array of cities
someCities = []
# Read input from first command line argument
readInput(sys.argv[1], someCities)
# dummy test cases
# city1 = City()
# city2 = City()
# city3 = City()
# city4 = City()
# city1.name = "1"
# city1.xCoord = 0
# city1.yCoord = 0
# city2.name = "2"
# city2.xCoord = 20
# city2.yCoord = 45
# city3.name = "3"
# city3.xCoord = 10
# city3.yCoord = 50
# city4.name = "4"
# city4.xCoord = 5
# city4.yCoord = 5
# someCities = [city1, city2,city3,city4]

# Finds the route using Nearest Neighbor Algorithm
NearestNeighbor(someCities)

	

# Test to confirm input read properly
#for nextCity in someCities:
#    print(nextCity.name)
#    print(nextCity.xCoord)
#    print(nextCity.yCoord)
