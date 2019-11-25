#pragma once

#include <cstddef>
#include <cassert>
#include <iostream>

namespace tact
{

///////////////////////////////////////////////////////////////////////////////

/// A heap allocated fixed-size block allocator
class HeapPool
{
public:
    /// Constructor
    HeapPool(std::size_t blockSize, std::size_t numBlocks);
    /// Destructor
    ~HeapPool();

    /// Allocates a block of memory
    void *allocate();
    /// Frees a block of memory
    void deallocate(void *ptr);
    /// Makes available all blocks in the pool
    void reset();

    /// Returns the number of blocks
    std::size_t blocksTotal() const;
    /// Returns the number of occupied blocks
    std::size_t blocksUsed() const;
    /// Returns the number of available blocks
    std::size_t blocksAvail() const;
    
private:
    /// Block linked list node
    struct Block
    {
        Block *next;
    };
    inline void push(Block *newBlock);
    inline Block *pop();
    HeapPool() = default;
    HeapPool(const HeapPool &) = default;
    HeapPool &operator=(const HeapPool &) = default;

private:
    const std::size_t m_blockSize;
    const std::size_t m_numBlocks;
    std::size_t m_blocksUsed;
    Block *m_head;
    void *m_memory;
};

///////////////////////////////////////////////////////////////////////////////

/// A stack allocated fixed-size block allocator
template <size_t BlockSize, size_t NumBlocks>
class StackPool
{
public:
    /// Constructor
    StackPool() : m_blocksUsed(0)
    { 
        static_assert(BlockSize >= 8, "Block size must be greater or equal to 8"); 
        reset(); 
        std::cout << "StackPool Created" << std::endl;
    }
    // Destructor
    ~StackPool() {
        std::cout << "StackPool Destroyed" << std::endl;
    }
    /// Allocates a block of memory
    void *allocate() {
        // std::cout << "alloc" << std::endl;
        Block *freePosition = pop();
        assert(freePosition != nullptr && "The pool PoolAllocator is full");
        m_blocksUsed++;
        return (void *)freePosition;
    }
    /// Frees a block of memory
    void deallocate(void *ptr) {
        // std::cout << "dealloc" << std::endl;
         m_blocksUsed--;
        push((Block *)ptr);
    }
    /// Makes available all blocks in the pool
    void reset() {
        m_blocksUsed = 0;
        for (std::size_t i = 0; i < NumBlocks; ++i)
        {
            std::size_t address = (std::size_t)m_memory + i * BlockSize;
            push((Block *)address);
        }
    }

    /// Returns the number of blocks
    constexpr std::size_t blocksTotal() const {
        return NumBlocks;
    }
    /// Returns the number of occupied blocks
    std::size_t blocksUsed() const {
        return m_blocksUsed;
    }
    /// Returns the number of available blocks
    std::size_t blocksAvail() const {
        return NumBlocks - m_blocksUsed;
    }

private:
    /// Block linked list node
    struct Block
    {
        Block *next;
    };

    inline void push(Block *newBlock) {
        newBlock->next = m_head;
        m_head = newBlock;
    }

    inline Block *pop() {
        Block *top = m_head;
        m_head = m_head->next;
        return top;
    }

    StackPool(const StackPool &) = default;
    StackPool &operator=(const StackPool &) = default;

private:
    std::size_t m_blocksUsed;
    Block *m_head;
    char m_memory[BlockSize * NumBlocks];
};

///////////////////////////////////////////////////////////////////////////////

} // namespace tact