#include <iostream>
#include <cmath>
#include <stdlib.h>
#include <cstdio>
#include <time.h>
#include <list>
#include "LotterySystem.h"
#include "LotteryComponent.h"
#include "Player.h"
#include "Entry.h"
#include "Draw.h"
#include <fstream>
#include <string>
#include "Globals.h"
#include <sstream>

using namespace std;

LotterySystem::LotterySystem()
{
	//sets default values of variables
	week = 1;
	programRunning = true;
	newGame = true;
	
	

}

LotterySystem::~LotterySystem()
{
	
	
	closeLog();

}

int LotterySystem::getWeek()
{
	return week;

}

void LotterySystem::updateWeek()
{
	week++;
}

void LotterySystem::openLog(bool newGame)
{
	//opens up the gameLog ofstream

	//opens up the log in overwrite mode if starting a new game
	//else opens it up in append mode
	if (newGame == true)
		gameLog.open("gamelog.txt", ios::out, ios::trunc);
	else if (newGame == false)
		gameLog.open("gamelog.txt", ios::app);
		
	
}

void LotterySystem::closeLog()
{
	gameLog.close();
}

void LotterySystem::clearLog(int currWeek)
{

	//clears old records from gamelog
	//currently deletes oldest record every week after week 10 so gamelog only stores results
	//for the previous 10 weeks
	closeLog();
	
	ifstream readGameLog("gamelog.txt");
	ofstream printToLog;
	string search = "___________________________________________________________________________";
	string inputString;
	string newFileContents;
	int lineCount = 0; //counts how many header lines have passed (such as the one in the 'search string'
	bool deleteLine = false;

	//while text remains in the document
	while (getline(readGameLog, inputString))
	{

		
		if (inputString == search)
			lineCount++;
		
		//while less that three 'lines' have been encountered, that means that oldest week
		//of the document is still being deleted
		//once the third line is hit, from that point on is what we want to keep
		if (lineCount < 3)
			deleteLine = true;
		else
			deleteLine = false;

		if (deleteLine == false)
		{
			//stores all the text we want to keep in a string
			newFileContents += inputString;
			newFileContents.push_back('\n');
		}

	
	}

	readGameLog.close();

	//writes the text that hasn't been deleted back to the game log
	printToLog.open("gamelog.txt",ios::trunc);
	printToLog << newFileContents;
	printToLog.close();

	//re-opens the gameLog ofstream that is used elsewhere
	openLog(false);

}


int LotterySystem::getEntryPrice()
{
	return globalConsts.PRICE_OF_ENTRY;
}

