#pragma once
#include "Entry.h"
#include "LotteryComponent.h"
#include <list>

using namespace std;

class Player : public LotteryComponent
{
private:
int weeklyMoneySpent;
int weeklyMoneyEarned;
int totalMoneySpent;
int totalMoneyEarned;
int balance;

list<Entry>entries;
list<Entry>::iterator NumItr;
list<Entry>::iterator deleteItr;

public:

Player();
~Player();

void inputNumbers(int currentWeek);
void increaseSpending(int amount); 
void increaseEarning(int amount);
void increaseBalance(int amount);
void decreaseBalance(int amount);
void print();
void resetAttributes();

void setIteratorToFront();
void incrementIterator();
int * getEntryNumbers();
int getEntryWeek();
void deleteEntry();
int getListSize();
void createEntry(int entryNumbers[], int entryWeek);
void modifyEntry(int listPos);
void printEntries();

int getTotalMoneySpent();
int getTotalMoneyEarned();
int getBalance();
void setTotalMoneySpent(int amount);
void setTotalMoneyEarned(int amount);
void setBalance(int amount);
bool checkBalance();



int getTotalProfit();
int getWeeklyProfit();

private:

string validateYesNo(string str);
void handleUserInput(int week, bool modifying);
void clearInputBuffer();
};