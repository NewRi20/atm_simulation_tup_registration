#include "classes.h"
#include <string>
#include <sstream>
#include <iostream>
#include "MainFrame.h"

using namespace std;

// DEFINITION OF METHODS OF 'Bank' CLASS FROM 'classes.h'

bool Bank::isEmpty()
{
	return (head == nullptr);
}

bool Bank::accountDuplicationChecker(string name, string contactNum)
{
	Node* ptr = head;
	while (ptr)
	{
		if (ptr->account.name == name ||
			ptr->account.contactNumber == contactNum)
		{
			return true;
		}
		ptr = ptr->next;
	}
	return false;
}


string Bank::hashPinCode(const string& pincode, const string& accountNumber)
{
	string transformedPin;
	for (char c : pincode)
	{
		if (isdigit(c))
		{
			int digit = (c - '0' + 7) % 10;
			transformedPin += to_string(digit);
		}
	}

	string encodedPin;
	for (char c : transformedPin)
	{
		char letter = 'A' + (c - '0');
		encodedPin += letter;
	}
	return  accountNumber + "-" + encodedPin;
}


bool Bank::accountExists(const string& hashedaccountNum) const
{
	// Loop through the bank's internal account list
	Node* ptr = head;
	while (ptr)
	{
		if (ptr->account.pincode == hashedaccountNum)
			return true;
		ptr = ptr->next;
	}
	return false;
}

string Bank::getSignificantDigits(const string& accountNumber)
{
	// Strip leading zeros
	std::string strippedAccount = accountNumber;
	strippedAccount.erase(0, strippedAccount.find_first_not_of('0'));

	// If account number is still empty after stripping zeros, return "0"
	if (strippedAccount.empty()) {
		return "0";
	}

	// Return last two digits (or whatever number of digits you want to use for hashing)
	return strippedAccount.substr(max((int)strippedAccount.length() - 2, 0));
}

bool Bank::isCardRegistered()
{
	string filepath = string(1, driveLetter) + ":\\account.txt";

	ifstream inputFile(filepath);
	if (!inputFile.is_open())
		return false;

	else
		return true;
}

void Bank::retrieveAllAccounts()
{
	ifstream inputFile(filename);
	if (!inputFile.is_open())
	{
		return;
	}

	Node* ptr = head;
	string line;
	int maxAccNum = 0; // To track the highest account number
	while (getline(inputFile, line))
	{
		stringstream stream(line);
		string accountNumber, name, pincode, birthday, contactNumber, balanceStr;
		double balance;

		getline(stream, accountNumber, ',');
		getline(stream, name, ',');
		getline(stream, pincode, ',');
		getline(stream, birthday, ',');
		getline(stream, contactNumber, ',');
		getline(stream, balanceStr, ',');

		balance = stod(balanceStr);

		Account account(accountNumber, name, pincode, birthday, contactNumber, balance);
		add(account);

		// Extract account number and find max
		maxAccNum = max(maxAccNum, stoi(accountNumber));
	}

	// Update accNumberCounter for the next new account
	accNumberCounter = maxAccNum + 1; // Increment to the next available number
}

void Bank::saveAllAccounts()
{
	updateAccount(currentAccount);
	ofstream outputFile(filename);

	if (!outputFile.is_open())
	{
		return;
	}

	Node* ptr = head;
	
	while (ptr)
	{
		outputFile << ptr->account.accountNumber << ","
			<< ptr->account.name << ","
			<< ptr->account.pincode << ","
			<< ptr->account.birthday << ","
			<< ptr->account.contactNumber << ","
			<< ptr->account.balance << endl;
		ptr = ptr->next;
	}
	outputFile.close();
}

Account& Bank::searchAccount(string& accountNumber)
{
	Node* current = head;

	while (current) {
		if (current->account.accountNumber == accountNumber)
		{
			return current->account;
		}
		current = current->next;
	}
}

void Bank::updateAccount(Account& updatedAccount)
{
	Node* current = head;

	while (current)
	{
		if (current->account.accountNumber == updatedAccount.accountNumber)
		{
			current->account = updatedAccount;  // Update the account in the node
			return;
		}
		current = current->next;
	}
}

void Bank::add(Account account) 
{
	Node* newNode = new Node(account);
	if (isEmpty())
	{
		newNode->next = nullptr;
		head = newNode;
	}
	else
	{
		newNode->next = head;
		head = newNode;
	}
}
