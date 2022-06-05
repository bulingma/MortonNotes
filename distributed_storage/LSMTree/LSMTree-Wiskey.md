
## 背景  
<u>适合磁盘的索引结构通常有两种,**一种是就地更新(B+Tree),一种是非就地更新(LSM)**。就地更新的索引结构拥有最好的读性能(随机读与顺序读)，而随机写性能很差，无法满足现实工业中的工作负载要求。而非就地更新的索引结构-LSM充分发挥顺序写入的高性能特性，成为写入密集的数据系统的基础。</u>  

<u>LSM-tree(Log-Structured Merged-tree) 现如今已经被广泛应用在了各个NoSQL 存储系统中，如BigTable, Dynamo, HBase, Cassandra, LevelDB, RocksDB 和 AsterixDB。相比于传统的in-place updates(就地更新) 索引结构，LSM-tree 第一次写入都缓存到内存中，并通过后台的flush来顺序追加到磁盘中，也就是out-of-palce updates。 **LSM-tree这样的实现方式有非常多的优点，包括写性能的提升、较高的空间利用率、简单的并发控制和异常恢复等**。这些优点使LSM树可以服务于多种场景，如Facebook开发的基于LSM-tree的存储引擎 rocksdb， 被广泛应用在实时数据处理/图数据处理/流式数据处理以及 OLTP(on line transaction processing) 等多种workload。</u>  

## LSMTree关键点 
* 分层设计      
解决最先写入且从未更新的key要读取全量数据的问题，为解决这个问题LSM结构引入了分层设计的思想。将所有的kv文件分为c0-ck 共k+1层。<u>c0层是直接从不变的内存表中dump下的结果，而c1-ck是发生过合并的文件</u>。<u>由于ci+1 是ci中具有重叠部分的文件合并的产物，因此可以说在同一层内是不存在重叠key的，因为重叠key已经在其上一层被合并了</u>。那么只有c0层是可能存在重叠的文件的。<u>所以当要读取磁盘上的数据时，最坏情况下只需要读取c0的所有文件以及c1-ck每一层中的一个文件即c0+k个文件即可找到key的位置，分层合并思想使得非就地更新索引在常数次的IO中读取数据</u>。  

* SSTable(Sorted string tables)   
通常c0文件定义为2M，每一级比上一级大一个数量级的文件大小。所以高层的文件难以被一次性的加载到内存，因此需要一定的磁盘索引机制。<u>我们对每个磁盘文件进行布局设计，分为元数据块，索引块，数据块三大块。元数据块中存储布隆过滤器快速的判断这个文件中是否存在某个key，同时通过对排序索引(通常缓存在内存中)二分查找定位key所在磁盘的位置。进而加速读取的速度，我们叫这种数据文件为SSTABLE(字符串排序表)。</u>  
为了标记哪些SStable属于那一层因此要存在一个sstable的元数据管理文件，在levelDB中叫做MANIFEST文件。其中存储每一个sstable的文件名，所属的级别，最大与最小key的前缀。  

* 预写日志（Write ahead log）    
<u>作为一个DB引擎，必须保证数据库进程崩溃前后的数据一致性，常见的做法就是使用预写日志。</u>
将所有的操作记录在一个仅追加的log文件中(称之为WAL(Write ahead log))，所有的写入操作都要保证写入WAL成功后才能继续，因此当数据库崩溃后写入WAL的操作将被回溯，反之则被丢弃(只有写入WAL成功才会回复客户端ack)。那么从尾部重放这个WAL文件的操作即可恢复DB。  
但是这个过程由于会消耗磁盘的空间因此也需要不断的进行压缩，同时<u>如果WAL过大也会使得数据库恢复的时间增大这是不可接受的，为此我们需要支持checkpoint特性</u>。  

## 机遇与挑战
LSM 是以牺牲读取性能以及空间利用率为代价而换取顺序写入性能的。因此，对LSM结构的优化目标就是想办法提高读取性能和空间利用率。   
  
详见原文

### KV分离对SSD的优化  
* KV分离设计  

* 合并压缩  
 
* 故障恢复     


## 参考
[LSM Tree论文精读 -专为SSD优化的wisckey实现—bilibili视频](https://www.bilibili.com/video/BV16A411V7DE?p=4)  
[wiscKey论文精读](https://hardcore.feishu.cn/docs/doccnxbpzJQ5KJulm5YtGPULRxc#eQXr5r)    
[WiscKey：在SSD存储上的键值分离设计](https://hardcore.feishu.cn/docs/doccn4WRb8aFRznvfyc8hRf9Chf)    
[LSM-论文导读与Leveldb源码解读](https://hardcore.feishu.cn/docs/doccnKTUS5I0qkqYMg4mhfIVpOd)    
[BadgerDB源码分析之Wisckey论文](https://shimingyah.github.io/2019/08/BadgerDB%E6%BA%90%E7%A0%81%E5%88%86%E6%9E%90%E4%B9%8Bwisckey%E8%AE%BA%E6%96%87/)    
[dgraph-io/badger](https://github.com/dgraph-io/badger)   
BadgerDB is an embeddable, persistent and fast key-value (KV) database written in pure Go. It is the underlying database for Dgraph, a fast, distributed graph database. It's meant to be a performant alternative to non-Go-based key-value stores like RocksDB.  