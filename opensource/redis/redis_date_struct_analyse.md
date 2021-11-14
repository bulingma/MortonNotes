
<!-- TOC -->

- [redis encoding type汇总](#redis-encoding-type汇总)
- [linkedlist——双端链表](#linkedlist双端链表)
  - [双端链表的应用](#双端链表的应用)
    - [实现Redis的列表类型](#实现redis的列表类型)
    - [Redis自身功能的构建](#redis自身功能的构建)
  - [链表和链表节点定义](#链表和链表节点定义)
  - [链表实现特点](#链表实现特点)
  - [连表API及时间复杂度](#连表api及时间复杂度)
- [hashtables（hash/set内部使用）](#hashtableshashset内部使用)
  - [字典的应用](#字典的应用)
    - [实现数据库键空间（key space）](#实现数据库键空间key-space)
    - [用作Hash类型键的其中一种底层实现](#用作hash类型键的其中一种底层实现)
  - [字典的定义](#字典的定义)
  - [字典的方法](#字典的方法)
  - [字典的剖析](#字典的剖析)
- [quicklist（list使用）](#quicklistlist使用)
- [ziplist(zset/hash/list内部元素个数小于512、128个&每个元素值小于64字节时使用)](#ziplistzsethashlist内部元素个数小于512128个每个元素值小于64字节时使用)
- [skiplist（zset使用）](#skiplistzset使用)

<!-- /TOC -->
## redis encoding type汇总

![图2-2 Redis数据结构和内部编码](../../z_images/redis/redis_five_data_struct_and_econding.png)


| type   | value inner type | describtion                                                                                                                        |
| ------ | ---------------- | ---------------------------------------------------------------------------------------------------------------------------------- |
| string | raw              | raw:大于39个字节的字符串                                                                                                           |
| string | int              | int:8个字节的长整型                                                                                                                |
| string | embstr           | raw:小于39个字节的字符串                                                                                                           |
| hash   | hashtables       | 当哈希类型无法满足ziplist的条件时，Redis会使用hashtable作为哈希的内部实现                                                          |
| hash   | ziplist          | 哈希类型元素个数小于hash-max-ziplist-entries配置(默认512个)、同时所有值都小于hash-max-ziplist-value配置(默认64字节)时              |
| list   | linkedlist       | 当列表类型无法满足ziplist的条件时                                                                                                  |
| list   | ziplist          | 当列表的元素个数小于list-max-ziplist-entries配置 (默认512个)，同时列表中每个元素的值都小于list-max-ziplist-value配置时(默认64字节) |
| list   | quicklist        | 简单地说它是以一个ziplist为节点的linkedlist，它结合了ziplist和linkedlist两者的优势                                                 |
| set    | hashtables       | 当集合类型无法满足intset的条件时                                                                                                   |
| set    | intset           | 当集合中的元素都是整数且元素个数小于set-max-intset-entries配置(默认512个)时                                                        |
| zset   | skiplist         | 当ziplist条件不满足时                                                                                                              |
| zset   | ziplist          | 有序集合的元素个数小于zset-max-ziplist-entries配置(默认128个)，同时每个元素的值都小于zset-max-ziplist-value配置(默认64字节)时      |


## linkedlist——双端链表
### 双端链表的应用
双端链表作为一种通用的数据结构，在Redis内部使用得非常多：它既是Redis列表结构的底层实现之一，还被大量Redis模块所使用，用于构建Redis的其他功能。主要有如下几种功能：  
#### 实现Redis的列表类型      
  双端链表是Redis列表类型的底层实现之一，当对列表类型的键进行操作——比如执行RPUSH、LPOP或LLEN等命令时，程序在底层操作的可能就是双端链表。
#### Redis自身功能的构建  
  除了实现列表类型以外，双端链表还被很多Redis内部模块所应用：  
  • 事务模块使用双端链表来按顺序保存输入的命令；  
  • 服务器模块使用双端链表来保存多个客户端；  
  • 订阅/发送模块使用双端链表来保存订阅模式的多个客户端；  
  • 事件模块使用双端链表来保存时间事件（time event）；  

### 链表和链表节点定义
```
// adlist.h
typedef struct listNode {
    struct listNode *prev;
    struct listNode *next;
    void *value;
} listNode;

typedef struct list {
    listNode *head;
    listNode *tail;
    void *(*dup)(void *ptr);
    void (*free)(void *ptr);
    int (*match)(void *ptr, void *key);
    unsigned long len;
} list;

typedef struct listIter {
    listNode *next;
    int direction;

} listIter;
```

### 链表实现特点
1、双端链表，含表头、尾节点;  
2、多态:   
a、listNode的value属性的类型是void *，说明这个双端链表对节点所保存的值的类型不做限制。  
b、对于不同类型的值，有时候需要不同的函数来处理这些值，因此，list类型保留了三个函数指针——dup、free和match，分别用于处理值的复制、释放和对比匹配。在对节点的值进行处理时，如果有给定这些函数，那么它们就会被调用。  
举个例子：当删除一个listNode时，如果包含这个节点的list的list->free函数不为空，那么删除函数就会先调用list->free(listNode->value)清空节点的值，再执行余下的删除操作（比如说，释放节点）。  

另外，从这两个数据结构的定义上，也可以看出它们的一些行为和性能特征：
* listNode带有prev和next两个指针，因此，对链表的遍历可以在两个方向上进行：从表头到表尾，或者从表尾到表头。
* list保存了head和tail两个指针，因此，对链表的表头和表尾进行插入的复杂度都为O(1)——这是高效实现LPUSH、RPOP、RPOPLPUSH等命令的关键。
* list带有保存节点数量的len属性，所以计算链表长度的复杂度仅为O(1)，这也保证了LLEN命令不会成为性能瓶颈。

### 连表API及时间复杂度
* api  
```
/* Prototypes */
list *listCreate(void);
void listRelease(list *list);
void listEmpty(list *list);
list *listAddNodeHead(list *list, void *value);
list *listAddNodeTail(list *list, void *value);
list *listInsertNode(list *list, listNode *old_node, void *value, int after);
void listDelNode(list *list, listNode *node);
listIter *listGetIterator(list *list, int direction);
listNode *listNext(listIter *iter);
void listReleaseIterator(listIter *iter);
list *listDup(list *orig);
listNode *listSearchKey(list *list, void *key);
listNode *listIndex(list *list, long index);
void listRewind(list *list, listIter *li);
void listRewindTail(list *list, listIter *li);
void listRotateTailToHead(list *list);
void listRotateHeadToTail(list *list);
void listJoin(list *l, list *o);
```

* 复杂度  
![双端链表的API及复杂度](../../z_images/redis/linkedlist_api_and_big_o.png)

## hashtables（hash/set内部使用）
### 字典的应用
字典在Redis中的应用广泛，使用频率可以说和SDS以及双端链表不相上下，基本上各个功能模块都有用到字典的地方。其中，字典的主要用途有以下两个：  
* 1.实现数据库键空间（key space）；
* 2.用作Hash类型键的其中一种底层实现；  

#### 实现数据库键空间（key space）
Redis是一个键值对数据库，数据库中的键值对就由字典保存：每个数据库都有一个与之相对应的字典，这个字典被称之为键空间（key space）。  
当用户添加一个键值对到数据库时（不论键值对是什么类型），程序就将该键值对添加到键空间；当用户从数据库中删除一个键值对时，程序就会将这个键值对从键空间中删除；等等。  
大部分针对数据库的命令，比如DBSIZE、FLUSHDB、:ref:RANDOMKEY，等等，都是构建于对字典的操作之上的；而那些创建、更新、删除和查找键值对的命令，也无一例外地需要在键空间上进行操作。
#### 用作Hash类型键的其中一种底层实现
Redis的Hash类型键使用以下两种数据结构作为底层实现:  
1.字典；  
2.压缩列表；  
因为压缩列表比字典更节省内存，所以程序在创建新Hash键时，默认使用压缩列表作为底层实现，当有需要时，程序才会将底层实现从压缩列表转换到字典。

### 字典的定义  
实现字典的方法有很多种：  
•最简单的就是使用链表或数组，但是这种方式只适用于元素个数不多的情况下；  
•要兼顾高效和简单性，可以使用哈希表；  
•如果追求更为稳定的性能特征，并且希望高效地实现排序操作的话，则可以使用更为复杂的平衡树；  
在众多可能的实现中，Redis选择了高效且实现简单的哈希表作为字典的底层实现。

```
/*
 * 哈希表节点
 */
typedef struct dictEntry {
    // 键
    void *key;
    // 值
    union {
        void *val;
        uint64_t u64;
        int64_t s64;
    } v;
    // 指向下个哈希表节点，形成链表
    // 使用链地址法（拉链发）来处理键碰撞
    struct dictEntry *next;
} dictEntry;

/*
 * 字典类型特定函数
 */
typedef struct dictType {
    // 计算哈希值的函数
    unsigned int (*hashFunction)(const void *key);
    // 复制键的函数
    void *(*keyDup)(void *privdata, const void *key);
    // 复制值的函数
    void *(*valDup)(void *privdata, const void *obj);
    // 对比键的函数
    int (*keyCompare)(void *privdata, const void *key1, const void *key2);
    // 销毁键的函数
    void (*keyDestructor)(void *privdata, void *key);
    // 销毁值的函数
    void (*valDestructor)(void *privdata, void *obj);
} dictType;

/*
 * 哈希表
 * 每个字典都使用两个哈希表，从而实现渐进式 rehash 。
 */
typedef struct dictht {
    // 哈希表数组
    dictEntry **table;
    // 哈希表大小
    unsigned long size;
    // 哈希表大小掩码，用于计算索引值
    // 总是等于 size - 1
    unsigned long sizemask;
    // 该哈希表已有节点的数量
    unsigned long used;
} dictht;

typedef struct dict {
    // 类型特定函数
    dictType *type;
    // 私有数据
    void *privdata;
    // 哈希表
    dictht ht[2];
    // rehash 索引
    // 当 rehash 不在进行时，值为 -1
    int rehashidx; /* rehashing not in progress if rehashidx == -1 */
    // 目前正在运行的安全迭代器的数量
    int iterators; /* number of iterators currently running */
} dict;

```
### 字典的方法
```

```
![dict API和算法复杂度](../../z_images/redis/hash_api_and_big_o.png)

### 字典的剖析  
注意dict类型使用了两个指针分别指向两个哈希表。    
其中，0号哈希表（ht[0]）是字典主要使用的哈希表，  
而1号哈希表（ht[1]）则只有在程序对0号哈希表进行rehash时才使用。
![字典结构](../../z_images/redis/hash_dict_structrue_arch.png)
在上图的字典示例中，字典虽然创建了两个哈希表，但正在使用的只有0号哈希表，这说明字典未进行rehash状态。

* hash函数  
  Redis目前使用两种不同的哈希算法：  
  1.MurmurHash2 32 bit算法：  
  这种算法的分布率和速度都非常好，具体信息请参考Mur-murHash的主页：http://code.google.com/p/smhasher/。  
  2.基于djb算 法 实 现 的 一 个 大 小 写 无 关 散 列 算 法： 具 体 信 息 请 参 考http://www.cse.yorku.ca/~oz/hash.html。  
  使用哪种算法取决于具体应用所处理的数据：  
  •命令表以及Lua脚本缓存都用到了算法2。  
  •算法1的应用则更加广泛：数据库、集群、哈希键、阻塞操作等功能都用到了这个算法

* 冲突解决  
  使用链地址法（拉链发）来处理键碰撞

* rehash
对于使用链地址法来解决碰撞问题的哈希表dictht来说，哈希表的性能依赖于它的大小（size属性）和它所保存的节点的数量（used属性）之间的比率：  
•比率在1:1时，哈希表的性能最好；  
•如果节点数量比哈希表的大小要大很多的话，那么哈希表就会退化成多个链表，哈希表本身的性能优势就不再存在；    

为了在字典的键值对不断增多的情况下保持良好的性能，字典需要对所使用的哈希表（ht[0]）进行rehash操作：  
在不修改任何键值对的情况下，对哈希表进行扩容，尽量将比率维持在1:1左右。dictAdd在每次向字典添加新键值对之前，都会对哈希表ht[0]进行检查，对于ht[0]的size和used属性，如果它们之间的比率ratio = used / size满足以下任何一个条件的话，rehash过程就会被激活    
 1.自然rehash：ratio >= 1，且变量dict_can_resize为真。  
 2.强制rehash：ratio大 于 变 量dict_force_resize_ratio（目 前 版 本 中，dict_force_resize_ratio的值为5）。  
 Note:什么时候dict_can_resize会为假？在前面介绍字典的应用时也说到过，一个数据库就是一个字典，数据库里的哈希类型键也是一个字典，当Redis使用子进程对数据库执行后台持久化任务时（比如执行BGSAVE或BGREWRITEAOF时），为了最大化地利用系统的copy on write机制，程序会暂时将dict_can_resize设为假，避免执行自然rehash，从而减少程序对内存的触碰（touch）。当持久化任务完成之后，dict_can_resize会重新被设为真。另一方面，当字典满足了强制rehash的条件时，即使dict_can_resize不为真（有BGSAVE或BGREWRITEAOF正在执行），这个字典一样会被rehash。


## quicklist（list使用）

## ziplist(zset/hash/list内部元素个数小于512、128个&每个元素值小于64字节时使用)

## skiplist（zset使用）