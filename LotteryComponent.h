#pragma once

class LotteryComponent
{
public:
virtual void print() = 0;
void printArray(int arrayToPrint[], int arraySize); 
bool validateNumber(int num, int tempArray[]);
};