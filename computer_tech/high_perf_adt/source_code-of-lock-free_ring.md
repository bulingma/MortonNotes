
<!-- TOC -->

- [lock-free ring介绍](#lock-free-ring介绍)
  - [0、CAS原语](#0cas原语)
    - [0.1、c++支持的CAS原语](#01c支持的cas原语)
    - [0.2、gcc的封装](#02gcc的封装)
    - [0.3、weak与strong的区别](#03weak与strong的区别)
  - [1、dpdk: lock-free ring](#1dpdk-lock-free-ring)
    - [1.1、dpdk lock-free ring介绍](#11dpdk-lock-free-ring介绍)
    - [1.2、lock-free ring的定义](#12lock-free-ring的定义)
    - [1.3、lock-free ring的API](#13lock-free-ring的api)
    - [1.4、以rte_ring_mc_dequeue_bulk_elem为例分析dpdk的cas实现](#14以rte_ring_mc_dequeue_bulk_elem为例分析dpdk的cas实现)
  - [2、boost: lock-free](#2boost-lock-free)
    - [2.1、boost: lock-free定义](#21boost-lock-free定义)
    - [2.2、boost: lock-free实现](#22boost-lock-free实现)

<!-- /TOC -->

# lock-free ring介绍
cpp标准库和boost、dpdk均实现了lock-free ring。
采取的方法都是CAS(compary and swap, dpdk里面很多函数都说使用compare and set)


## 0、CAS原语

### 0.1、c++支持的CAS原语
```
/*
* @brief：compare & swap(CAS)。如果等于expect则swap，否则就返回--是否交换成功, 注意expect如果不相等，会把当前值写入到expected里面。
* 相比于strong，weak可能会出现[spurious wakeup](<http://en.wikipedia.org/wiki/Spurious_wakeup>).
* @param          若x等于expect,则设置为desired 返回true，
*                 否则最新值写入expect，返回false
*/
class atomic {
bool compare_exchange_strong(T& expect /*用来比较的值*/, T desired/*用来设置的值*/)
bool compare_exchange_weak(T& expect, T desired)
}
```

### 0.2、gcc的封装
gcc 内置Built-in Function:  
```
bool __atomic_compare_exchange_n(
    type *ptr,              // 需要进行比较的ptr
    type *expected,         // 旧的值，会返回ptr里面的值
    type desired,           // 想要设置的新的值
    bool weak,              // 强一致，还是弱一致
    int success_memorder,   // 成功时的内存序
    int failure_memorder    // 失败时的内存序
)
```

### 0.3、weak与strong的区别
weak = true的时候，对应c++11的compare_exchange_weak函数。
weak = false的时候，对应的是compare_exchange_strong函数。
区别这两个函数的区别在于，weak在有的平台上（注意，是有的平台，这里不包括x86）会存在失败的可能性。即，当*ptr == *expected依然有可能什么都不做而返回false。
所以，在x86平台来说，这两者可以说是没什么区别。只是如果想要代码可移值性好，那么采用compare_exchange_weak并且使用循环来判断，那么是一种比较好的办法。

结论就是:  
想要性能，使用compare_exchange_weak + 循环来处理。  
想要简单，使用compare_exchange_strong。   
如果是x86平台，两者没区别  
如果想在移值的时候，拿到高性能，用compare_exchange_weak。  

[Built-in Functions for Memory Model Aware Atomic Operations](https://gcc.gnu.org/onlinedocs/gcc/_005f_005fatomic-Builtins.html)  
[c++并发编程3. CAS原语](https://zhuanlan.zhihu.com/p/56055215)


## 1、dpdk: lock-free ring
版本：spdk21.01  
dpdk对lock-free ring实现的主要文件：rte_ring.h  

### 1.1、dpdk lock-free ring介绍
```
介绍:
/**
 * The Ring Manager is a fixed-size queue, implemented as a table of
 * pointers. Head and tail pointers are modified atomically, allowing
 * concurrent access to it. It has the following features:
 *
 * - FIFO (First In First Out)
 * - Maximum size is fixed; the pointers are stored in a table.
 * - Lockless implementation.
 * - Multi- or single-consumer dequeue.
 * - Multi- or single-producer enqueue.
 * - Bulk dequeue.
 * - Bulk enqueue.
 * - Ability to select different sync modes for producer/consumer.
 * - Dequeue start/finish (depending on consumer sync modes).
 * - Enqueue start/finish (depending on producer sync mode).
 *
 * Note: the ring implementation is not preemptible(抢占式). Refer to Programmer's
 * guide/Environment Abstraction Layer/Multiple pthread/Known Issues/rte_ring
 * for more information.
 */
```

### 1.2、lock-free ring的定义
```
/** prod/cons sync types */
enum rte_ring_sync_type {
	RTE_RING_SYNC_MT,     /**< multi-thread safe (default mode) */
	RTE_RING_SYNC_ST,     /**< single thread only */
#ifdef ALLOW_EXPERIMENTAL_API
	RTE_RING_SYNC_MT_RTS, /**< multi-thread relaxed tail sync */
	RTE_RING_SYNC_MT_HTS, /**< multi-thread head/tail sync */
#endif
};

/**
 * structures to hold a pair of head/tail values and other metadata.
 * Depending on sync_type format of that structure might be different,
 * but offset for *sync_type* and *tail* values should remain the same.
 */
struct rte_ring_headtail {
	volatile uint32_t head;      /**< prod/consumer head. */
	volatile uint32_t tail;      /**< prod/consumer tail. */
	RTE_STD_C11
	union {
		/** sync type of prod/cons */
		enum rte_ring_sync_type sync_type;
		/** deprecated -  True if single prod/cons */
		uint32_t single;
	};
};

union __rte_ring_rts_poscnt {
	/** raw 8B value to read/write *cnt* and *pos* as one atomic op */
	uint64_t raw __rte_aligned(8);
	struct {
		uint32_t cnt; /**< head/tail reference counter */
		uint32_t pos; /**< head/tail position */
	} val;
};

struct rte_ring_rts_headtail {
	volatile union __rte_ring_rts_poscnt tail;
	enum rte_ring_sync_type sync_type;  /**< sync type of prod/cons */
	uint32_t htd_max;   /**< max allowed distance between head/tail */
	volatile union __rte_ring_rts_poscnt head;
};

union __rte_ring_hts_pos {
	/** raw 8B value to read/write *head* and *tail* as one atomic op */
	uint64_t raw __rte_aligned(8);
	struct {
		uint32_t head; /**< head position */
		uint32_t tail; /**< tail position */
	} pos;
};

struct rte_ring_hts_headtail {
	volatile union __rte_ring_hts_pos ht;
	enum rte_ring_sync_type sync_type;  /**< sync type of prod/cons */
};

/**
 * An RTE ring structure.
 *
 * The producer and the consumer have a head and a tail index. The particularity
 * of these index is that they are not between 0 and size(ring). These indexes
 * are between 0 and 2^32, and we mask their value when we access the ring[]
 * field. Thanks to this assumption, we can do subtractions between 2 index
 * values in a modulo-32bit base: that's why the overflow of the indexes is not
 * a problem.
 */
struct rte_ring {
	/*
	 * Note: this field kept the RTE_MEMZONE_NAMESIZE size due to ABI
	 * compatibility requirements, it could be changed to RTE_RING_NAMESIZE
	 * next time the ABI changes
	 */
	char name[RTE_MEMZONE_NAMESIZE] __rte_cache_aligned; /**< Name of the ring. */
	int flags;                                           /**< Flags supplied at creation. */
	const struct rte_memzone *memzone;             /**< Memzone, if any, containing the rte_ring */
	uint32_t size;           /**< Size of ring. */
	uint32_t mask;           /**< Mask (size-1) of ring. */
	uint32_t capacity;       /**< Usable size of ring */

	/** Ring producer status. */
	RTE_STD_C11
	union {
		struct rte_ring_headtail prod;
		struct rte_ring_hts_headtail hts_prod;
		struct rte_ring_rts_headtail rts_prod;
	}  __rte_cache_aligned;

	/** Ring consumer status. */
	RTE_STD_C11
	union {
		struct rte_ring_headtail cons;
		struct rte_ring_hts_headtail hts_cons;
		struct rte_ring_rts_headtail rts_cons;
	}  __rte_cache_aligned;
};
```
### 1.3、lock-free ring的API
总结：  
1、func_name带有mp（multi process）表示多 multi-producers safe，带有sp（single process）为NOT multi-producers safe；  
2、func_name带有bulk表示多 enqueue/dequeue 多个；  
3、func_name带有mc表示multi consumer;  
4、func_name带有dequeue_burst表示优先按照用户要求个数和实际存在元素个数dequeue;   
| api func name               | Description                                                     | other                                                                                                                                                            |
| --------------------------- | --------------------------------------------------------------- | ---------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| rte_ring_get_memsize        | Calculate the memory size needed for a ring                     | /                                                                                                                                                                |
| rte_ring_init               | Initialize a ring structure.                                    | /                                                                                                                                                                |
| rte_ring_create             | Create a new ring named *name* in memory                        | /                                                                                                                                                                |
| rte_ring_free               | De-allocate all memory used by the ring.                        | /                                                                                                                                                                |
| rte_ring_dump               | Dump the status of the ring to a file.                          | /                                                                                                                                                                |
| rte_ring_create             | Create a new ring named *name* in memory                        | /                                                                                                                                                                |
| rte_ring_free               | De-allocate all memory used by the ring.                        | /                                                                                                                                                                |
| rte_ring_dump               | Dump the status of the ring to a file.                          | /                                                                                                                                                                |
| rte_ring_mp_enqueue_bulk    | Enqueue several objects on the ring (multi-producers safe).     | uses a "compare and set" instruction to move the producer index atomically.                                                                                      |
| rte_ring_sp_enqueue_bulk    | Enqueue several objects on a ring (NOT multi-producers safe).   |                                                                                                                                                                  |
| rte_ring_enqueue            | Enqueue one object on a ring.                                   | This function calls the multi-producer or the single-producer version, depending on the default behaviour that was specified at ring creation time (see flags).  |
| rte_ring_mc_dequeue_bulk    | Dequeue several objects from a ring (multi-consumers safe).     | This function uses a **<u>"compare and set"</u>** instruction to move the consumer index atomically.                                                             |
| rte_ring_sc_dequeue_bulk    | Dequeue several objects from a ring (NOT multi-consumers safe). |                                                                                                                                                                  |
| rte_ring_dequeue_bulk       | Dequeue several objects from a ring.                            | This function calls the multi-consumers or the single-consumer version, depending on the default behaviour that was specified at ring creation time (see flags). |
| rte_ring_mc_dequeue         | Dequeue one object from a ring (multi-consumers safe).          |                                                                                                                                                                  |
| rte_ring_sc_dequeue         | Dequeue one object from a ring (NOT multi-consumers safe).      |                                                                                                                                                                  |
| rte_ring_dequeue            | Dequeue one object from a ring.                                 | /                                                                                                                                                                |
| rte_ring_reset              | Flush a ring.                                                   | /                                                                                                                                                                |
| rte_ring_count              | Return the number of entries in a ring.                         | /                                                                                                                                                                |
| rte_ring_free_count         | Return the number of free entries in a ring.                    | /                                                                                                                                                                |
| rte_ring_full               | Test if a ring is full.                                         | /                                                                                                                                                                |
| rte_ring_empty              | Test if a ring is empty.                                        | /                                                                                                                                                                |
| rte_ring_get_size           | Return the size of the ring.                                    | /                                                                                                                                                                |
| rte_ring_get_capacity       | Return the number of elements which can be stored in the ring.  | /                                                                                                                                                                |
| rte_ring_get_prod_sync_type | Return sync type used by producer in the ring.                  | /                                                                                                                                                                |
| rte_ring_is_prod_single     | Check is the ring for single producer.                          | /                                                                                                                                                                |
| rte_ring_get_cons_sync_type | Return sync type used by consumer in the ring.                  | /                                                                                                                                                                |
| rte_ring_is_cons_single     | Check is the ring for single consumer.                          | /                                                                                                                                                                |
| rte_ring_list_dump          | Dump the status of all rings on the console                     | /                                                                                                                                                                |
| rte_ring_lookup             | Search a ring from its name                                     | /                                                                                                                                                                |
| rte_ring_mp_enqueue_burst   | Enqueue several objects on the ring (multi-producers safe).     | /                                                                                                                                                                |
| rte_ring_sp_enqueue_burst   | Enqueue several objects on a ring (NOT multi-producers safe).   | /                                                                                                                                                                |
| rte_ring_enqueue_burst      | Enqueue several objects on a ring.                              | This function calls the multi-producer or the single-producer version depending on the default behavior that was specified at ring creation time (see flags).    |
| rte_ring_mc_dequeue_burst   | Dequeue several objects from a ring (multi-consumers safe).     | When the request  objects are more than the available objects, only dequeue the actual number of objects                                                         |
| rte_ring_sc_dequeue_burst   | Dequeue several objects from a ring (NOT multi-consumers safe). | 同上                                                                                                                                                             |
| rte_ring_dequeue_burst      | Dequeue multiple objects from a ring up to a maximum number.    | This function calls the multi-consumers or the single-consumer version, depending on the default behaviour that was specified at ring creation time (see flags). |


### 1.4、以rte_ring_mc_dequeue_bulk_elem为例分析dpdk的cas实现
```
static __rte_always_inline unsigned int
rte_ring_mc_dequeue_bulk_elem(struct rte_ring *r, void *obj_table,
		unsigned int esize, unsigned int n, unsigned int *available)
{
	return __rte_ring_do_dequeue_elem(r, obj_table, esize, n,
			RTE_RING_QUEUE_FIXED, RTE_RING_SYNC_MT, available){
        //见下面的源码分析
        n = __rte_ring_move_cons_head(r, (int)is_sc, n, behavior,
                &cons_head, &cons_next, &entries);
        ...
    }
}
```
```
/**
 * @internal This function updates the consumer head for dequeue
 *
 * @param r
 *   A pointer to the ring structure
 * @param is_sc
 *   Indicates whether multi-consumer path is needed or not
 * @param n
 *   The number of elements we will want to enqueue, i.e. how far should the
 *   head be moved
 * @param behavior
 *   RTE_RING_QUEUE_FIXED:    Dequeue a fixed number of items from a ring
 *   RTE_RING_QUEUE_VARIABLE: Dequeue as many items as possible from ring
 * @param old_head
 *   Returns head value as it was before the move, i.e. where dequeue starts
 * @param new_head
 *   Returns the current/new head value i.e. where dequeue finishes
 * @param entries
 *   Returns the number of entries in the ring BEFORE head was moved
 * @return
 *   - Actual number of objects dequeued.
 *     If behavior == RTE_RING_QUEUE_FIXED, this will be 0 or n only.
 */
static __rte_always_inline unsigned int
__rte_ring_move_cons_head(struct rte_ring *r, int is_sc,
		unsigned int n, enum rte_ring_queue_behavior behavior,
		uint32_t *old_head, uint32_t *new_head,
		uint32_t *entries)
{
	/* move cons.head atomically */
	*old_head = __atomic_load_n(&r->cons.head, __ATOMIC_RELAXED);
	do {
		n = max;   /* Restore n as it may change every loop */

		__atomic_thread_fence(__ATOMIC_ACQUIRE);   /* Ensure the head is read before tail */

		/* this load-acquire synchronize with store-release of ht->tail
		 * in update_tail.*/
		prod_tail = __atomic_load_n(&r->prod.tail, __ATOMIC_ACQUIRE);

		/* The subtraction is done between two unsigned 32bits value
		 * (the result is always modulo 32 bits even if we have
		 * cons_head > prod_tail). So 'entries' is always between 0
		 * and size(ring)-1.
		 */
		*entries = (prod_tail - *old_head);

		/* Set the actual entries for dequeue */
		if (n > *entries) n = (behavior == RTE_RING_QUEUE_FIXED) ? 0 : *entries;

		if (unlikely(n == 0)) return 0;

		*new_head = *old_head + n;
		if (is_sc)
			r->cons.head = *new_head, success = 1;
		else
			/* on failure, *old_head will be updated */
			/* 采用gcc封装的原子操作__atomic_compare_exchange_n，使用compare_exchange_stronge + 循环来处理 */
			success = __atomic_compare_exchange_n(&r->cons.head,
							old_head, *new_head,
							0, __ATOMIC_RELAXED,
							__ATOMIC_RELAXED);
	} while (unlikely(success == 0));
	return n;
}
```

## 2、boost: lock-free  
版本：boost1.53.0  
文件：/boost_1_53_0/boost/lockfree/queue.hpp

### 2.1、boost: lock-free定义

```
/** The queue class provides a multi-writer/multi-reader queue, pushing and popping is lock-free,
 *  construction/destruction has to be synchronized. It uses a freelist for memory management,
 *  freed nodes are pushed to the freelist and not returned to the OS before the queue is destroyed.
 *
 *  \b Policies:
 *  - \ref boost::lockfree::fixed_sized, defaults to \c boost::lockfree::fixed_sized<false> \n
 *    Can be used to completely disable dynamic memory allocations during push in order to ensure lockfree behavior. \n
 *    If the data structure is configured as fixed-sized, the internal nodes are stored inside an array and they are addressed
 *    by array indexing. This limits the possible size of the queue to the number of elements that can be addressed by the index
 *    type (usually 2**16-2), but on platforms that lack double-width compare-and-exchange instructions, this is the best way
 *    to achieve lock-freedom.
 *
 *  - \ref boost::lockfree::capacity, optional \n
 *    If this template argument is passed to the options, the size of the queue is set at compile-time.\n
 *    It this option implies \c fixed_sized<true>
 *
 *  - \ref boost::lockfree::allocator, defaults to \c boost::lockfree::allocator<std::allocator<void>> \n
 *    Specifies the allocator that is used for the internal freelist
 *
 *  \b Requirements:
 *   - T must have a copy constructor
 *   - T must have a trivial assignment operator
 *   - T must have a trivial destructor
 *
 * */
#ifndef BOOST_DOXYGEN_INVOKED
template <typename T,
          class A0 = boost::parameter::void_,
          class A1 = boost::parameter::void_,
          class A2 = boost::parameter::void_>
#else
template <typename T, ...Options>
#endif
class queue:
    boost::noncopyable
{
private:
#ifndef BOOST_DOXYGEN_INVOKED

#ifdef BOOST_HAS_TRIVIAL_DESTRUCTOR
    BOOST_STATIC_ASSERT((boost::has_trivial_destructor<T>::value));
#endif

#ifdef BOOST_HAS_TRIVIAL_ASSIGN
    BOOST_STATIC_ASSERT((boost::has_trivial_assign<T>::value));
#endif

    typedef typename detail::queue_signature::bind<A0, A1, A2>::type bound_args;
    static const bool has_capacity = detail::extract_capacity<bound_args>::has_capacity;
    static const size_t capacity = detail::extract_capacity<bound_args>::capacity;
    static const bool fixed_sized = detail::extract_fixed_sized<bound_args>::value;
    static const bool node_based = !(has_capacity || fixed_sized);
    static const bool compile_time_sized = has_capacity;

    struct BOOST_LOCKFREE_CACHELINE_ALIGNMENT node
    {
        typedef typename detail::select_tagged_handle<node, node_based>::tagged_handle_type tagged_node_handle;
        typedef typename detail::select_tagged_handle<node, node_based>::handle_type handle_type;

        node(T const & v, handle_type null_handle):
            data(v)//, next(tagged_node_handle(0, 0))
        {
            /* increment tag to avoid ABA problem */
            tagged_node_handle old_next = next.load(memory_order_relaxed);
            tagged_node_handle new_next (null_handle, old_next.get_tag()+1);
            next.store(new_next, memory_order_release);
        }

        node (handle_type null_handle):
            next(tagged_node_handle(null_handle, 0))
        {}

        node(void)
        {}

        atomic<tagged_node_handle> next;
        T data;
    };

    typedef typename detail::extract_allocator<bound_args, node>::type node_allocator;
    typedef typename detail::select_freelist<node, node_allocator, compile_time_sized, fixed_sized, capacity>::type pool_t;
    typedef typename pool_t::tagged_node_handle tagged_node_handle;
    typedef typename detail::select_tagged_handle<node, node_based>::handle_type handle_type;

    void initialize(void)

    struct implementation_defined
    {
        typedef node_allocator allocator;
        typedef std::size_t size_type;
    };

#endif

public:
    typedef T value_type;
    typedef typename implementation_defined::allocator allocator;
    typedef typename implementation_defined::size_type size_type;

    /**
     * \return true, if implementation is lock-free.* */
    bool is_lock_free (void) const

    //! Construct queue
    // @{
    queue(void):
        head_(tagged_node_handle(0, 0)),
        tail_(tagged_node_handle(0, 0)),
        pool(node_allocator(), capacity)
    {
        BOOST_ASSERT(has_capacity);
        initialize();
    }

    template <typename U>
    explicit queue(typename node_allocator::template rebind<U>::other const & alloc):
        head_(tagged_node_handle(0, 0)),
        tail_(tagged_node_handle(0, 0)),
        pool(alloc, capacity)
    {
        BOOST_STATIC_ASSERT(has_capacity);
        initialize();
    }

    explicit queue(allocator const & alloc):
        head_(tagged_node_handle(0, 0)),
        tail_(tagged_node_handle(0, 0)),
        pool(alloc, capacity)
    {
        BOOST_ASSERT(has_capacity);
        initialize();
    }
    // @}

    //! Construct queue, allocate n nodes for the freelist.
    // @{
    explicit queue(size_type n):
        head_(tagged_node_handle(0, 0)),
        tail_(tagged_node_handle(0, 0)),
        pool(node_allocator(), n + 1)
    {
        BOOST_ASSERT(!has_capacity);
        initialize();
    }

    template <typename U>
    queue(size_type n, typename node_allocator::template rebind<U>::other const & alloc):
        head_(tagged_node_handle(0, 0)),
        tail_(tagged_node_handle(0, 0)),
        pool(alloc, n + 1)
    {
        BOOST_STATIC_ASSERT(!has_capacity);
        initialize();
    }

    /** \copydoc boost::lockfree::stack::reserve* */
    void reserve(size_type n)
    /** \copydoc boost::lockfree::stack::reserve_unsafe* */
    void reserve_unsafe(size_type n)
    /** Destroys queue, free all nodes from freelist.* */
    ~queue(void)
    /** Check if the queue is empty* */
    bool empty(void)
    /** Pushes object t to the queue.* */
    bool push(T const & t)
    /** Pushes object t to the queue.* */
    bool bounded_push(T const & t)


private:
#ifndef BOOST_DOXYGEN_INVOKED
    template <bool Bounded> bool do_push(T const & t)
#endif

public:
    /** Pushes object t to the queue.* */
    bool unsynchronized_push(T const & t)
    /** Pops object from queue.* */
    bool pop (T & ret)
    /** Pops object from queue.* */
    template <typename U> bool pop (U & ret)
    /** Pops object from queue **/
    bool unsynchronized_pop (T & ret)
    /** Pops object from queue. **/
    template <typename U> bool unsynchronized_pop (U & ret)

private:
#ifndef BOOST_DOXYGEN_INVOKED
    atomic<tagged_node_handle> head_;
    atomic<tagged_node_handle> tail_;
    pool_t pool;
#endif
};
```

### 2.2、boost: lock-free实现

```
#ifndef BOOST_DOXYGEN_INVOKED
    template <bool Bounded>
    bool do_push(T const & t)
    {
        node * n = pool.template construct<true, Bounded>(t, pool.null_handle());
        handle_type node_handle = pool.get_handle(n);
        if (n == NULL)
            return false;

        for (;;) { // 死循环，直到push成功
            tagged_node_handle tail = tail_.load(memory_order_acquire);
            node * tail_node = pool.get_pointer(tail);
            tagged_node_handle next = tail_node->next.load(memory_order_acquire);
            node * next_ptr = pool.get_pointer(next);
            tagged_node_handle tail2 = tail_.load(memory_order_acquire);
            if (likely(tail == tail2)) { //确定tail在当前时间点上没有修改过，修改过就继续执行
                if (next_ptr == 0) {     //非满
                    tagged_node_handle new_tail_next(node_handle, next.get_tag() + 1);
                     //使用compare_exchange_weak原子操作修改队列tail_node->next
                    if ( tail_node->next.compare_exchange_weak(next, new_tail_next) ) { 
                        tagged_node_handle new_tail(node_handle, tail.get_tag() + 1);
                        //使用compare_exchange_strong原子操作修改队列tail
                        tail_.compare_exchange_strong(tail, new_tail);
                        return true;
                    }
                }
                else { // 队列已满
                //如下操作不知道为什么
                    tagged_node_handle new_tail(pool.get_handle(next_ptr), tail.get_tag() + 1);
                    tail_.compare_exchange_strong(tail, new_tail);
                }
            }
        }
    }
#endif
```

