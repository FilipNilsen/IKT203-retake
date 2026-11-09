// Submission-01.cpp : Defines the entry point for the application.
//

#include "main.h"
#include "ReadNames.h"		 // For reading names from file
#include "BankAccount.h"	 // For TBankAccount and EBankAccountType
#include "BankAccountList.h" // For TLinkedList
#include <string>			 // For std::getline and std::string
#include <iostream>			 // For std::cout
#include <sstream>			 // For std::istringstream
#include <fstream>
#include "TSummary.h"

static TSummary statistics;

static EBankAccountType getRandomAccountType()
{
	return static_cast<EBankAccountType>(rand() % 5); // Randomly returns one of the 5 account types
}

TLinkedList* bankAccounts = new TLinkedList(true); // List owns the TBankAccount objects
TBankAccount** bankAccountArray = nullptr;


static bool OnNameRead(const std::string& firstName, const std::string& lastName)
{
	//For each name read, create from 5 to 10 random bank accounts
	int accountCount = rand() % 6 + 5; // Random number between 5 and 10
	for (int i = 0; i < accountCount; i++)
	{
		EBankAccountType accType = getRandomAccountType();
		TBankAccount* newAccount = new TBankAccount(accType, firstName, lastName);
		bankAccounts->Add(newAccount);
	}
	return true; //bankAccounts->getSize() < 100; // For demo purposes
}

static void resetStatistics()
{
	statistics.comparisonCount = 0;
	statistics.timeTaken = (static_cast<double>(clock())) / CLOCKS_PER_SEC;
}

static void printStastics() {
	statistics.timeTaken = (static_cast<double>(clock())) / CLOCKS_PER_SEC - statistics.timeTaken;
	std::cout << "Comparisons: " << statistics.comparisonCount << ", Time taken: " << statistics.timeTaken << " seconds." << std::endl;
}

static void printStastics(std::ofstream& f) {
	statistics.timeTaken = (static_cast<double>(clock())) / CLOCKS_PER_SEC - statistics.timeTaken;
	std::cout << "Comparisons: " << statistics.comparisonCount << ", Time taken: " << statistics.timeTaken << " seconds." << std::endl;
	f << "Comparisons: " << statistics.comparisonCount << ", Time taken: " << statistics.timeTaken << " seconds." << std::endl;
}

/*
Part 3: Standalone Search Functions (The External Analyst)
To simulate working with data from different perspectives, you will also implement search functions that are not part of the list class. These functions will operate on a simple array of pointers.
*/

/**
@brief Finds a bank account from a C array of TBankAccount*.
@param accountArray A C-array of TBankAccount*.
@param arraySize The size of the accountArray.
@param accountNumber std::string with the number of the acccount you're looking for.
@returns nullptr if the account wasn't found, otherwise it's a pointer to the found TBankAccount.
*/
static TBankAccount* FindAccountByNumber(TBankAccount** accountArray, int arraySize, const std::string& accountNumber) {
	if (accountArray == nullptr || arraySize <= 0) return nullptr;
	for (int i = 0; i < arraySize; i++) {
		statistics.comparisonCount++;
		if (accountArray[i]->getAccountNumber() == accountNumber) {
			return accountArray[i]; // Found
		}
	}
	return nullptr; // Not found
}

auto logFile = std::ofstream("results.txt", std::ios::app);

/**
@brief Prints every account in a given date range to cout.
@param accountArray C-array of TBankAccount*.
@param arraySize Size of accountArray.
@param from Start of the date rang.
@param to End of the date range.
@note The timing gets pretty high because it includes printing to the terminal, commenting out that code when running this should give a fairer timing of this function.
*/
static void PrintEveryAccountInDateRange(TBankAccount** accountArray, int arraySize, time_t from, time_t to) {
	if (accountArray == nullptr || arraySize <= 0) return;
	std::cout << "------------------------------" << std::endl;
	resetStatistics();
	int foundCount = 0;
	for (int i = 0; i < arraySize; i++) {
		statistics.comparisonCount++;
		time_t ts = accountArray[i]->getCreationTimestamp();
		if (ts >= from && ts < to) {
			std::cout << i + 1 << ". ";
			accountArray[i]->printAccountInfo();
			foundCount++;
		}
	}
	printStastics(logFile);
	std::cout << "Total accounts found in date range: " << foundCount << std::endl;
}

/**
@implements FCompareAccount
*/
static bool SearchAccountNumberCB(TBankAccount* account, void* searchKey) {
	if (*(std::string*)searchKey == account->getAccountNumber()) {
		return true;
	}
	return false;
}

