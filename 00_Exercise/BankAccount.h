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
		this_thread::sleep_for(chrono::milliseconds(100*rand()/RAND_MAX));
		m_lock.unlockW();
	}

	double getBalance() const {
		m_lock.lockR();
		double amount = m_balance;
        this_thread::sleep_for(chrono::milliseconds(200*rand()/RAND_MAX));
        m_lock.unlockR();
		return amount;
	}

	size_t getReaders() const {
		return m_lock.getReaders();
	}
};