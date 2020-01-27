#pragma once

#include <cassert>
#include <cstddef>
#include <iostream>
#include <vector>
#include <mutex>

namespace tact {

///////////////////////////////////////////////////////////////////////////////

/// A heap allocated fixed-size block allocator
class HeapPool {
public:
  /// Constructor
  HeapPool(std::size_t blockSize, std::size_t BlockCount);
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
  struct Block {
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
template <std::size_t BlockSize, std::size_t BlockCount> class StackPool {
public:
  /// Constructor
  StackPool() : m_blocksUsed(0) {
    static_assert(BlockSize >= 8, "Block size must be greater or equal to 8");
    reset();
  }

  /// Returns the number of blocks
  constexpr std::size_t blocksTotal() const { return BlockCount; }
  /// Returns the number of occupied blocks
  std::size_t blocksUsed() const { return m_blocksUsed; }
  /// Returns the number of available blocks
  std::size_t blocksAvailable() const { return BlockCount - m_blocksUsed; }
  /// Returns a boolean list indicating occupied blocks
  std::vector<bool> blocksOccupied() const {
    std::vector<bool> ret(BlockCount, true);
    std::size_t first = (std::size_t)m_memory;
    for (Block *i = m_head; i != nullptr; i = i->next) {
      std::size_t idx = ((std::size_t)i - first) / BlockSize;
      ret[idx].flip();
    }
    return ret;
  }

  /// Allocates a block of memory
  void *allocate() {
    // std::lock_guard<std::mutex> lock(m_mutex);
    // std::cout << "allocate()" << std::endl;
    Block *freePosition = pop();
    assert(freePosition != nullptr && "The pool is full");
    m_blocksUsed++;
    return (void *)freePosition;
  }
  /// Frees a block of memory
  void deallocate(void *ptr) {
    // std::lock_guard<std::mutex> lock(m_mutex);
    // std::cout << "deallocate()" << std::endl;
    assert(contains(ptr) && "The pool doesn't manage this address");
    if (!contains(ptr))
      std::cout << "Shitfire!" << std::endl;
    m_blocksUsed--;
    push((Block *)ptr);
  }
  /// Makes available all blocks in the pool
  void reset() {
    // std::lock_guard<std::mutex> lock(m_mutex);
    m_blocksUsed = 0;
    for (std::size_t i = 0; i < BlockCount; ++i) {
      std::size_t address = (std::size_t)m_memory + i * BlockSize;
      push((Block *)address);
    }
  }
  /// Returns true if the address of ptr is inside the memory pool
  bool contains(void *ptr) {
    std::size_t first = (std::size_t)m_memory;
    std::size_t last = first + BlockSize * (BlockCount - 1);
    std::size_t addr = (std::size_t)ptr;
    return addr >= first && addr <= last;
  }

private:
  /// Block linked list node
  struct Block {
    Block *next = nullptr;
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
  char m_memory[BlockSize * BlockCount];
  std::mutex m_mutex;
};

///////////////////////////////////////////////////////////////////////////////

/// A StackPool where memory management is only available to Friend
// template <size_t BlockSize, size_t BlockCount, class Friend>
// class FriendlyStackPool : public StackPool<BlockSize, BlockCount> {
// public:
//   using StackPool::blocksAvailable;
//   using StackPool::blocksTotal;
//   using StackPool::blocksUsed;
//   using StackPool::StackPool;

// protected:
//   friend Friend;
//   using StackPool::allocate;
//   using StackPool::deallocate;
//   using StackPool::reset;
// };

///////////////////////////////////////////////////////////////////////////////

} // namespace tact