int main()
{
	srand(time(0)); //Otherwise the random numbers will be the same each time.

	std::cout << "--- Submission 4: Sort & Search ---" << std::endl;

	// Test TBankAccount
	//Gen random account type
	//Change this name for you own names file
	//std::string namesFile = "F:\\IKT203\\VisualStudio\\DATA\\Random_Name.txt";
	std::string namesFile = "C:/Users/Filip/Code/ikt203-teacher-git/Submissions/Submission-04/random_names.txt";
	std::cout << "Reading names from file: " << namesFile << std::endl;
	readNamesFromFile(namesFile, OnNameRead);
	std::cout << "Total Bank Accounts Created: " << bankAccounts->getSize() << std::endl;
	std::cout << "Converting linked list to array..." << std::endl;
	bankAccountArray = bankAccounts->ToArray();
	std::cout << "Array created with " << bankAccounts->getSize() << " accounts." << std::endl;

	std::cout << "Using TLinkedList::Find() to find a random account\n";

	TSummary* a = new TSummary;
	int getRandomIndex = rand() % bankAccounts->getSize();
	std::string* accNum = new std::string(bankAccountArray[getRandomIndex]->getAccountNumber());
	auto account = bankAccounts->Find(&SearchAccountNumberCB, accNum, *a);

	if (account)
	{
		std::cout << "Found Account: " << std::endl;
		account->printAccountInfo();
	}
	else
	{
		std::cout << "Account not found." << std::endl;
	}

	std::cout << *a;
	logFile << *a;

	delete a;
	delete accNum;

	/*
	// Insane stress test to get data for a csv.
	for (size_t i = 0; i < 20000;++i) {
		a = new TSummary;
		int getRandomIndex = rand() % bankAccounts->getSize();
		accNum = new std::string(bankAccountArray[getRandomIndex]->getAccountNumber());
		auto account = bankAccounts->Find(&SearchAccountNumberCB, accNum, *a);
		logFile << a->comparisonCount << "," << a->timeTaken << "\n";
		delete a;
		delete accNum;
	}
	*/

	std::cout << "Using TLinkedList::Find() to find a non-existing account\n";

	a = new TSummary;
	account = bankAccounts->Find(
		&SearchAccountNumberCB,
		new std::string(std::to_string(rand() % 9000 + 1000) + "." + toString(rand() % 90 + 10) + "." + toString(rand() % 90000 + 10000)),
		*a);

	if (account)
	{
		std::cout << "Found Account: " << std::endl;
		account->printAccountInfo();
	}
	else
	{
		std::cout << "Account not found." << std::endl;
	}

	std::cout << *a;
	logFile << *a;

	delete a;

	std::cout << "Demonstration of the standalone FindAccountByNumber(), trying to look for an random account.\n";
	//std::cin.get();

	resetStatistics();
	getRandomIndex = rand() % bankAccounts->getSize();
	TBankAccount* foundAccount = FindAccountByNumber(bankAccountArray, bankAccounts->getSize(), bankAccountArray[getRandomIndex]->getAccountNumber());
	if (foundAccount)
	{
		std::cout << "Found Account: " << std::endl;
		foundAccount->printAccountInfo();
	}
	else
	{
		std::cout << "Account not found." << std::endl;
	}
	printStastics(logFile);

	std::cout << "\nDemonstration of FindAccountByNumber() trying to find an account that doesn't exist.\n";
	//std::cin.get();

	resetStatistics();
	foundAccount = FindAccountByNumber(bankAccountArray, bankAccounts->getSize(), "1234.56.78901");
	if (foundAccount)
	{
		std::cout << "Found Account: " << std::endl;
		foundAccount->printAccountInfo();
	}
	else
	{
		std::cout << "Account not found." << std::endl;
	}
	printStastics(logFile);

	// Find All (Integrated): Use your Every() method to find all accounts created in June 2024 and print their details.
	resetStatistics();
	struct June2024Key {
		time_t start;
		time_t end;
	};
	June2024Key juneKey{};
	std::tm fromToTm = {};
	fromToTm.tm_year = 2024 - 1900; // Year since 1900
	fromToTm.tm_mon = 5; // June (0-based)
	fromToTm.tm_mday = 1; // 1st
	fromToTm.tm_hour = 0;
	fromToTm.tm_min = 0;
	fromToTm.tm_sec = 0;
	juneKey.start = _mkgmtime(&fromToTm); // Use _mkgmtime for UTC
	fromToTm.tm_mday = 30; // 30th
	fromToTm.tm_hour = 23;
	fromToTm.tm_min = 59;
	fromToTm.tm_sec = 59;
	juneKey.end = _mkgmtime(&fromToTm); // Use _mkgmtime for UTC

	std::cout << "\nUsing TLinkedList::Every() to find all accounts created in June.";
	//std::cin.get();

	//FCompareAccount is provided as a lambda function in here.
	TLinkedList* juneAccounts = bankAccounts->Every(
		[](TBankAccount* account, void* searchKey) -> bool {
			June2024Key* key = static_cast<June2024Key*>(searchKey);
			time_t ts = account->getCreationTimestamp();
			return ts >= key->start && ts < key->end;
		}, &juneKey);

	std::cout << "Accounts created in June 2024: " << juneAccounts->getSize() << std::endl;
	printStastics(logFile);

	std::cout << "\nUsing TLinkedList::forEach() on the linked list created to print every one.";
	//std::cin.get();

	juneAccounts->forEach(
		[](TBankAccount* aAccount, int aIndex) {
			std::cout << aIndex + 1 << ". ";
			aAccount->printAccountInfo();
		});

	std::cout << "\nUsing PrintEveryAccountInDateRange() to do the same as above, but now operateing on a C-array.";
	//std::cin.get();

	PrintEveryAccountInDateRange(bankAccountArray, bankAccounts->getSize(), juneKey.start, juneKey.end);


	// Cleanup
	// First delete the array, then the linked list
	delete[] bankAccountArray;
	delete bankAccounts;

	logFile << std::endl;
	logFile.close();

	return 0;
}
