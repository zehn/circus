#ifndef _CIRCUS_MPOOL_H_
#define _CIRCUS_MPOOL_H_
#include <cstdint>

namespace Circus {

// reserved
#define MPOOL_FLAG_MASK    0xff

struct MemoryPoolItem {
        unsigned int ref;
        unsigned char data[0]; // it's tricky here 
};

template <typename __DataType>
class MemoryPool {
public:
    enum MemoryPoolState {
        MemoryPoolInit  = 0x00,
        MemoryPoolAlloc = 0x01,
        MemoryPoolReady = 0x02
    }; // reserved

    MemoryPool(uint32_t nsize);
    ~MemoryPool();

    // mpool is full means the queue is empty, as the queue
    // stores the pointers point to the items in pool.
    inline bool Full() const { 
        return head_ == tail_; 
    }
    inline bool Empty() const { 
        return (tail_ - head_) == item_max_; 
    }

    // return the element size in the pool
    inline uint32_t Size() const { 
        return item_max_; 
    }
    // return the element size that in used in the pool
    inline uint32_t Count() const { 
        return (item_max_ + head_ - tail_); 
    } 

    // put the memory back, we MUST take care of the pointer of pd,
    // make sure the pointer is valid and correct
    void Put(__DataType *datap);

    // get a memory block from the memory pool, NULL is returned
    // if the pool is full
    __DataType* Get(void); 

    // get a memory block from the memory pool by the index
    __DataType* GetByIndex(uint32_t index);

    // get the index of the data in the memory pool
    int32_t GetIndex(__DataType* datap);

    // alignment size of the elements
    static unsigned alignment;

protected:
    // memory pool initialization func. be responsible for allocating
    // memory, setting memory queue and etc.
    int init(uint32_t nsize, uint32_t esize);
    void destroy();
    //
    MemoryPoolItem* dequeue();
    void enqueue(MemoryPoolItem *item);

private:
    uint32_t element_size_;     //!< the data original size
    uint32_t item_size_;        //!< the item size that be aligned
    uint32_t item_max_;         //!< the max item number in the memory pool
    volatile uint32_t head_;    //!< index queue head cursor
    volatile uint32_t tail_;    //!< index queue tail cursor
    unsigned char *mem_;
    unsigned char *pool_;
    MemoryPoolItem **queue_;
};

template <typename __DataType> 
unsigned MemoryPool<__DataType>::alignment = 8;

} // namespace Circus

#endif // _CIRCUS_MPOOL_H_
