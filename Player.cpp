#include <iostream>
#include "Player.h"
#include "Entry.h"
#include "LotteryComponent.h"
#include <list>
#include <algorithm>
#include <string>
#include <sstream>
#include "Globals.h"
using namespace std;

Player::Player()
{
	weeklyMoneySpent = 0;
	weeklyMoneyEarned = 0;
	totalMoneyEarned = 0;
	totalMoneySpent = 0;
	balance = 50;

}

Player::~Player()
{

}

int Player::getTotalProfit()
{
	return (totalMoneyEarned-totalMoneySpent);
}

int Player::getWeeklyProfit()
{
	return (weeklyMoneyEarned-weeklyMoneySpent);
}



void Player::inputNumbers(int currentWeek)
{ 
	//asks the user if the want to enter into draws
	
	string answer;
	string modifyAnswer;
	string deleteAnswer;
	string futureAnswer;
	string tempAnswer;
	string textToNum;

	int entryToModify;
	int listSize;
	int tempWeek;
	bool balanceOk;


	
	std::cout << "Do you want to enter numbers for this weeks draw? (y/n)" << std::endl;
	getline(cin,answer);

	answer = validateYesNo(answer);

	while (answer == "y")
	{

		balanceOk = checkBalance();
		if (balanceOk = false)
			return;
		
		decreaseBalance(globalConsts.PRICE_OF_ENTRY);

		//asks user to input numbers
		std::cout << "Please input " << globalConsts.NUMBER_OF_BALLS << " unique numbers between " << 
			globalConsts.LOWEST_POSSIBLE_NUMBER << "-" << globalConsts.HIGHEST_POSSIBLE_NUMBER << " to enter into this weeks draw" << std::endl;
		cout << "Type one number per line & press 'Enter' after each one" << endl;
		
		handleUserInput(currentWeek,false);

		std::cout << "Do you want to enter another set of numbers into the draw? (y/n)" << std::endl;
		clearInputBuffer();
		getline(cin,tempAnswer);

		answer = validateYesNo(tempAnswer);
		

	}

	std::cout << "Do you want to enter into a future weeks draw?(y/n)" << std::endl;
	getline(cin,futureAnswer);

	futureAnswer = validateYesNo(futureAnswer);

	while (futureAnswer == "y")
	{

		balanceOk = checkBalance();
		if (balanceOk == false)
			return;

		decreaseBalance(globalConsts.PRICE_OF_ENTRY);
		//stores user input in a string, then converts it to int
		//to handle unexpected input
		cout << "What week do you want to enter into? (current week is " << currentWeek << ".)"<< endl;
		getline(cin, textToNum); 
		stringstream convert(textToNum);
		convert >> tempWeek;

		//checks that the user hasn't entered a negative week
		//or a week that has already happened
		while (tempWeek < 0 || tempWeek <= currentWeek)
		{

			//stores user input in a string, then converts it to int
			//to handle unexpected input
			cout << "Please enter a valid week number" << endl;
			clearInputBuffer();
			getline(cin, textToNum);
			stringstream convert(textToNum);
			convert >> tempWeek;
			
		}


		std::cout << "Please input " << globalConsts.NUMBER_OF_BALLS << " unique numbers between " << 
			globalConsts.LOWEST_POSSIBLE_NUMBER << "-" << globalConsts.HIGHEST_POSSIBLE_NUMBER << " to enter into that weeks draw" << std::endl;	
		cout << "Type one number per line & press 'Enter' after each one" << endl;

		handleUserInput(tempWeek, false);
		

		std::cout << "Do you want to enter another set of numbers into a future draw? (y/n)" << std::endl;
		clearInputBuffer();
		getline(cin,tempAnswer);

		futureAnswer = validateYesNo(tempAnswer);
	}

	//only asks the user if they want to modify entries if there are entries there to modify
	listSize = getListSize();
	if (listSize > 0)
	{
		//decreases listSize by one to prevent off-by-one errors when it is sent to the modifyEntry function
		listSize -=1;
	
		std::cout << "Do you want to modify any of your entries?(y/n)" << std::endl;
		getline(cin,modifyAnswer);
		cout << endl;

		modifyAnswer = validateYesNo(modifyAnswer);

		while (modifyAnswer == "y")
		{
			printEntries();

			cout << "Enter the number that corresponds to the entry you want to modify" << endl;
			clearInputBuffer();
			cin >> entryToModify;


			//validates the answer
			while (entryToModify < 0 || entryToModify > listSize)
			{
				cout << "Please enter a valid number" << endl;
				clearInputBuffer();
				getline(cin, textToNum);
				stringstream convert(textToNum);
				convert >> entryToModify;
			}

			modifyEntry(entryToModify);

			std::cout << "Do you want to modify any other entries? (y/n)" << std::endl;
			getline(cin,modifyAnswer);
			cout << endl;

			modifyAnswer = validateYesNo(modifyAnswer);
		}
	
	}
}