void LotterySystem::runLottery()
{
	//central function of system

	int forLoopLength;
	int entryWeek;
	bool weeksMatch;
	string menuReturn;

	//checks to see if it is a new game or not
	checkNewGame();

	//opens up the game log
	openLog(newGame);

	//if user wants to load an existing game, load all data from files
	if (newGame == false)
	{	
		loadGameLog();
		loadData();
		loadEntries();
	}
	else
	{
		cout << "You have a starting balance of " << (char)156 << player.getBalance() << endl;
	}
	//main system loop
	while(programRunning == true)
	{
		

		//prints the week header
		printHeader();

		//asks user for input and handles input
		player.inputNumbers(getWeek());

		//sets the iterator to the front of the entries list
		player.setIteratorToFront();

		//goes through the entire entries list and
		//increases the amount the user has spent by the entry price
		//for every entry they have in the current week
		for (int i = 0; i < player.getListSize(); i++)
		{
			
			entryWeek = player.getEntryWeek();
			weeksMatch = compareInts(week, entryWeek);
			player.incrementIterator();
			if (weeksMatch == true)
			{
				player.increaseSpending(getEntryPrice());
			}
		}
		
		
		//output to let the user what is happening as there can be a slight delay
		//when generating lottery numbers
		cout << endl << "Generating lottery results..." << endl << endl;

		//generates the lottery numbers
		draw.setDrawNumbers();
		
		//prints out the lottery numbers
		printWeeklyNumbers();
		//writes the numbers to the gameLog
		saveNumbers();

		player.setIteratorToFront();
		
		//for every entry in the list...
		forLoopLength = player.getListSize();
		for (int i = 0; i < forLoopLength; i++)
		{
			//check if the week is the same as the current system week
			entryWeek = player.getEntryWeek();
			weeksMatch = compareInts(week, entryWeek);
			//if it is...
			if(weeksMatch == true)
			{	
				//print the entry numbers
				player.printArray(player.getEntryNumbers(),globalConsts.NUMBER_OF_BALLS);
				//compare the numbers with the numbers drawn
				draw.compareNumbers(draw.getDrawNumbers(),player.getEntryNumbers(),globalConsts.NUMBER_OF_BALLS);
				//print out how much was won
				draw.print();
				//save the results to the gameLog
				saveResults();
				//increase player earnings by any prize one
				player.increaseEarning(draw.calcPrize());
				player.increaseBalance(draw.calcPrize());
				//increment the iterator
				player.incrementIterator();
				//delete the entry from the list
				player.deleteEntry();
				

			}

		}
		
		//print out the player's money won/lost
		player.print();
		//resets the players weekly money won/lost attributes
		player.resetAttributes();

		

		//runs the clearLog function once the maximum amount of weeks have been saved
		if (week > MAX_NUM_OF_RECORDS)
			clearLog(week);

		updateWeek();

		//saves the variable states and current entry list to files
		saveData();
		saveEntries();


		

	}

	

}

void LotterySystem::printWeeklyNumbers()
{
	//prints out the lottery numbers for the week

	cout << "The lottery numbers for week " << getWeek() << " were: " << endl;
	
	draw.printArray(draw.getDrawNumbers(),globalConsts.NUMBER_OF_BALLS);


	cout << "Your numbers for week " << getWeek() << " were: " << endl;
}

void LotterySystem::saveNumbers()
{
	//saves the lottery numbers to the gameLog
	//ofstream gameLog("gamelog.txt");


	gameLog << "___________________________________________________________________________" << endl;
	gameLog << "Week " << getWeek() << endl;
	gameLog << "___________________________________________________________________________" << endl;

	gameLog << "The lottery numbers for week " << getWeek() << " were: ";

	printArrayToLog(draw.getDrawNumbers(),globalConsts.NUMBER_OF_BALLS);

	gameLog << endl;

	gameLog << "Your numbers for week " << getWeek() << " were: ";

	gameLog << endl;

	
}

void LotterySystem::saveResults()
{
	//saves the player numbers & how much money they win to the gameLog



	printArrayToLog(player.getEntryNumbers(),globalConsts.NUMBER_OF_BALLS);

	gameLog << endl;
	
	gameLog << "There were " << draw.getMatchedNumbers() << " matched numbers" << endl;

	if (draw.getMatchedNumbers() == 0)
		gameLog << "You won no money on this line" << endl;
	else
		gameLog << "You won " << (char)156 << draw.calcPrize() << " on this line" << endl;

	gameLog << endl << endl;


	

}

void LotterySystem::printArrayToLog(int arrayToPrint[], int arraySize)
{
	//takes the array sent to it and prints it to the gameLog


	for (int i = 0; i < arraySize; i++)
	{
		gameLog << arrayToPrint[i] << ", ";
	}

	gameLog << endl;

}


void LotterySystem::checkNewGame()
{
	//checks if the user wants to start a new game or not

	ifstream fileCheckGame("gamelog.txt");
	ifstream fileCheckData("datalog.txt");
	ifstream fileCheckEntries("entrylog.txt");
	string answer;

	//if the three save files exist, ask the player whether they want to load them or not
	//if any one of the save files don't exist then newGame will remain true (its default value)
	//because loading an existing game is not possible
	if (fileCheckGame && fileCheckData && fileCheckEntries)
	{
		cout << "Do you want to load existing game? (y/n)" << endl << "Entering 'n' will cause previous data to be overwritten. " << endl;
		getline(cin,answer);
		cin.clear();

		while (answer != "y" && answer != "n" && answer != "N" && answer != "Y")
		{
			cout << "Please choose 'y' or 'n'" << endl;
			getline(cin,answer);
			cin.clear();

		}

		if (answer == "y" || answer == "Y")
			newGame = false;
		else if (answer == "n" || answer == "N")
			newGame = true;
	}

	fileCheckGame.close();
	fileCheckData.close();
	fileCheckEntries.close();
}

