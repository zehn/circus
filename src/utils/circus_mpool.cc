#include "circus_mpool.h"
#include <cstring>

#ifndef offsetof
#ifdef __compiler_offsetof
#define offsetof(TYPE,MEMBER) __compiler_offsetof(TYPE,MEMBER)
#else
#define offsetof(TYPE, MEMBER) ((unsigned long) &((TYPE *)0)->MEMBER)
#endif // __compiler_offsetof 
#endif // offsetof

#ifdef container_of
#undef container_of
#endif
// This macro is not recommanded and used only in this file
#define container_of(ptr, type, member) ({ \
    (type *)( (char *)ptr - offsetof(type,member) );})

template <typename __DataType>
Circus_MemoryPool<__DataType>::Circus_MemoryPool(uint32_t nsize) {
    init(nsize, sizeof(__DataType));
}

template <typename __DataType>
Circus_MemoryPool<__DataType>::~Circus_MemoryPool()  {
    destroy();
}

template <typename __DataType>
void Circus_MemoryPool<__DataType>::Put(__DataType* datap) {
    if (datap)  {      
        MemoryPoolItem *it = container_of(datap, MemoryPoolItem, data);
        unsigned int r = it->ref;
        if (0 == --r) 
            enqueue(it);
    }
}

template <typename __DataType>
__DataType* Circus_MemoryPool<__DataType>::Get() {
    __DataType *datap = NULL;
    MemoryPoolItem *it = dequeue();
    if (it) {
        if (0 == it->ref) {
            it->ref = 1;
            memset(it->data, 0, element_size_);
            datap = reinterpret_cast<__DataType *>(it->data);
        }
    }
    return datap;
}

template <typename __DataType>
__DataType* Circus_MemoryPool<__DataType>::GetByIndex(uint32_t index) {
    if (item_max_ < index) return NULL;
    MemoryPoolItem *datap = reinterpret_cast<MemoryPoolItem *>(pool_ + index * item_size_);
    return (__DataType *)(datap->data);
}

template <typename __DataType>
int32_t Circus_MemoryPool<__DataType>::GetIndex(__DataType* datap) {
    if (datap && !Empty()) {
        MemoryPoolItem *it = container_of(datap, MemoryPoolItem, data);
        if (0 != it->ref) {
            uint32_t l = (uint32_t)((unsigned char *)it - pool_);
            if (l % item_size_ != 0)
                return -1;
            uint32_t index = l / item_size_; 
            if (item_max_ > l)
                return (int32_t)index;  // not good
        }
    }
    return -1;
}

template <typename __DataType>
int Circus_MemoryPool<__DataType>::init(uint32_t nsize, uint32_t esize) {
    // element aligned size in memory
    uint32_t asize = (esize + sizeof (MemoryPoolItem) + alignment - 1) 
                        / alignment * alignment;

    // |<--queue-->|<-----------------pool------------------>|
    // +--+-----+--+--------+-----------------------+--------+
    // |q1| ... |qn| elem 1 |        ......         | elem n |
    // +--+-----+--+--------+-----------------------+--------+
    //   |        |    |                                 |
    //   +--------+----+                                 |              
    //            |                                      |
    //            +--------------------------------------+ 
    // total size of the pool in memory
    uint32_t total_size = nsize * (sizeof(void *) + asize);

    element_size_ = esize;
    item_size_ = asize;
    item_max_ = total_size / (sizeof(void *) + item_size_);
    tail_ = item_max_;
    head_ = 0; 
    //
    mem_ = new unsigned char [total_size];
    if (NULL == mem_) return 0;
    queue_ = reinterpret_cast<MemoryPoolItem**>(mem_);
    pool_  = mem_ + nsize * sizeof(void *);

    for (uint32_t i = 0; i < item_max_; ++i)
        queue_[i] = reinterpret_cast<MemoryPoolItem *>(pool_ + i * item_size_);

    return total_size;
}

template <typename __DataType>
void Circus_MemoryPool<__DataType>::destroy() {
    if (mem_) delete mem_;
}

template <typename __DataType>
MemoryPoolItem* Circus_MemoryPool<__DataType>::dequeue() {
    if (Full()) return NULL;
    return queue_[head_++ % item_max_];
}

template <typename __DataType>
void Circus_MemoryPool<__DataType>::enqueue(MemoryPoolItem *item) {
    if (item)
        queue_[tail_++ % item_max_] = item;
}

#undef container_of