void Player::handleUserInput(int week, bool modifying)
{
	//handles the user entering numbers into a draw

	const int arraySize = globalConsts.NUMBER_OF_BALLS;
	int numHold;
	string textToNum;
	bool numOk;
	int tempArray[arraySize];
	int tempWeek;

	tempWeek = week;

	for (int i = 0; i < arraySize; i++)
	{
		
		//stores user input in a string, then converts it to int
		//to handle unexpected input
		//clearInputBuffer();
		
		
		getline(cin,textToNum);
		
		stringstream convert(textToNum);
		convert >> numHold;
		//checks validity of user input
		numOk = validateNumber(numHold, tempArray);
		
		while (numOk == false)
		{
			std::cout << "The number you have entered is invalid." << std::endl << "Please enter a unique number between " << 
			globalConsts.LOWEST_POSSIBLE_NUMBER << "-" << globalConsts.HIGHEST_POSSIBLE_NUMBER << std::endl;
			clearInputBuffer();
			getline(cin,textToNum);
			stringstream convert(textToNum);
			convert >> numHold;

			numOk = validateNumber(numHold,tempArray);
		}
		
		//if user input is valid, add it to array
		if (numOk == true)
			tempArray[i] = numHold;
	
	}

	//create a new entry using the values sent to it
	createEntry(tempArray,tempWeek);

	//reset tempArray (needs to be cleared each time to check for uniqueness correctly)
	for (int i = 0; i < globalConsts.NUMBER_OF_BALLS; i++)
	{
		tempArray[i] = 0;
	}
}


string Player::validateYesNo(string str)
{
	//validates user y/n input and returns a valid input

	string answer;

	if (str == "y" || str == "n" || str ==  "Y" || str == "N")
	{	answer = str;
		return answer;
	}

	while (str != "y" && str != "n" && str !=  "Y" && str != "N")
	{
		cout << "Please enter a valid answer (y/n)" << endl;
		clearInputBuffer();
		getline(cin,str);
	}

	answer = str;
	return answer;

}

void Player::increaseEarning(int amount)
{
	weeklyMoneyEarned+= amount;
	totalMoneyEarned+= amount;
}

void Player::increaseSpending(int amount)
{
	weeklyMoneySpent+= amount;
	totalMoneySpent+= amount;
}

void Player::setTotalMoneySpent(int amount)
{
	totalMoneySpent = amount;
}

void Player::setTotalMoneyEarned(int amount)
{
	totalMoneyEarned = amount;
}

int Player::getTotalMoneyEarned()
{
	return totalMoneyEarned;
}

int Player::getTotalMoneySpent()
{
	return totalMoneySpent;
}



void Player::print()
{
	//prints the users money spent/earned and profit

	string lossOrProfit;

	if (getWeeklyProfit() <0)
		lossOrProfit = " for a loss of ";
	else
		lossOrProfit = " for a profit of ";
	
	cout << "This week you spent " << (char)156 << weeklyMoneySpent << " and earned " << (char)156 << weeklyMoneyEarned
	<< lossOrProfit << (char)156 << getWeeklyProfit() << endl << endl;

	if (getTotalProfit() <0)
		lossOrProfit = " for a loss of ";
	else
		lossOrProfit = " for a profit of ";

	cout << "In total you have spent " << (char)156 << totalMoneySpent << " and earned " << (char)156 << totalMoneyEarned
	<< lossOrProfit << (char)156 << getTotalProfit() << endl << endl;
	
	cout << "Your current balance is: " << (char)156 << balance << endl << endl;
}