void LotterySystem::loadGameLog()
{
	//takes the text stored in the game log and prints it to the screen

	string inputString;
	string fileContents;
	ifstream readGameLog("gamelog.txt");

	//while text remains in the gameLog
	//get the text from the current line
	// append it to the fileContents variable
	//add a new line to the fileContents variable
	//code from: http://stackoverflow.com/questions/13035674/how-to-read-line-by-line-or-a-whole-text-file-at-once

	
	while (getline(readGameLog, inputString))
	{
		
		fileContents += inputString;
		fileContents.push_back('\n');
	}

	cout << fileContents;

	readGameLog.close();
}

void LotterySystem::saveData()
{
	//saves variable states to a file

	ofstream dataLogOut;
	dataLogOut.open("datalog.txt");

	dataLogOut << week << endl;
	dataLogOut << player.getTotalMoneySpent() << endl;
	dataLogOut << player.getTotalMoneyEarned() << endl;
	dataLogOut << player.getBalance() << endl;

	dataLogOut.close();
}

void LotterySystem::saveEntries()
{
	//saves entries to a file

	ofstream entryLogOut;
	entryLogOut.open("entrylog.txt", ios::trunc);

	//stores the number of entries in the list
	entryLogOut << player.getListSize() << endl;

	player.setIteratorToFront();

	//for each entry in the list
	//store the week, then each number on its own line (for reading in purposes)
	for (int i = 0; i < player.getListSize(); i++)
	{
		entryLogOut << player.getEntryWeek() << endl;

		for (int i = 0; i < globalConsts.NUMBER_OF_BALLS; i++)
		{
			entryLogOut << player.getEntryNumbers()[i] << endl;
		}

		player.incrementIterator();
	}
}

void LotterySystem::loadEntries()
{
	//loads entries from file

	ifstream entryLogIn;
	entryLogIn.open("entrylog.txt");

	int listSize;
	int tempWeek;
	int tempArray[globalConsts.NUMBER_OF_BALLS];

	//loads the listSize from the first line of the file
	entryLogIn >> listSize;

	//repopulates the entries list by reading the values from the file
	for (int i = 0; i < listSize; i++)
	{
		entryLogIn >> tempWeek;

		for (int x = 0; x < globalConsts.NUMBER_OF_BALLS; x++)
		{
			entryLogIn >> tempArray[x];
		}

		player.createEntry(tempArray,tempWeek);
	}

}
void LotterySystem::loadData()
{
	//loads variables from file

	int tempSpent;
	int tempEarned;
	int tempBalance;

	//creates an input stream
	ifstream dataLogIn;

	//opens the data file
	dataLogIn.open("datalog.txt");

	//sets variables to the values stored in file
	dataLogIn >> week;
	dataLogIn >> tempSpent;
	dataLogIn >> tempEarned;
	dataLogIn >> tempBalance;

	//sets moneyEarned & spent
	player.setTotalMoneyEarned(tempEarned);
	player.setTotalMoneySpent(tempSpent);
	player.setBalance(tempBalance);

	//closes data stream
	dataLogIn.close();
}

void LotterySystem::printHeader()
{
	//outputs the weekly header

	cout << "___________________________________________________________________________" << endl;
	cout << "Week " << getWeek() << endl;
	cout << "___________________________________________________________________________" << endl; 
}

bool LotterySystem::compareInts(int num1, int num2)
{
	//compares two numbers and returns whether they are equal or not

	if (num1 == num2)
		return true;
	else
		return false;
}


