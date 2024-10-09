#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <fstream>
#include <string>
#include <sstream>
#include <iostream>
#include <wx/string.h>
#include <wx/wx.h>



using namespace std;

struct Account
{
	string accountNumber;
	string name;
	string pincode;
	string birthday;
	string contactNumber;
	double balance;

	Account() : accountNumber("00000"), name("default"), pincode("123456"), birthday("01-01-1900"), contactNumber("+63 000-000-0000"), balance(5000) {}
	Account(string accNumber, string _name, string pin, string bday, string contact, double _balance) : accountNumber(accNumber), name(_name), pincode(pin), birthday(bday), contactNumber(contact), balance(_balance) {}
};

struct Node
{
	Account account;
	Node* next;
	Node() : next(nullptr) {}
	Node(Account _account) : account(_account), next(nullptr) {}
};

class Bank
{
private:
	Node* head;
	
	int accNumberCounter;

	string filename = "allAccounts.csv";
	char driveLetter = 'D';

	bool isEmpty();

public:
	Account& currentAccount;

	char getDriveLetter() const { return driveLetter; }

	Bank(Account& admin) : head(nullptr), currentAccount(admin), accNumberCounter(0) {}

	string getSignificantDigits(const string& accountNumber);

	bool isCardRegistered();
	
	void retrieveAllAccounts();
	void saveAllAccounts();

	bool accountDuplicationChecker(string name, string contactNum);
	bool accountExists(const string& hashedaccountNum) const;

	void add(Account account);

	string generateAccountNumber() {
		// Generate the account number in 5-digit format with leading zeros
		return wxString::Format("%05d", accNumberCounter++).ToStdString();
	}

	string hashPinCode(const string& pincode, const string& accountNumber);
	
};

