#pragma once

#include "RWLock.h"
#include <chrono>

class BankAccount {
	mutable RWLock m_lock;	// mutable: can be modified even in const methods
	double m_balance = 0;	// bank account balance

public:
	void deposit(double amount) {
		m_lock.lockW();
		this->m_balance += amount;
		m_lock.unlockW();
	}

	double getBalance() const {

		m_lock.lockR();
		double amount = m_balance;
		m_lock.unlockR();
		return amount;
	}

	size_t getReaders() const {
		return m_lock.getReaders();
	}
};