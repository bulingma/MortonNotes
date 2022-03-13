## 1、SPDK介绍
The Storage Performance Development Kit (SPDK) provides a set of tools and libraries for writing high performance, scalable, user-mode storage applications. It achieves high performance by :
* moving all of the necessary drivers into userspace, which avoids syscalls and enables zero-copy access from the application. 
* Polling hardware for completions instead of relying on interrupts, which lowers both total latency and latency variance.  
* Avoiding all locks in the I/O path, instead relying on message passing.

## 2、关键技术及取得的好处
### 2.1、关键技术
* 1、userspace(用户态)  
necessary drivers are under userspace——必要驱动都运行在用户空间
* 2、polling mode(polling模式)  
operating in a polled mode instead of relying on interrupts——操作都是polling的模式
* 3、io路径无锁化(通过message进行线程通信)

### 2.2、好处
* 1、没有了内核上下文切换 & 处理中断的开销
avoids kernel context switches and eliminates interrupt handling overhead
避免内核上下文切换和取消掉中断处理的开销

## 3、SPDK包含
### 3.1、spdk包含简介
<u>**1、The bedrock of SPDK is a user space, polled-mode, asynchronous, lockless NVMe driver.**</u>   
NVMe driver是spdk的基石

<u>**2、SPDK further provides a full block stack as a user space library that performs many of the same operations as a block stack in an operating system.[Block Device User Guide](https://spdk.io/doc/bdev.html).**</u>  
SPDK还提供完整的块堆栈作为用户空间库

<u>**3、Finally, SPDK provides NVMe-oF, iSCSI, and vhost servers built on top of these components that are capable of serving disks over the network or to other processes.**</u> 
能够通过网络或其他进程提供磁盘服务

The standard Linux kernel initiators for NVMe-oF and iSCSI interoperate with these targets, as well as QEMU with vhost. These servers can be up to an order of magnitude more CPU efficient than other implementations. These targets can be used as examples of how to implement a high performance storage target, or used as the basis for production deployments.
NVMe-oF和iSCSI的标准Linux内核启动器(initiators)与这些targets互操作，以及QEMU与vhost互操作。 这些服务器的CPU效率可以比其他实现高一个数量级。这些目标可用作如何实现高性能存储目标的示例，或用作生产部署的基础。

### 3.2、spdk主要包含的内容
The development kit currently includes:
* NVMe driver
* I/OAT (DMA engine) driver
* NVMe over Fabrics target
* iSCSI target
* vhost target
* Virtio-SCSI driver

## 4、SPDK架构
![picture 4](../../z_images/spdk-architecture.png)  
（该构架图主要对应了SPDK 18.07发布版本）  

从下往上构建，主要的组件包括：
### 4.1、存储协议(Storage Protocols)
* iSCSI target  
  建立了通过以太网的块流量规范，大约是内核LIO效率的两倍。现在的版本默认使用内核TCP/IP协议栈，后期会加入对用户态TCP/IP协议栈的集成。
* NVMe-oF target  
  实现了NVMe-oF规范。将本地的高速设备通过网络暴露出来，结合SPDK通用块层和高效用户态驱动，实现跨网络环境下的丰富特性和高性能。支持的网络不限于RDMA一种，FC，TCP等作为Fabrics的不同实现，会陆续得到支持。
* vhost target  
  KVM/QEMU的功能利用了SPDK NVMe驱动，使得访客虚拟机访问存储设备时延迟更低，使得I/O密集型工作负载的整体CPU负载减低，支持不同的设备类型供虚拟机访问，比如SCSI, Block, NVMe块设备。

### 4.2、存储设备(Storages Services)
* Block device abstration layer（bdev）  
  <u>**这种通用的块设备抽象是连接到各种不同设备驱动和块设备的存储协议的粘合剂**</u>。并且还在块层中提供灵活的API，用于额外的用户功能，如磁盘阵列、压缩、去冗等等。
* Blobstore  
  为SPDK实现一个高精简的文件式语义（非POSIX）。这可以为数据库、容器、虚拟机或其他不依赖于大部分POSIX文件系统功能集（比如用户访问控制）的工作负载提供高性能基础。
* Blobstore Block Device  
  由SPDK Blobstore分配的块设备，是虚拟机或数据库可以与之交互的虚拟设备。这些设备得到SPDK基础架构的优势，意味着零拷贝和令人难以置信的可扩展性。
* Logical Volume  
  类似于内核软件栈中的逻辑卷管理，SPDK通过Blobstore的支持，同样带来了用户态逻辑卷的支持，包括更高级的按需分配、快照、克隆等功能。
* Ceph RADOS Block Device（RBD）  
  使Ceph成为SPDK的后端设备，比如这可能允许Ceph用作另一个存储层。‍‍‍
* Linux Asynchrounous I/O（AIO）  
  允许SPDK与内核设备（比如机械硬盘）交互。

### 4.3、驱动(Drivers)
* NVMe Driver  
  SPDK的基础组件，这个高优化无锁的驱动有着高扩展性、高效性和高性能的特点。 
* Intel QuickData Technology  
  也称为Intel I/O Acceleration Technology（Inter IOAT，英特尔I/O加速技术），这是一种基于Xeon处理器平台上的copy offload引擎。通过提供用户空间访问，减少了DMA数据移动的阈值，允许对小尺寸I/O或NTB的更好利用。
* NVMe over Fabrics（NVMe-oF）initiator  
  从程序员的角度来看，本地SPDK NVMe驱动和NVMe-oF启动器共享一套共同的API命令。这意味着，例如本地/远程复制将十分容易实现。


存储协议层(Storage Protocols)，指SPDK支持存储应用类型。  
iSCSI Target对外提供iSCSI服务，用户可以将运行SPDK服务的主机当前标准的iSCSI存储设备来使用；  vhost-scsi或vhost-blk对qemu提供后端存储服务，qemu可以基于SPDK提供的后端存储为虚拟机挂载virtio-scsi或virtio-blk磁盘；  
NVMF对外提供基于NVMe协议的存储服务端；  
存储服务层(Storage Services)，该层实现了对块和文件的抽象。目前来说，SPDK主要在块层实现了QoS特性，这一层整体上还是非常薄的。  
驱动层(drivers)，这一层实现了存储服务层定义的抽象接口，以对接不同的存储类型，如NVMe，RBD，virtio，aio等等。图中把驱动细分成两层，和块设备强相关的放到了存储服务层，而把和硬件强相关部分放到了驱动层。

## 附录
[spdk](https://github.com/spdk/spdk)  
[What is SPDK](https://spdk.io/doc/about.html)    
[浅析SPDK技术和知识点](https://blog.csdn.net/BtB5e6Nsu1g511Eg5XEg/article/details/95681481)  
[spdk vhost和源码分析](https://www.cnblogs.com/yi-mu-xi/p/12107203.html)  
