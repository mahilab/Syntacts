#include <Tact/MemoryPool.hpp>
#include <iostream>
#include <algorithm>

namespace tact {

HeapPool::HeapPool(std::size_t blockSize, std::size_t numBlocks) :
    m_blockSize(blockSize),
    m_numBlocks(numBlocks),
    m_blocksUsed(0)
{
    assert(blockSize >= 8 && "Block size must be greater or equal to 8");
    m_memory = std::malloc(blockSize * numBlocks);
    reset();
    std::cout << "HeapPool Created" << std::endl;
};

HeapPool::~HeapPool()
{
    std::free(m_memory);
    std::cout << "HeapPool Destroyed" << std::endl;
}

void* HeapPool::allocate()
{
    // std::cout << "allocate" << std::endl;
    Block *freePosition = pop();
    assert(freePosition != nullptr && "The pool PoolAllocator is full");
    m_blocksUsed++;
    return (void *)freePosition;
}

void HeapPool::deallocate(void *ptr)
{
    // std::cout << "deallocate" << std::endl;
    m_blocksUsed--;
    push((Block *)ptr);
}

void HeapPool::reset()
{
    m_blocksUsed = 0;
    for (std::size_t i = 0; i < m_numBlocks; ++i)
    {
        std::size_t address = (std::size_t)m_memory + i * m_blockSize;
        push((Block *)address);
    }
}

std::size_t HeapPool::blocksTotal() const {
    return m_numBlocks;
}

std::size_t HeapPool::blocksUsed() const {
    return m_blocksUsed;
}

std::size_t HeapPool::blocksAvail() const {
    return m_numBlocks - m_blocksUsed;
}


void HeapPool::push(Block *newBlock)
{
    newBlock->next = m_head;
    m_head = newBlock;
}

HeapPool::Block* HeapPool::pop()
{
    Block *top = m_head;
    m_head = m_head->next;
    return top;
}    

} // namespace tact