
<!-- TOC -->

- [存储指标](#存储指标)
- [存储平台](#存储平台)
  - [RedHat/ceph](#redhatceph)
  - [Openstack](#openstack)
- [Block Storage](#block-storage)
  - [Sheepdog](#sheepdog)
  - [AWS/EBS](#awsebs)
  - [Aliyun/盘古系统](#aliyun盘古系统)
  - [Tencent/云硬盘](#tencent云硬盘)
  - [Ceph/RBD](#cephrbd)
- [File Storage](#file-storage)
  - [GoogleFileSystem](#googlefilesystem)
  - [GFS2 (colossus-巨象)](#gfs2-colossus-巨象)
  - [FacebookHaystack](#facebookhaystack)
  - [TaobaoFileSystem](#taobaofilesystem)
  - [Google/HDFS](#googlehdfs)
  - [Ceph/CephFS](#cephcephfs)
  - [Gluster](#gluster)
- [Object Storage](#object-storage)
  - [OpenStack/Swift](#openstackswift)
  - [Ceph/RADOS](#cephrados)
  - [Amazon/S3](#amazons3)
- [KeyValue Storage](#keyvalue-storage)
  - [Facebook/Rocksdb](#facebookrocksdb)
  - [Netflix/dynomite](#netflixdynomite)
  - [google/leveldb](#googleleveldb)
  - [Tencent/MMKV](#tencentmmkv)
  - [AmazonDynamo](#amazondynamo)
  - [TaoBaoTair](#taobaotair)
- [Table Storage](#table-storage)
  - [Google/Bigtable](#googlebigtable)
  - [Google/Megastore](#googlemegastore)
  - [Microsoft/AzureStorega](#microsoftazurestorega)
- [RelationDatabase](#relationdatabase)
  - [Alibaba/PolarDB](#alibabapolardb)
  - [PingCAP/TiDB](#pingcaptidb)
  - [Anti/OceanBase](#antioceanbase)
- [NoSql(Not only Sql)](#nosqlnot-only-sql)
  - [KV存储](#kv存储)
    - [Redis](#redis)
    - [MemcacheDB](#memcachedb)
    - [Berkeley DB](#berkeley-db)
    - [Tokyo Cabinet(Tyrant)](#tokyo-cabinettyrant)
  - [列存储](#列存储)
    - [HBase](#hbase)
    - [Cassandra](#cassandra)
    - [Hypertable](#hypertable)
  - [文档存储](#文档存储)
    - [MongoDB](#mongodb)
    - [CouchDB](#couchdb)
  - [图存储](#图存储)
    - [Neo4J](#neo4j)
    - [FlockDB](#flockdb)
  - [对象存储](#对象存储)
    - [db4o](#db4o)
    - [Versant](#versant)
  - [xml数据库](#xml数据库)
    - [Berkeley DB](#berkeley-db-1)
    - [XML BaseX](#xml-basex)
- [Universal Storage](#universal-storage)
  - [Huawei/OceanStor](#huaweioceanstor)
  - [RedHat/ceph](#redhatceph-1)
  - [TileDB](#tiledb)

<!-- /TOC -->

## 存储指标
[【江枫原创】分布式存储系统的几个技术指标](https://zhuanlan.zhihu.com/p/34305823)
* 持久性 (Durability)
* 一致性 (Consistency)
* 可用性 (Availability)
* 延迟（Latency）
* 带宽（Bandwidth）
* 空间利用率（Utilization）
* 可扩展性（Scalability）
* 写屏障支持（Write Barrier）
* 安全性 (Safe)




## 存储平台
### RedHat/ceph
[ceph/ceph](https://github.com/ceph/ceph)  
Ceph is a distributed object, block, and file storage platform

[Ceph介绍及原理架构分享](https://www.jianshu.com/p/cc3ece850433)


### Openstack
[Openstack](https://github.com/openstack)  
[OpenStack](https://docs.openstack.org/xena/)   

OpenStack is a cloud operating system that controls large pools of compute, storage, and networking resources throughout a datacenter, all managed through a dashboard that gives administrators control while empowering their users to provision resources through a web interface.

## Block Storage

[ceph分布式存储简介](https://zhuanlan.zhihu.com/p/161358190)
块存储：这种接口通常以 QEMU Driver 或者 Kernel Module 的方式存在，这种接口需要实现 Linux 的 Block Device 的接口或者 QEMU 提供的 Block Driver 接口，如 Sheepdog，AWS 的 EBS，青云的云硬盘和阿里云的盘古系统，还有 Ceph 的 RBD（RBD是Ceph面向块存储的接口）。在常见的存储中 DAS、SAN 提供的也是块存储；

### Sheepdog
### AWS/EBS
### Aliyun/盘古系统
### Tencent/云硬盘
### Ceph/RBD


## File Storage
[ceph分布式存储简介](https://zhuanlan.zhihu.com/p/161358190)
文件系统存储：通常意义是支持 POSIX 接口，它跟传统的文件系统如 Ext4 是一个类型的，但区别在于分布式存储提供了并行化的能力，如 Ceph 的 CephFS (CephFS是Ceph面向文件存储的接口)，但是有时候又会把 GlusterFS ，HDFS 这种非POSIX接口的类文件存储接口归入此类。当然 NFS、NAS也是属于文件系统存储；

### GoogleFileSystem

### GFS2 (colossus-巨象)  
Colossus is the successor to the Google File System (GFS) as mentioned in the paper on Spanner at OSDI 2012. Colossus is also used by spanner to store its tablets. 

[Colossus: Successor to the Google File System (GFS)](https://www.systutorials.com/colossus-successor-to-google-file-system-gfs/)

### FacebookHaystack
### TaobaoFileSystem
### Google/HDFS
[HDFS](https://www.jianshu.com/p/f1e785fffd4d)  

### Ceph/CephFS
[一篇文章讲透分布式存储](https://zhuanlan.zhihu.com/p/55964292)  

### Gluster

## Object Storage
[请问，块存储，对象存储，分布式存储本质区别？](https://www.zhihu.com/question/52274164)  

对象存储：也就是通常意义的键值存储，其接口就是简单的GET、PUT、DEL 和其他扩展，代表主要有 Swift 、S3 以及 Gluster 等；

### OpenStack/Swift
OpenStack Object Storage（Swift）是OpenStack开源云计算项目的子项目之一，被称为对象存储，提供了强大的扩展性、冗余和持久性。

### Ceph/RADOS

### Amazon/S3  
Amazon Simple Storage Service



## KeyValue Storage

### Facebook/Rocksdb
[Facebook/Rocksdb](https://github.com/search?q=org%3Afacebook+storage)  

### Netflix/dynomite
[Netflix/dynomite]([Netflix/dynomite](https://github.com/Netflix/dynomite))  

### google/leveldb
[google/leveldb](https://github.com/google/leveldb)  

### Tencent/MMKV
[Tencent/MMKV](https://github.com/Tencent/MMKV)  

### AmazonDynamo
### TaoBaoTair

## Table Storage
### Google/Bigtable
### Google/Megastore
### Microsoft/AzureStorega

## RelationDatabase
### Alibaba/PolarDB
[PolarDB—Alibaba]()

### PingCAP/TiDB
[TiDB—PingCAP](https://github.com/pingcap/tidb)  

### Anti/OceanBase
[OceanBase—Anti](https://github.com/oceanbase/oceanbase)


## NoSql(Not only Sql)
[什么是Cassandra？](https://zhuanlan.zhihu.com/p/265027506)     
[NoSQL 数据库你真的清楚明白吗？](https://zhuanlan.zhihu.com/p/135993569)  

### KV存储
#### Redis
#### MemcacheDB  
#### Berkeley DB
#### Tokyo Cabinet(Tyrant)  

### 列存储
#### HBase
Apache HBase-HBase是一种以Google的BigTable建模的开源，非关系，分布式数据库，并使用Java编写。它是Apache Hadoop项目的一部分，在HDFS之上运行，为Hadoop提供类似BigTable的功能。  

#### Cassandra
Apache Cassandra是一个开源，分布式和分散/分布式存储系统（数据库），用于管理分布在世界各地的大量结构化数据。它提供高可用性服务，没有单点故障。  

#### Hypertable

### 文档存储
#### MongoDB
MongoDB-MongoDB是一个跨平台的面向文档的数据库系统，它避免使用传统的基于表的关系数据库结构，而是使用具有动态模式的类似JSON的文档，从而使数据在某些类型的应用程序中的集成更加容易和快捷。  

#### CouchDB

### 图存储
#### Neo4J
#### FlockDB


### 对象存储
#### db4o
#### Versant

### xml数据库

#### Berkeley DB

#### XML BaseX


## Universal Storage
对于<u>集中式存储</u>而言，统一存储一般是指SAN（主要分为IP SAN和FC SAN）与NAS（主要分为CIFS和NFS）的融合，主要是存储厂商在主导，例如华为的OceanStor系列。
对于<u>分布式存储</u>而言，统一存储一般是指对象存储（Object Storage）、文件存储（File Storage）和块存储（Block Storage）的统一，主要是开源文件系统在主导，例如已被红帽收购的“当红炸子鸡”Ceph。
[什么是统一存储？](https://www.zhihu.com/question/343713625#:~:text=%E7%BB%9F%E4%B8%80%E5%AD%98%E5%82%A8%E6%98%AF%E6%8C%87%E5%90%8C%E4%B8%80,%E4%B9%89%E7%9A%84%E5%88%86%E5%B8%83%E5%BC%8F%E6%9E%B6%E6%9E%84%E3%80%82)


### Huawei/OceanStor
[OceanStor 混合闪存存储](https://e.huawei.com/cn/products/storage/hybrid-flash-storage)

### RedHat/ceph
[ceph/ceph](https://github.com/ceph/ceph)
Ceph is a distributed object, block, and file storage platform

### TileDB
[TileDB](https://github.com/TileDB-Inc/TileDB)







