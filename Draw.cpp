#include <iostream>
#include <cmath>
#include <stdlib.h>
#include <time.h>
#include <list>
#include "LotterySystem.h"
#include "LotteryComponent.h"
#include "Player.h"
#include "Draw.h"
#include <fstream>
#include "Globals.h"


using namespace std;

Draw::Draw()
{
	//seeds the random number function with the time
	srand (time(NULL));
}

Draw::~Draw()
{
}


int Draw::generateNumber(int max, int min)
{
	//generates a random number within the upper & lower bounds

	int rnd;
	
	rnd = rand() % max + min;
	
	return rnd;

}

void Draw::setDrawNumbers()
{
	//populates the drawNumbers array with 6 unique numbers

	int numHold; //used to temporarily store the number that is generated
	bool numOk; //used to store whether or not the number is valid

	//sets the first element in the array
	//doesn't validate it as it is guaranteed to be unique
	drawNumbers[0] = generateNumber(globalConsts.HIGHEST_POSSIBLE_NUMBER, globalConsts.LOWEST_POSSIBLE_NUMBER); 

	for (int i = 1; i < globalConsts.NUMBER_OF_BALLS; i++)
	{
		//populates the temporary variable
		numHold = generateNumber(globalConsts.HIGHEST_POSSIBLE_NUMBER, globalConsts.LOWEST_POSSIBLE_NUMBER);
		//sends the number to be checked for validity
		numOk = validateNumber(numHold, drawNumbers);
		
		//repeat the above process while the number generated isn't valid
		while (numOk == false)
		{
			numHold = generateNumber(globalConsts.HIGHEST_POSSIBLE_NUMBER, globalConsts.LOWEST_POSSIBLE_NUMBER);
			numOk = validateNumber(numHold,drawNumbers);
		}

		//if the number is valid, add it to the array	
		if (numOk == true)
			drawNumbers[i] = numHold;
	}
		
}


int * Draw::getDrawNumbers()
{
	return drawNumbers;
}

void Draw::compareNumbers(int numArray[], int numArray2[], int arraySize)
{
	//compares two arrays of numbers and counts how many match

	list<int>numbers1;
	list<int>numbers2;
	list<int>::iterator NumItr;
	list<int>::iterator NumItr2;

	int matchCount = 0;

	//populates the two lists with the arrays sent to the function
	for (int i = 0; i < (arraySize); i++)
	{
		numbers1.push_back(numArray[i]);
		numbers2.push_back(numArray2[i]);
	}

	//compares the two lists, counts how many numbers match
	for (NumItr = numbers1.begin(); NumItr != numbers1.end(); NumItr++)
	{
		
		for (NumItr2 = numbers2.begin(); NumItr2 != numbers2.end(); NumItr2++)
		{
			if (*NumItr == *NumItr2)
				matchCount++;
		}
		
	}
	
	//sets matchedNumbers variable 
	matchedNumbers = matchCount;



}

int Draw::getMatchedNumbers()
{
	return matchedNumbers;
}

void Draw::print()
{
	//outputs the results of the Draw

	cout << "There were " << matchedNumbers << " matched numbers" << endl;

	if (matchedNumbers == 0)
		cout << "You won no money on this line" << endl;
	else
		cout << "You won " << (char)156 << calcPrize() << " on this line" << endl;
	//'(char)156' outputs the '£' sign

	cout << endl;

}

int Draw::calcPrize()
{
	//returns the amount of money earnt

	int prizeMoneyValues[7] = {0,0,3,30,3000,30000,3000000};

	//takes the value of matchedNumbers and returns the corresponding element in the array
	return prizeMoneyValues[matchedNumbers];

}


