#pragma once

#include <cstddef>
#include <cstdint>

class BipBuffer {
   public:
    BipBuffer();
    ~BipBuffer();

    // Read bytes from the buffer
    bool Read(void* dest, size_t bytes);
    // Write bytes to the buffer
    bool Write(const void* data, size_t bytes);
    // Returns the number of available bytes left
    size_t GetSpace() const;
    // Return the number of bytes currently stored in the buffer
    size_t GetSize() const { return m_buffer == nullptr ? 0 : m_regionASize + m_regionBSize; }
    // Returns the number of continguous bytes(that can be pushed out in one operation)
    size_t GetContiguiousBytes() const;

   private:
    // Removes len bytes from the front of the buffer
    void Remove(size_t len);
    // Returns a pointer an the "end" of the buffer, where new data can be written
    void* GetBuffer();

    // Allocate the buffer with room for size bytes
    void Allocate(size_t size);

    // Increments the "Written" pointer forward len bytes
    void IncrementWritten(size_t len);

    // Returns a pointer at the "beginning" of the buffer, where data can be pulled from
    void* GetBufferStart();

   private:
    inline size_t GetAFreeSpace() const { return (m_bufferEnd - m_regionAPtr - m_regionASize); }
    inline size_t GetSpaceBeforeA() const { return (m_regionAPtr - m_buffer); }
    inline size_t GetSpaceAfterA() const { return (m_bufferEnd - m_regionAPtr - m_regionASize); }
    inline size_t GetBFreeSpace() const {
        return m_regionBPtr == nullptr ? 0 : (m_regionAPtr - m_regionBPtr - m_regionBSize);
    }

    inline void AllocateB() const { m_regionBPtr = m_buffer; }

   private:
    // Allocated whole block pointer
    uint8_t* m_buffer{};
    uint8_t* m_bufferEnd{};
    // region A pointer, and size
    uint8_t* m_regionAPtr{};
    size_t m_regionASize{};

    // region B pointer, and size
    mutable uint8_t* m_regionBPtr{};
    size_t m_regionBSize{};
};
