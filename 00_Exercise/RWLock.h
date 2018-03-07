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
        m_mutex.lock();
        return;
        std::unique_lock<std::mutex> lock(m_mutex);
        lock.lock();
        m_writeLocked = true;
    }

    void unlockW() {
        m_mutex.unlock();
        return;
        std::unique_lock<std::mutex> lock(m_mutex);
        lock.unlock();
        m_writeLocked = false;
        m_writingAllowed.notify_one();
        m_readingAllowed.notify_all();
    }
};