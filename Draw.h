#pragma once
#include "LotteryComponent.h"

class Draw: public LotteryComponent
{
private:
int drawNumbers[6]; 
int matchedNumbers;

public:

Draw();
~Draw();

void setDrawNumbers();
int * getDrawNumbers();
void compareNumbers(int numArray[], int numArray2[], int arraySize); 
int getMatchedNumbers();
void print(); 
int calcPrize(); 


private:
int generateNumber(int max, int min); 

};