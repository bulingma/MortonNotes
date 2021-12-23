


TB（Terabyte），1 TB = 1024 GB；
PB（Petabyte），1 PB = 1024 TB；
EB（Exabyte），1 EB = 1024 PB；

## 如何使用文件系统
使用一个文件系统最重要的两个步骤一个是创建一个是挂载。  
创建文件系统（windows下好像俗称格式化），意思就是将文件系统的基本组织格式写道存储设备上。

有了一个文件系统后我们并不能直接访问它，你可以访问文件系统所在的磁盘，比如你对磁盘进行读写操作都没问题。但是文件系统不能在未挂载的情况下使用。    
挂载就是将存储设备上的文件和目录和已有目录树关联，让你可以通过文件系统的逻辑来访问存储设备。  

## 现有文件系统有哪些
从历史的长河来看，出现过的文件系统非常多。现在大家比较熟知的是NTFS, Ext2/3/4，ZFS，XFS，Btrfs，NFS等。这些是目前比较主流的文件系统，其中NFS不是本地文件系统，它是通过网络虚拟出文件系统的概念来访问Server端数据，其数据端往往架设在一个现有的本地文件系统之上（如ext4），然后export出一个挂载点，供Client端挂载使用。类似这样概念的文件系统还有很多。

### NTFS  
Windows当前的默认文件系统，由于版权和组织格式（对inode不友好）等原因没有被Linux内核直接支持，但可以通过ntfs-3g用户态软件包配合Linux的FUSE机制来使用windows的NTFS文件系统。当你在Linux上插一块带有NTFS文件系统的U盘或硬盘时，挂载的时候Linux就会这样做。

### XFS
XFS是一个日志型的文件系统，能在断电以及操作系统崩溃的情况下保证数据的一致性。XFS最早是针对IRIX操作系统开发的，后来移植到linux上，目前CentOS 7已将XFS作为默认的文件系统。使用XFS已成为了潮流，所以很有必要了解下其数据结构和原理。

接下来将介绍XFS的一些概念，包括分配组、超级块、inode等等，过程中会结合xfs_db（xfs提供的输出文件系统信息的工具）打印一些信息，了解当前XFS的实时数据。

#### 分配组（Allocation Group）
XFS的第一个概念就是Allocation Group，简称AG，XFS首先将一个文件系统分成多个Group——分配组，每个分配组大小相等（最后一个可能不等）。分配组包含有超级块、inode管理和剩余空间管理等，所以分配组可以认为是一个单独的文件系统。正是分配组这样的设计，使得XFS拥有了并行IO的能力。在单个分区上使用XFS体现不了这种并行IO能力，但是如果文件系统跨越多个物理硬件比如ceph，并行IO将大大提高吞吐量利用率。  

上图为分配组的结构图，重点关注前面4个扇区，从上到下分别为<u>超级块、空闲块信息、inode信息和内部空闲列表。</u>

##### 超级块（superblock）
超级块位于分配组的第一个扇区，包含了分配组和文件系统的全部元数据信息，由于结构体比较大，这里就不作列举，可去官方文档中查看https://xfs.org/docs/xfsdocs-xm

##### 空闲块信息（AG free block info）
位于分配组的第二个扇区，主要描述两个空闲空间B+树和剩余空间信息，结构体如下：
```
typedef struct xfs_agf {
     __be32              agf_magicnum;
     __be32              agf_versionnum;
     __be32              agf_seqno;
     __be32              agf_length;
     __be32              agf_roots[XFS_BTNUM_AGF];
     __be32              agf_spare0;
     __be32              agf_levels[XFS_BTNUM_AGF];
     __be32              agf_spare1;
     __be32              agf_flfirst;
     __be32              agf_fllast;
     __be32              agf_flcount;
     __be32              agf_freeblks;
     __be32              agf_longest;
     __be32              agf_btreeblks;
} xfs_agf_t;
```
核心成员如下：

agf_roots：XFS_BTNUM_AGF为2，指明了2棵空闲空间B+树在哪个block，通过查找这两棵树找到合适的空闲block；
agf_levels：树高；
agf_freeblks：分配组目前空闲block数目

##### Inode B+树信息
位于分配组的第三个扇区，主要描述inode B+树的根block、已构造的inode个数以及空闲个数，数据结构如下：  
```
typedef struct xfs_agi {
     __be32              agi_magicnum;
     __be32              agi_versionnum;
     __be32              agi_seqno
     __be32              agi_length;
     __be32              agi_count;
     __be32              agi_root;
     __be32              agi_level;
     __be32              agi_freecount;
     __be32              agi_newino;
     __be32              agi_dirino;
     __be32              agi_unlinked[64];
} xfs_agi_t;
```
核心成员如下：  

agi_root：inode B+树的根block；  
agi_level：树高；  
agi_count：已构造的inode数目；  
agi_freecount：空闲的inode数目。  

[XFS的on-disk组织结构(1)——AG概述](https://zhuanlan.zhihu.com/p/352464797)
[关于XFS文件系统概述](https://zhuanlan.zhihu.com/p/326481091)


### ExtN  
Ext系列是Linux的老牌文件系统了，一直是Linux的主打，从Ext3开始支持日志，Ext4更加完善了很多功能。目前看好像没有Ext5这种打算，没看见Ted提过。由于其组织形式的限制，很难将很多现代文件系统的功能加进去，所以Linux才会出Btrfs项目企图重头开发一个功能强劲的文件系统。

### Btrfs  
Btrfs的目标很远大，从创立之初就被冠以Linux下一代主打文件系统的称号。但是由于各种原因其一直不能足够稳定（这里说的是面对企业级应用级别，普通用户足够用）。RedHat一直没有将其纳入自己的系统主流，可见其确实bug不断。虽然SUSE选择的是Btrfs，但是也是在一定范围内支持，很多功能是不提供稳定保证的。

### ZFS
它可以说是“目前最好”的文件系统，功能强大且稳定。Btrfs也是很大程度上想超越至少看齐ZFS的。可惜ZFS只在Solaris上提供最新最全的功能，BSD上还是略逊，Linux上则没有这个直接支持。虽然有ZFSonLinux这个项目，但是那是另一个概念，不讨论了。

### VFS  
这里我列一下VFS是为了强调一下，这个不是文件系统，这是一个文件系统上层的抽象，不是具体的某个文件系统。你跟人说文件系统包括VFS，就好像你说你编程语言学的VC++一样。



[README - 计算机专业性文章及回答总索引](https://zhuanlan.zhihu.com/p/67686817)