#pragma once
#include "Player.h"
#include "Draw.h"
#include "LotteryComponent.h"
#include "Entry.h"
#include <fstream>

using namespace std;

class LotterySystem
{
private:
Player player;
Draw draw;
int week;
bool programRunning; 
ofstream gameLog;
bool newGame; //used to check whether or not it is a new game or if a pre-existing game is being loaded
static const int MAX_NUM_OF_RECORDS = 10;



public:

LotterySystem();
~LotterySystem(void);

int getWeek(); 
void updateWeek(); 
void openLog(bool newGame);
void closeLog();
void clearLog(int currWeek);
void loadData();
void loadGameLog();
void loadEntries();
void saveEntries();
void runLottery();
void printWeeklyNumbers();
void printHeader();
void printArrayToLog(int arrayToPrint[], int arraySize);
int getEntryPrice();
void saveNumbers(); 
void saveResults();
void saveData();
void checkNewGame();
bool compareInts(int num1, int num2);


};