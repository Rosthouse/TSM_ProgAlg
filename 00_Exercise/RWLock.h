#pragma once

#include <mutex>

using namespace std;

class RWLock {
    mutex m_mutex;                // re-entrance not allowed
    condition_variable m_readingAllowed, m_writingAllowed;
    bool m_writeLocked = false;    // locked for writing
    size_t m_readLocked = 0;    // number of concurrent readers

public:
    size_t getReaders() const {
        return m_readLocked;
    }

    void lockR() {
        // TODO
        m_readLocked += 1;

    }

    void unlockR() {
        // TODO
        m_readLocked -= 1;
    }

    void lockW() {
        unique_lock<mutex> lock(m_mutex, std::defer_lock);
        lock.lock();
        return;
    }

    void unlockW() {
        unique_lock<mutex> lock(m_mutex, std::defer_lock);
        lock.unlock();
        return;
    }
};