void Player::resetAttributes()
{
	weeklyMoneyEarned = 0;
	weeklyMoneySpent = 0;
}

int * Player::getEntryNumbers()
{
	return (NumItr)->entryNumbers;
}

int Player::getEntryWeek()
{
	return (NumItr)->entryWeek;
}

void Player::setIteratorToFront()
{
	NumItr = entries.begin();
}

void Player::incrementIterator()
{
	//sets deleteItr to NumItr so that the NumItr doesn't end up pointing to a deleted entry
	deleteItr = NumItr;
	NumItr++;
}


void Player::deleteEntry()
{
	entries.erase(deleteItr);
}

int Player::getListSize()
{
	return entries.size();
}


void Player::createEntry(int entryNumbers[6], int entryWeek)
{
	//creates a new entry with the values sent to it and adds it to
	//the entries list

	Entry entry;

	for (int i = 0; i < globalConsts.NUMBER_OF_BALLS; i++)
	{
		entry.entryNumbers[i] = entryNumbers[i];
	}

	entry.entryWeek = entryWeek;

	entries.push_back(entry);

}

void Player::clearInputBuffer()
{
	//clears the cin input buffer
	//code from: http://stackoverflow.com/questions/257091/how-do-i-flush-the-cin-buffer

	//resets stream state
	cin.clear();
	
}

void Player::decreaseBalance(int amount)
{
	balance -= amount;
}

void Player::increaseBalance(int amount)
{
	balance += amount;
}

void Player::setBalance(int amount)
{
	balance = amount;
}

int Player::getBalance()
{
	return balance;
}

bool Player::checkBalance()
{
	//checks if the user has enough money to enter the draw and returns a true/false value
	//if the user doesn't have enough it asks if they want to deposit more money

	string depositAnswer;
	int depositAmount;
	
	if (balance < globalConsts.PRICE_OF_ENTRY)
	{
		cout << "You do not have enough money in your account to enter. Would you like to deposit more money? (y/n)" << endl;
		getline(cin,depositAnswer);
		depositAnswer = validateYesNo(depositAnswer);

		if(depositAnswer == "y")
		{
			cout << "How much do you want to deposit into your account?" << endl;
			cin >> depositAmount;
			increaseBalance(depositAmount);
			return true;

		}

		if (depositAnswer == "n")
			return false;

	}
	else
	{
		return true;
	}
}

void Player::printEntries()
{
	//iterates through the list of entries and prints them to the screen

	setIteratorToFront();
	
	for (int i = 0; i < getListSize(); i++)
	{
		cout << i << ": ";
		cout << "| Week " << NumItr->entryWeek << " | " << endl;
		printArray(NumItr->entryNumbers, globalConsts.NUMBER_OF_BALLS);
		incrementIterator();
	}

	setIteratorToFront();
}

void Player::modifyEntry(int listPos)
{
	//allows the user to modify one of their entries

	setIteratorToFront();

	//takes the listPos value sent to it and uses it to iterate to the correct position in the list
	for (int i = 0; i < listPos; i++)
	{
		incrementIterator();
	}

	clearInputBuffer();
	std::cout << "Please input a new set of " << globalConsts.NUMBER_OF_BALLS << " unique numbers between " << 
			globalConsts.LOWEST_POSSIBLE_NUMBER << "-" << globalConsts.HIGHEST_POSSIBLE_NUMBER << " to enter into that weeks draw" << std::endl;	
		cout << "Type one number per line & press 'Enter' after each one" << endl;

	cin.ignore();

	//creates a new entry for the same week to give the appearance of modification
	handleUserInput(NumItr->entryWeek, true);

	//prevents NumItr from pointing at a deleted entry
	deleteItr = NumItr;
	setIteratorToFront();

	//deletes the old entry
	deleteEntry();
}