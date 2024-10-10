#include "classes.h"
#include <string>
#include <sstream>
#include <iostream>
using namespace std;

bool Bank::withraw(double amount) {
	if (amount <= 0) {
		throw invalid_argument("Amount must be greater than 0");
		return false;
	}
	if (currentAccount.balance - amount <= 0) {
		throw out_of_range("Amount exceeded the current balnce");
		return false;
	}
		currentAccount.balance -= amount;
		return true;
}

bool Bank::deposit(double amount) {
	if (amount > 1000000000000) {
		throw invalid_argument("Amount exceeded the Limit");
	}
	if (amount <= 0) {
		throw exception("Amount must be greater than 0");
	}
		currentAccount.balance += amount;
		return true;
}

bool Bank::fundTransfer(const string& receiverAccountNumber, double amount)
{
	// If valid amount
	if (amount <= 0) {
		throw invalid_argument("Amount must be greater than 0");
	}
	if (currentAccount.balance - amount <= 0) {
		throw out_of_range("Amount exceeded the current balnce");
	}

	// Find the receiver account
	Node* current = head;
	while (current != nullptr) {
		if (current->account.accountNumber == receiverAccountNumber) {
			currentAccount.balance -= amount; 
			current->account.balance += amount; 
			return true; 
		}
		current = current->next;
	}
	throw exception("Account Number Not Found");
	return false; 
}

bool Bank::changePincode(const string& oldP, const string& newP)
{
	if (oldP.empty() || newP.empty()) {
		throw exception("Input Empty");
	}
	if (currentAccount.pincode != oldP) {
		throw invalid_argument("Incorrect Old PIN");
	}
	if (newP.length() != 8) {
		throw out_of_range("The PIN must be 6-Digits long");
	}
	currentAccount.pincode = newP;
	
	return true;


}

bool Bank::isNumber(const string& str) {
	for (char c : str) {
		if (!isdigit(c)) {
			return false;
		}
	}
	return true;
}


