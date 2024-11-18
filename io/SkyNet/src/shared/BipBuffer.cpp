#include "BipBuffer.h"

#include <cstdlib>
#include <cstring>

BipBuffer::BipBuffer() {}

BipBuffer::~BipBuffer() {
    free(m_buffer);
    m_buffer = nullptr;
}

/*
 * Read bytes from the buffer
 * @param dest pointer to destination where bytes will be written
 * @param bytes number of bytes to read
 * @return true if there was enough data , false otherwise
 */
bool BipBuffer::Read(void* dest, size_t bytes) {
    if (!m_buffer) {
        return false;
    }

    // copy as much out of region A
    size_t cnt = bytes;
    size_t aRead = 0, bRead = 0;

    if (m_regionASize + m_regionBSize < bytes) {
        return false;
    }

    // If we have both region A and region B, always "finish" off region A first
    // as this will contain the "oldest" data
    if (m_regionASize > 0) {
        aRead = (cnt > m_regionASize) ? m_regionASize : cnt;
        memcpy(dest, m_regionAPtr, aRead);
        m_regionASize -= aRead;
        m_regionAPtr += aRead;
        cnt -= aRead;
    }

    // Data left over? read the data from buffer B
    if (cnt > 0 && m_regionBSize > 0) {
        bRead = (cnt > m_regionBSize) ? m_regionBSize : cnt;
        memcpy((uint8_t*)dest + aRead, m_regionBPtr, bRead);
        m_regionBSize -= bRead;
        m_regionBPtr += bRead;
        cnt -= bRead;
    }

    // is buffer A empty? move buffer B to buffer A, to increase future performanace
    if (m_regionASize == 0) {
        if (m_regionBSize > 0) {
            // push it all to the start of buffer
            if (m_regionBPtr != m_buffer) {
                memmove(m_buffer, m_regionBPtr, m_regionBSize);
            }

            m_regionAPtr = m_buffer;
            m_regionASize = m_regionBSize;
            m_regionBPtr = nullptr;
            m_regionBSize = 0;

        } else {
            // no data in region B
            m_regionBPtr = nullptr;
            m_regionBSize = 0;
            m_regionAPtr = m_buffer;
            m_regionASize = 0;
        }
    }

    return true;
}

/*
 * Write bytes to the buffer
 * @param data pointer to the data to be written
 * @param bytes number of bytes to be writtern
 * @return true if was successful ,otherwise false
 */
bool BipBuffer::Write(const void* data, size_t bytes) {
    if (!m_buffer) {
        return false;
    }

    // If buffer B exists, write to it
    if (m_regionBPtr != nullptr) {
        if (GetBFreeSpace() < bytes) {
            return false;
        }

        memcpy(&m_regionBPtr[m_regionBSize], data, bytes);
        m_regionBSize += bytes;
        return true;
    }
    // otherwise, write to Buffer A, or initialize buffer B depending on which was more space
    if (GetAFreeSpace() < GetSpaceBeforeA()) {
        AllocateB();
        if (GetBFreeSpace() < bytes) {
            return false;
        }

        memcpy(&m_regionBPtr[m_regionBSize], data, bytes);
        m_regionBSize += bytes;
        return true;
    } else {
        if (GetAFreeSpace() < bytes) {
            return false;
        }

        memcpy(&m_regionAPtr[m_regionASize], data, bytes);
        m_regionASize += bytes;
        return true;
    }
}

// Allocate the buffer with room for size bytes
void BipBuffer::Allocate(size_t size) {
    m_buffer = (uint8_t*)malloc(size);
    m_bufferEnd = m_buffer + size;
    m_regionAPtr = m_buffer;  // reset A to the start
}

// Returns a pointer at the "beginning" of the buffer, where data can be pulled from
void* BipBuffer::GetBufferStart() {
    if (m_regionASize > 0) {
        return m_regionAPtr;
    } else {
        return m_regionBPtr;
    }
}

// Return a pointer at the "end" of the buffer, where new data can be written
void* BipBuffer::GetBuffer() {
    if (m_regionBPtr != nullptr) {
        return m_regionBPtr + m_regionBSize;
    } else {
        return m_regionAPtr + m_regionASize;
    }
}

// Increments the "Writen" pointer forward len bytes
void BipBuffer::IncrementWritten(size_t len) {
    if (m_regionBPtr != nullptr) {
        m_regionBSize += len;
    } else {
        m_regionASize += len;
    }
}

// Returns the number of available bytes left
size_t BipBuffer::GetSpace() const {
    if (m_regionBPtr != nullptr) {
        return GetBFreeSpace();
    } else {
        // would allocating buffer B get us more data ?
        if (GetAFreeSpace() < GetSpaceBeforeA()) {
            AllocateB();
            return GetBFreeSpace();
        }
        // or not?
        return GetAFreeSpace();
    }
}

// Returns the number of contiguous bytes(that can be pushed in one operation)
size_t BipBuffer::GetContiguiousBytes() const {
    if (!m_buffer) {
        return 0;
    }

    // A Before B
    return m_regionASize ? m_regionASize : m_regionBSize;
}

// Remove len bytes from the front of the buffer
// @param len the number of bytes to "cut"
void BipBuffer::Remove(size_t len) {
    if (!m_buffer) {
        return;
    }

    // remove from A first before we remove from b
    size_t cnt = len;
    size_t aRem, bRem;

    // If we have both region A and region B, always "finish" off region A first,as
    // this will contain the "oldest" data

    if (m_regionASize > 0) {
        aRem = (cnt > m_regionASize) ? m_regionASize : cnt;
        m_regionASize -= aRem;
        m_regionAPtr += aRem;
        cnt -= aRem;
    }

    // Data left over ? cut the data from buffer B
    if (cnt > 0 && m_regionBSize > 0) {
        bRem = (cnt > m_regionBSize) ? m_regionBSize : cnt;
        m_regionBSize -= bRem;
        m_regionBPtr += bRem;
        cnt -= bRem;
    }

    // is buffer A empty? move buffer B to Buffer A, to increase future performance
    if (m_regionASize == 0) {
        if (m_regionBSize > 0) {
            if (m_regionBPtr != m_buffer) {
                memmove(m_buffer, m_regionBPtr, m_regionBSize);
            }

            m_regionAPtr = m_buffer;
            m_regionASize = m_regionBSize;
            m_regionBPtr = nullptr;
            m_regionBSize = 0;
        } else {
            // no data in region B
            m_regionBPtr = nullptr;
            m_regionBSize = 0;
            m_regionAPtr = m_buffer;
            m_regionBSize = 0;
        }
    }
}
