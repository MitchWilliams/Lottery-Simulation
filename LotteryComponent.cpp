#include <iostream>
#include <algorithm>
#include "LotteryComponent.h"
#include "Globals.h"

using namespace std;

void LotteryComponent::printArray(int arrayToPrint[], int arraySize)
{
	//outputs an array to the screen

	for (int i = 0; i < arraySize; i++)
	{
		cout << arrayToPrint[i] << ", ";
	}

	cout << endl << endl;
}

bool LotteryComponent::validateNumber(int num, int tempArray[])
{
	//checks the number sent to it to make sure it is within the boundaries
	//and to make sure it is unique to its array

	//defaults the exitCode to true
	//will not change if the number is valid
	bool exitCode = true;

	//tests if the number is within the boundaries
	if ((num < globalConsts.LOWEST_POSSIBLE_NUMBER) || (num > globalConsts.HIGHEST_POSSIBLE_NUMBER))
		exitCode = false;

	//tests if number is unique in its areay
	for (int i = 0; i < globalConsts.NUMBER_OF_BALLS; i++)
	{
		if(num == tempArray[i])
			exitCode = false;
	}



	return exitCode;

}