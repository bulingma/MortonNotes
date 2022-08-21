<!-- TOC -->

- [第4章 存储加速](#第4章-存储加速)
	- [4.1 基于**CPU**处理器的加速和优化方案](#41-基于cpu处理器的加速和优化方案)
	- [4.2 基于协处理器或其他硬件的加速方案](#42-基于协处理器或其他硬件的加速方案)
	- [4.3 智能存储加速库(**ISA-L**)](#43-智能存储加速库isa-l)
	- [4.4 存储性能软件加速库(**SPDK**)](#44-存储性能软件加速库spdk)
		- [4.4.1 SPDK NVMe驱动](#441-spdk-nvme驱动)
			- [1.用户态驱动](#1用户态驱动)
			- [2.SPDK用户态驱动](#2spdk用户态驱动)
			- [3.SPDK NVMe驱动性能](#3spdk-nvme驱动性能)
			- [4.SPDK NVMe驱动新特性](#4spdk-nvme驱动新特性)
			- [5.SPDK用户态驱动多进程的支持](#5spdk用户态驱动多进程的支持)
		- [4.4.2 SPDK应用框架](#442-spdk应用框架)
		- [4.4.3 SPDK用户态块设备层](#443-spdk用户态块设备层)
		- [4.4.4 SPDK vhost target](#444-spdk-vhost-target)
		- [4.4.5 SPDK iSCSI Target](#445-spdk-iscsi-target)
			- [1.SPDK iSCSI Target加速设计和实现](#1spdk-iscsi-target加速设计和实现)
			- [2.在Linux环境下配置SPDK iSCSI Target示例](#2在linux环境下配置spdk-iscsi-target示例)
		- [4.4.6 SPDK NVMe-oF Target](#446-spdk-nvme-of-target)
		- [4.4.7 SPDK RPC](#447-spdk-rpc)
			- [1.RPC](#1rpc)
			- [2.JSON](#2json)
			- [3.JSON-RPC](#3json-rpc)
			- [4.SPDK JSON-RPC](#4spdk-json-rpc)
			- [5.SPDK JSON-RPC运行机制](#5spdk-json-rpc运行机制)
		- [4.4.8 SPDK生态工具介绍](#448-spdk生态工具介绍)

<!-- /TOC -->

## 第4章 存储加速

### 4.1 基于**CPU**处理器的加速和优化方案

### 4.2 基于协处理器或其他硬件的加速方案

### 4.3 智能存储加速库(**ISA-L**)

### 4.4 存储性能软件加速库(**SPDK**)

#### 4.4.1 SPDK NVMe驱动

##### 1.用户态驱动

1)UIO

2)VFIO

3)用户态DMA

4)大页(Hugepage)

##### 2.SPDK用户态驱动

1)异步轮询方式

2)无锁化

· 读/写处理要在一个CPU核上完成，避免核间的缓存同步。

· 单核上的处理，对资源的分配是无锁化的。

这其中涉及第二个问题，在处理该核上的读/写请求时，需要分配 相关的资源，如Buffer。这些Buffer主要通过大页分配而来。DPDK为 SPDK提供了基础的内存管理，单核上的资源依赖于DPDK的内存管 理，不仅提供了核上的专门资源，还提供了高效访问全局资源的数据结 构，如mempool、无锁队列、环等。

3)专门为Flash来优化

内核驱动的设计以通用性为主，考虑了不同的硬件设备实现一个通 用的块设备驱动的问题。这样的设计有很好的兼容性和维护性，但是单 从性能角度看，不一定能发挥出特定性能的优势。

SPDK作为用户态驱动，就是专门针对高速NVMe SSD设备的。为 了能让上层应用程序充分利用硬件设备的高性能(高带宽、低延时)， SPDK实现了一组C代码开发库，这些开发库的接口可以直接和应用程 序结合起来。

通过UIO或VFIO把PCI设备的BAR(Base Address Register)地址映 射到应用进程的空间，这样SPDK用户态驱动就可以遵循NVMe的规范 来初始化NVMe SSD，创建出最基本的I/O发送和完成队列，最终实现对 NVMe SSD设备的I/O读或写操作。



我们再来看下Linux kernel和SPDK的对比，<u>SPDK用户态驱动的这些技术能否更为高效地使用单CPU核</u>。基于下面的测试环境，图4-17显 示了单CPU核情况下SPDK和Linux Kernel的可扩展性测试结果，在单个 CPU核上E5-2695v4，通过4KB的随机读测试、128的队列深度，SPDK 可以将8块Intel P3700 NVMe SSD设备的性能上限跑出来。如果要达到类似的高性能的话，<u>内核驱动至少需要使用8个CPU核</u>。

总结下来，<u>SPDK用户态驱动是专门为NVMe SSD优化的，尤其是对高速NVMe SSD，比如基于3D XPoint的Intel Optane设备，能够在单 CPU核上管理多个NVMe SSD设备，实现高吞吐量、低延时、多设备、 高效CPU使用的特点</u>。

##### 3.SPDK NVMe驱动性能

##### 4.SPDK NVMe驱动新特性

SPDK会随着NVMe规范的丰富不断引入新的特性到用户态驱动里面。

· Reservations:可以很好地支持双控制器的NVMe SSD(如Intel D3700)，在需要高可靠性的场景下，达到控制器的备份冗余。

· Scatter Gather List(SGL):可以更灵活地分配内存，减少I/O操作，提供高效的读/写操作。

· Multiple Namespace:可以暴露给上层应用多个逻辑空间，做到在同一物理设备上的共享和隔离。

· In Controller Memory Buffer(CMB):可以把I/O的发送和完成 队列放到固态硬盘设备上，同时相应的Buffer也从固态硬盘设备上来分 配，一方面可以减少延时，另一方面使得两个NVMe SSD设备间的DMA 成为可能。

##### 5.SPDK用户态驱动多进程的支持

前面提到SPDK用户态驱动会暴露对应的API给应用程序来控制和操作硬件设备。此时内核NVMe驱动已经不会对设备做任何的操作，所以类似于/dev/nvme0和/dev/nvme0n1的设备不会存在。这样带来一个问题，如果多个应用程序都需要访问同一个硬件设备的话，那么SPDK用户态驱动该如何来支持。

1)共享内存

在初始化这些共享资源之前，我们给相关的进程做了区分，可以显示指定某个进程为主进程(Master Process)，或者系统自动判断第一个 进程为主进程。当主进程启动的时候，把相关的资源分配好，同时初始 化完成需要共享的资源。当配置副进程(Slave Process)的应用启动 时，无须再去分配内存资源，只需要通过共同的标识符来匹配主进程，把相关的内存资源配置到副进程上即可。

2)共享NVMe SSD

为了实现数据通道上的无锁化以保证高性能，我们更关注的是数据 通道上的隔离。SPDK在单CPU核的情况下，可以很容易地具备低延 时、高带宽的特性，这些性能指标只需要依赖少数甚至单个I/O队列就 可以达到。因此这里的I/O队列是需要让某个进程从逻辑上单独使用 的，即便整个NVMe SSD是对多个进程共享可见的。SPDK的用户态驱 动对单独I/O队列是无锁化处理的，因此从性能考虑，只需要应用程序分配自己的I/O队列就可以达到较高的性能。

另外，由于NVMe SSD本身只有一个管理队列，因此当多个应用程 序需要对设备发起相应的管理操作时，这个管理队列需要通过互斥的机 制来保证操作的顺序性。相对来说，在控制通道上引入互斥机制对每个 进程影响不会很大。同时，为了能够记录和回调每个应用对设备发起的 管理操作，这里引入了逻辑上每个进程独有的管理队列的完成队列来完 成对设备的控制。

3)管理软件完成队列

4)NVMe SSD共享管理流程

主进程在初始化的时候，会首先分配一个带名字且共享的资源，这 样副进程可以通过名字来获得这个共享的资源。这里需要提到的是，任 何需要共享的资源都应该放在某个带名字的共享资源下(见图4-23左侧 的步骤1)。当主进程完成了共享资源的分配后，将初始化用来同步进 程的互斥机制(见图4-23左侧的步骤2)，然后初始化所有的硬件设 备，完成设备的启动(见图4-23左侧的步骤3-4)。如果当前进程需要分配I/O队列的话，在设备正常启动后可以分配逻辑上独享的I/O队列来进 行设备的操作(见图4-23左侧的步骤5)。

副进程在启动的时候，首先需要做的是去查找这个带名字的共享资 源(见图4-23右侧的步骤1)，然后通过共享内存的机制访问该共享资 源下所有由主进程分配和初始化的资源。同时通过PID的方式创建特定 的数据结构来保存属于当前副进程的资源(见图4-23右侧的步骤)。因 为设备已经由主进程完成了正常启动，所以副进程可以直接向该设备发 送管理请求来创建I/O队列(见图4-23右侧的步骤3)。

SPDK用户态驱动提供了对多进程访问的支持后，有几个典型的使

用场景。

· 主进程完成对设备的管理和读/写操作，副进程来监控设备，读取设备使用信息。

· 主进程只负责资源的初始化和设备的初始化工作，多个副进程来操作设备，区分设备的管理通道和数据通道。

· 当主进程和副进程不进行区分时，都会对设备进行管理和读/写操作。

需要注意的是，在使用SPDK提供的该功能的时候，或者显示指定主进程，或者让系统来默认指定主进程，不可以出现都是显示指定副进 程的场景。同时考虑到任何一个进程都有可能出现异步退出的场景，所以需要引入相关的锁机制和资源清理机制来保证资源的正常释放，以及后续进程的正常启动。



#### 4.4.2 SPDK应用框架

虽然NVMe的底层函数有一些说明，但为了更好地发挥出底层 NVMe的性能，SPDK提供了一套编程框架(Application Framework)如 图4-24所示，用于指导软件开发人员基于SPDK的用户态NVMe驱动及用 户态块设备层构造高效的存储应用。

总的来说，SPDK的应用框架可以分为:1 对CPU core和线程的管理;2 线程间的高效通信;3 I/O的处理模型;4 数据路径的无锁化机制。

* 1)对CPU core和线程的管理

SPDK的原则是使用最少的CPU核和线程来完成最多的任务。为此 SPDK在初始化程序的时候限定使用绑定CPU的哪些核。可以在配置文 件或命名行中配置，如在命令行中使用“-c 0x5”，这是指使用core 0和 core 2来启动程序。

<u>也就是说，一个使用SPDK编程框架的应用，假设使用了两个CPU core，每个core上就会启动一个Reactor thread，那么用户怎么执行自己 的函数呢?为了解决这个问题，SPDK提供了一个Poller机制。所谓Poller，其实就是用户定义函数的封装。SPDK提供的Poller分为两种: 基于定时器的Poller和基于非定时器的Poller。SPDK的Reactor thread对应的数据结构由相应的列表来维护Poller的机制，比如一个链表维护定时器的Poller，另一个链表维护非定时器的Poller，并且提供Poller的注册及 销毁函数。在Reactor的while循环中，会不停地检查这些Poller的状态， 并且进行相应的调用，这样用户的函数就可以进行相应的执行了。</u>由于单个CPU核上，只有一个Reactor thread，所以同一个Reactor thread中不需要一些锁的机制来保护资源。当然位于不同CPU核上的thread还是有通信的必要的。为此，SPDK封装了线程间异步传递消息(Async Messaging Passing)的功能。

* 2)线程间的高效通信

<u>SPDK放弃使用传统的、低效的加锁方式来进行线程间的通信。为了使同一个thread只执行自己所管理的资源，SPDK提供了事件调用 (Event)的机制。这个机制的本质是每个Reactor对应的数据结构维护 了一个Event事件的环，这个环是多生产者和单消费者(Multiple Producer Single Consumer，MPSC)的模型，意思是每个Reactor thread 可以接收来自任何其他Reactor thread(包括当前的Reactor thread)的事件消息进行处理。</u>

目前SPDK中这个Event环的默认实现依赖于DPDK的机制，这个环应该有线性的锁的机制，但是相比较于线程间采用锁的机制进行同步， 要高效得多。毫无疑问的是，这个Event环其实也在Reactor的函数 _spdk_reactor_run中进行处理。每个Event事件的数据结构包括了需要执行的函数和相应的参数，以及要执行的core。

<u>简单来说，一个Reactor A向另外一个Reactor B通信，其实就是需要 Reactor B执行函数F(X)，X是相应的参数。</u>基于这样的机制，SPDK 就实现了一套比较高效的线程间通信的机制，具体例子可以参照SPDK NVMe-oF Target内部的一些实现，主要代码位于lib/nvmf目录下。

* 3)I/O的处理模型及数据路径的无锁化机制

SPDK主要的I/O处理模型是运行直到完成。如前所述，使用SPDK 应用框架，一个CPU core只拥有一个thread，这个thread可以执行很多 Poller(包括定时器和非定时器)。运行直到完成的原则是让一个线程最好执行完所有的任务。

显而易见，SPDK的编程框架满足了这个需要。如果不使用SPDK应 用编程框架，则需要编程者自己注意这个事项。比如使用SPDK用户态 NVMe驱动访问相应的I/O QPair进行读/写操作，SPDK提供了异步读/写 的函数spdk_nvme_ns_cmd_read，以及检查是否完成的函数 spdk_nvme_qpair_process_completions，这些函数的调用应当由一个线程 去完成，而不应该跨线程去处理。



#### 4.4.3 SPDK用户态块设备层

* 1.内核通用块层

Linux操作系统的设计总体上是需要满足应用程序的普遍需求的， 因此在设计模块的时候，考虑更多的是模块的通用性。针对内核块设备驱动而言，每个不同的块设备都会有自己特定的驱动，<u>与其让上层模块和每一个设备驱动来直接交互，不如引入一个通用的块层。这样设计的好处在于:一是容易引入新的硬件，只需要新硬件对应的设备驱动能接入通用的块层即可; 二是上层应用只需要设计怎么和通用块层来交互， 而不需要知道具体硬件的特性。</u>

当然，如果要特别地发挥某个硬件的特性，上层应用直接和设备驱动交互是值得推荐的方式。通用块层的引入除了可以提供上面两个优点，还可以支持更多丰富的功能，如下所示。

<u>· 软件I/O请求队列</u>: 更多的I/O请求可以在通用块层暂时保存，尤其是某些硬件本身不支持很高的I/O请求并发量。

<u>· 逻辑卷管理</u>: 包括对一个硬件设备的分区化，多个硬件的整体化逻辑设备，比如支持不同的磁盘阵列级别和纠删码的逻辑卷。又如快照、克隆等更高级的功能。

<u>· 硬件设备的插拔</u>: 包括在系统运行过程中的热插拔。

<u>· I/O请求的优化: 比如小I/O的合并，不同的I/O调度策略。</u>

<u> · 缓存机制</u>: 读缓存，不同的写缓存策略。 

<u>· 更多的软件功能</u>: 基于物理设备和逻辑设备。

由此可见，通用块层的重要性，除了对上层应用和底层硬件起承上启下的作用，更多的是提供软件上的丰富功能来支撑上层应用的不同场景。

* 2.SPDK用户态通用块层

如前所述，上层应用是通过SPDK提供的API来直接操作NVMe SSD硬件设备的。这是一个典型的让上层应用加速使用NVMe SSD的场景。 但是除了这个场景，上层应用还有更多丰富的场景，<u>如后端管理多种不同的硬件设备，除了NVMe SSD，还可以是慢速的机械磁盘、SATA SSD、SAS SSD，甚至远端挂载的设备。又如需要支持设备的热插拔、 通过逻辑卷共享一个高速设备等存储服务</u>。复杂的存储应用需要结合不同的后端设备，以及支持不同的存储软件服务。<u>值得一提的是，有些上层应用程序还需要文件系统的支持，在内核态的情况下，文件系统也是建立在通用块层之上的</u>。类似的文件系统的需求在SPDK用户态驱动中也需要提供相应的支持。

由此可见，在结合SPDK用户态驱动时，也需要SPDK提供类似的用 户态通用块层来支持复杂和高性能的存储解决方案。另外，在考虑设计用户态通用块层的时候，也要考虑它的可扩展性，比如是否能很容易地 扩展来支持新的硬件设备，这个通用块层的设计是不是高性能的，是否 可以最小限度地带来软件上的开销，以充分发挥后端设备的高性能。

图4-25 SPDK架构

SPDK架构解析如下。

<u>· 最下层为驱动层，**管理物理和虚拟设备，还管理本地和远端设备。**</u>

<u>· 中间层为通用块层，实现对不同后端设备的支持，提供对上层的统一接口，**包括逻辑卷的支持、流量控制的支持等存储服务。这一层也提供了对Blob(Binary Larger Object)及简单用户态文件系统BlobFS 的支持。**</u>

<u>· 最上层为协议层，**包括NVMe协议、SCSI协议等，可以更好地和上层应用相结合。**</u>

如前所述，SPDK应用框架采用的优化思想，在SPDK通用块层也是 类似的实现。包括从内存资源分配上、I/O资源池、大小Buffer资源池 等，既要考虑全局总的分配数量，也要考虑每个CPU核独享的资源。这 样在单线程、单核的情况下，可以实现资源的快速存取，也要考虑不要 给单核分配过多的资源而造成资源浪费。

每个核上，SPDK实现了单线程的高性能使用。线程的数量和核的 数量对应关系是1∶1的匹配，所有单核上的操作由一个线程来完成，这 样可以很好地实现单核上的无锁化。同时采用运行直到完成的I/O处理 方式，保证了一个I/O的资源分配和核心操作在同一个核上完成，避免 了额外的核间同步的问题。

为了达到这个目的，在通用块层引入了逻辑上的I/O Channel概念来 屏蔽下层的具体实现。目前来说，I/O Channel和Thread的对应关系也是 1∶1的匹配。这样总的匹配如下:

```
I/O Channel : Thread  :  Core = 1 : 1 : 1
```

SPDK Bdev设计主要考虑以下几个维度:  一是，抽象出来的通用设备需要数据结构来维护;  二是，操作通用设备的函数指针表，提供给后端具体硬件来实现;  三是，I/O数据结构用来打通上层协议模块和下层驱动模块。下面我们具体来看一下这些核心的数据结构。

** 通用块设备的数据结构:  

需要包括标识符如名字、UUID、对应的后端硬件名字等; 块设备的属性如大小、最小单块大小、对齐要求等;操作该设备的方法如注册和销毁等;该设备的状态，如重置、记录相关功能的数据结构等。具体可以**参考SPDK源码中的struct spdk_bdev 结构体**。

** 操作通用设备的函数指针表:

<u>定义通用的操作设备的方法。包括如何拿到后端具体设备相应的I/O Channel、后端设备如何处理 I/O(Read、Write、Unmap等)、支持的I/O类型、销毁后端具体块设备等操作。</u>每一类具体的后端设备都需要实现这张函数指针表，使得通用块设备可以屏蔽这些实现的细节，只需要调用对应的操作方法就可以 了。具体可以**参考SPDK源码中的struct spdk_bdev_fn_table结构体**。

** 块设备I/O数据结构:

类似于内核驱动中的bio数据结构，同样需要一个I/O块数据结构来具体操作块设备和后端对应的具体设备。<u>具体的I/O读和写的信息都会在这个数据结构中被保存，以及涉及的Buffer、 Bdev Channel等相关资源，后期需要结合高级的存储特性像逻辑卷、流量控制等都需要在I/O数据结构这里有相关的标识符和额外的属性。</u>具体可以**参考SPDK源码中的struct spdk_bdev_io结构体**。



<u>这些核心的数据结构，提供了最基本的功能上的特性来支持不同的后端设备，比如通过SPDK用户态NVMe驱动来操作NVMe SSD; 通过Linux AIO来操作除NVMe SSD外的其他慢速存储设备比如HDD、SATA SSD、SAS SSD等; 通过PMDK(Persistent Memory Development Kit) 来操作英特尔的Persistent Memory设备; 通过Ceph RBD(Reliable Block Device)来操作远端Ceph OSD设备; 通过GPT(GUID Partition Table) 在同一设备上创建逻辑分区; 等等</u>。

同时这些数据结构定义了清楚的使用方法。<u>例如，函数指针表来支持新后端设备的引入，可以是某种本地新的硬件设备，也可以是某种远端分布式存储暴露出来的虚拟设备。在可扩展性上除了一定需要支持的高速NVMe SSD设备，还提供了对传统设备及新设备的支持。</u>

在设计通用块层的数据流的时候，需要考虑后端不同设备的特性， 比如某些设备可以支持很高的并发量，某些设备无法支持单个I/O的终止操作(Abort)，对数据流上的考虑大致包括以下内容。

** 引入I/O队列来缓存从上层模块接收到的I/O请求，而不是直接传递给下层。

这样不同的后端设备都可以按照不同的速率来完成这些I/O 请求。同时基于这个I/O队列还能起到一些额外的作用，比如限速流控、不同优先级处理、I/O分发等。当后端设备遇到一些异常情况时， 比如当Buffer资源不够时，这个I/O队列也可以重新把这些发下去的I/O 请求再次进入队列做第二次读/写尝试。

** 引入通用的异常恢复机制。

比如某个I/O请求可能在下层具体设备停留过久导致的超时问题; 比如设备遇到严重问题导致无法响应而需要设备重置; 比如设备的热插拔导致的I/O请求的出错处理。与其让每 一个下层具体设备都来实现这些异常恢复机制，不如在通用块层就来进行处理。

能够让通用块层起到承接上层应用的读/写请求，高性能地利用下层设备的读/写性能，在实现高性能、可扩展性的同时，还需要考虑各种异常情况、各种存储特性的需求。这些都是在实现数据流时需要解决的问题。

* 3.通用块层的管理

管理通用块层涉及两方面问题，一方面是，对上层模块、对具体应用是如何配置的，怎么样才能让应用实施到某个通用块设备。<u>这里有两种方法，一种是通过配置文件，另一种是通过远程过程调用(RPC)的方法在运行过程中动态地创建和删除新的块设备。</u>

当我们引入更多的存储特性在通用块层的时候，我们可以把块设备分为两种:支持直接操作后端硬件的块设备，可以称之为基础块设备 (Base Bdev); 构建在基础块设备之上的设备，比如逻辑卷、加密、 压缩块设备，称之为虚拟块设备(Virtual Bdev)。

* 4.逻辑卷   

类似于内核的逻辑卷管理，SPDK在用户态也实现了基于通用块设备的逻辑卷管理。

1)内核LVM

2)Blobstore

​	Blobstore是支撑SPDK逻辑卷的核心技术。Blobstore本质上是一个 Block的分配管理。如果后端的具体设备具有数据持久性的话，如NVMe SSD，那么Block分配的这些信息，或者元数据可以在断电的情况下被 保留下来，等下次系统正常启动时，对应的Block的分配管理依旧有效。
 	这个Block的分配管理可为上层模块提供更高层次的存储服务，比如这里提到的逻辑卷管理，以及下面将要介绍的文件系统。这些基于 Blobstore的更高层次的存储服务，可以为本地的数据库，或者Key/Value仓库(RocksDB)提供底层的支持。



3)SPDK用户态逻辑卷

SPDK用户态逻辑卷基于Blobstore和Blob。每个逻辑卷是多个Blob的组合，它有自己唯一的UUID和其他属性，如别名。

对上层模块而言，这里我们引入一个类似的概念，逻辑卷块设备。 对逻辑卷块设备的操作会转换成对SPDK Blob的操作，最终还是依照之 前Blob的层次结构，转换成对Cluster、页和设备逻辑块的操作。

基于逻辑卷的功能，可以通过对Blob的不同处理来实现更高级别的存储服务，<u>比如如果这个Blob中的Cluster是在上层模块写入时才分配的，那么这种特性就是常见的**精简配置**，可以达到空间的高效使用。相反地，逻辑卷在创建的时候就把Blob的物理空间分配出来的特性，可以称之为**密集配置**。</u>这样的操作可以保证写性能的稳定性，没有额外分配 Cluster而引入的性能开销。

使用快照可以很快地生成某个时刻的数据集，尤其是对上层应用是 读密集型的场景，可以按需要生成多个快照给应用，同时能保证稳定的 读性能。但是快照需要考虑一个潜在的数据可靠性的限制，因为多个快 照都是指向同一个Cluster的，如果这个Cluster出现数据问题的话，则会 导致所有引用这个Cluster的快照都出现问题。

因此我们引入克隆这个特性，顾名思义把某个时间点的数据集复制 一份，而且是分配同等大小空间的数据集。这个过程需要花费时间，并 且依赖于需要做克隆的数据集的大小，但是一旦克隆完成，将会提供更 高的性能，比如新的读操作可以直接在克隆上完成。同时可以提供更好 的可靠性，即使原先的数据出现问题，克隆的数据还是有效的。从实现 来说，在复制某一时刻的数据集时，是通过先在那个时刻做一个快照， 将那个时刻的数据集固定下来，后续的读和写都可以基于相同时刻的快 照。



* 5.基于通用块的流量控制

流量控制是高级存储特性中一种常用的实现，在内核通用块层也有类似的实现，流量控制的需求主要来自以下两方面。 

· 多个应用需要共享一个设备，不希望出现的场景是某个应用长时间通过高I/O压力占用该设备，而影响其他应用对设备的使用。 

· 给某些应用指定预留某些带宽，这类应用往往有高于其他应用的优先级。

SPDK的流量控制，是基于通用块层来实现的，这样设计的好处如下所示。

· 可以是任何一个通用块设备，前面我们介绍了基础块设备、虚 拟块设备、逻辑卷等，只要是块设备，流量控制都可以在上面启用。这 样的设计可以很好地结合SPDK通用块层的特性。

· 和上层各种协议无关，无论是本地的传输协议，还是跨网络的 传输协议，都可以很好地支持。这些上层模块需要关心的是，各种协议 的设备和通用块设备是如何对应的。比如在iSCSI的场景中，暴露出来 的LUN是怎么和SPDK通用块设备对应的。这样，当LUN的用户需要启 用流量控制的时候，对相应的通用块设备设置流量控制就可以了。

· 和后端具体设备无关，无论是本地的高速NVMe SSD、低速的硬 盘驱动器，还是某个远端的块设备，这些具体的硬件已经由SPDK通用 块层隐藏起来了。但是需要注意的是，流量控制本身是不会提高硬件的 自身能力的上限的，需要给出合理的流量控制的目标。

前面我们详细描述了通用块层的一些主要结构，包括通用块设备结 构、I/O请求结构、I/O Channel结构、每个Channel的上下文块设备通道 结构等，也具体描述了通用块层的线程模型。这里涉及流量控制的主要 操作流程，就是结合这些重要数据结构和线程模型来实现资源上的管理 和I/O请求上的管理的。

SPDK基于通用块层的流量控制提供了很好的扩展性。在算法层面 上的实现简单明确，如果需要引入更高级的流量控制算法，可以很容易 地替换默认的算法，也可以支持其他更多种类的流量控制，比如读/写 分开的IOps，带宽限速;比如读/写不同优先级的区别控制;等等。 SPDK通用块层和SPDK应用框架为这一存储服务提供了很好的技术保障 和可扩展性。

#### 4.4.4 SPDK vhost target
主流的I/O设备虚拟化的方案:
· 纯软件模拟：完全利用软件模拟出一些设备给虚拟机使用，主要的工作可以在Simics、Bochs、纯QEMU解决方案中看到。
· 半虚拟（Para-Virtualization）：主要是一种frontend-backend的模型，在虚拟机中的Guest  OS中使用frontend的驱动，Hypervisor中暴露出backend接口。这种解决方案需要修改Guest OS，或者提供半虚拟化的前端驱动。
· 硬件虚拟化：主流的方案有SR-IOV、VT-D等，可以把整个设备直接分配给一个虚拟机，或者如果设备支持SR-IOV，就可以把设备的VF（Virtual Function）分配给虚拟机。

对于以上3种虚拟化的解决方案，我们会把重点放在virtio解决方案，即半虚拟化上，<u>因为SPDK的vhost-scsi/blk可以用来加速QEMU中半虚拟化的virtio-scsi/blk。另外针对QEMU中NVMe的虚拟化方案，我们也给出了vhost-NVMe的加速方案</u>。虽然SPDK  vhost-scsi/blk主要是用来加速virtio协议的，SPDK  vhost-NVMe用于加速虚拟机中的NVMe协议的，但是这3种加速方案其实可以有机地整合为一个整体的vhost target加速方案。

* 1.virtio
  virtio是I/O虚拟化中一种非常优秀的半虚拟化方案，需要在Guest的操作系统中运行virtio设备的驱动程序，通过virtio设备和后端的Hypervisor或用于加速的vhost进行交互。
  <u>在QEMU中，virtio设备是QEMU为Guest操作系统模拟的PCI设备</u>，这个设备可以是传统的PCI设备或PCIe设备，遵循PCI-SIG定义的PCI规范，可以具有配置空间、中断配置等功能。

  virtio在QEMU中的总体实现可以分成3层（见图4-34）：前端是设备层，位于Guest操作系统内部；中间是虚拟队列传输层，Guest和QEMU都包含该层，数据传输及命令下发完成都是通过该层实现的；第3层是virtio后端设备，用于具体落实来自Guest端发送的请求。

* 2.vhost加速
  如前所述，virtio后端设备用于具体响应Guest的命令请求。例如，对virtio-scsi设备来讲，该virtio后端负责SCSI命令的响应，<u>QEMU负责模拟该PCI设备，把该SCSI命令响应的模块在QEMU进程之外实现的方案称为vhost。</u>这里同样分为两种实现方式，在Linux内核中实现的叫作vhost-kernel，而在用户态实现的叫作vhost-user

  以virtio-scsi为例，目前主要有3种virtio-scsi后端的解决方案。

	1）QEMU virtio-scsi:   
	这个方案存在如下两个严重影响性能的因素。
	· 当Guest提交新的SCSI请求到virtqueue队列时，需要告知QEMU哪个队列含有最新的SCSI命令。
	· 在实际处理具体的SCSI读/写命令时（在hostOS  中），存在用户态到内核态的数据副本。

	​	2）Kernel vhost-scsi:  
	​	这个方案是QEMU  virtio-scsi的后续演进，基于LIO在内核空间实现为虚拟机服务的SCSI设备。实际上vhost-kernel方案并没有完全模拟一个PCI设备，QEMU仍然负责对该PCI设备的模拟，只是把来自virtqueue的数据处理逻辑拿到	内核空间了。

	​	3）SPDK vhost-user-scsi:  
	​	这个方案是基于Kernel vhost-scsi的进一步改进。SPDK vhost-user-scsi方案消除了这两方面的影响，后端的I/O处理线程在轮询所有的virtqueue，因此不需要Guest在添加新的请求到virtqueue后更新PCI的配置空间。SPDK vhost-user-scsi的后端I/O处理模块轮询机制加上零拷贝技术基本解决了前面我们提到的阻碍QEMU virtio-scsi性能提升的两个关键点。

* 3.SPDK vhost-scsi加速

使用SPDK vhost-scsi启动一个VM实例的命令如下：

```
-object memory-backend-file,id=mem0,size=4G,mem-path=/dev/hugepages,share=on -chardev socket,id=char0,path=/path/vhost.0
```

这里其实引入了vhost-user技术里面的两个关键技术实现：<u>指定mem-path意味着QEMU会在Guest  OS的内存中创建一个文件，share=on选项允许其他进程访问这个文件，也就意味着能访问Guest  OS内存，达到共享内存的目的。</u>字符设备/path/vhost.0是指定的socket文件，用来建立QEMU和后端的Slave target，即SPDK vhost target之间的通信连接。

QEMU Guest和SPDK vhost target是两个独立的进程，vhost-user方案一个核心的实现就是队列在Guest和SPDK  vhost  target之间是共享的，那么接下来我们就看一下vhost是如何实现这个内存共享的，以及Guest物理地址到主机的虚拟地址是如何转换的。

在vhost-kernel方案中，QEMU使用ioctl系统调用和内核的vhost-scsi模块建立联系，从而把QEMU中模拟的SCSI设备部分传递到了内核态，即内核态对该SCSI设备不是完全模拟的，仅仅负责对virtqueue进行处理，因此这个ioctl的消息主要负责3部分的内容传递：<u>Guest内存映射；Guest  Kick  Event、vhost-kernel驱动用来接收Guest的消息</u>，当接收到该消息后即可启动工作线程；IRQ  Event用于通知Guest的I/O完成情况。<u>同样地，当把内核对virtqueue处理的这个模块迁移到用户态时，以上3个主要部分的内容传递就变成了UNIX  Domain  socket文件了，消息格式及内容和Kernel的ioctl相比有许多相似和重复的地方。</u>

* 4 .SPDK vhost-NVMe加速

我们首先看一下virtio和NVMe协议的一个对比情况，virtio和NVMe协议在设计时都采用了相同的环型结构，virtio使用avaiable和used ring作为请求和响应，而NVMe使用提交队列和完成队列作为请求和响应。

图4-37 NVMe读/写的具体流程

QEMU中很早就添加了对NVMe设备的模拟，和QEMU  virtio-scsi类似，使用任意的文件来实现具体的NVMe  I/O命令，和之前的QEMUvirtio-scsi方案相比，QEMU  NVMe存在相同的性能瓶颈，在图4-37的步骤2和步骤8，Guest都要写NVMe  PCI配置空间寄存器，<u>因此会存在VMM  Trap自陷问题，由于后端主机使用文件来承载I/O命令，同样存在用户态到内核态数据副本的问题。如果要提升性能，那么同样需要解决这两个关键瓶颈。</u>

针对Guest提交命令和完成命令时的写PCI寄存器问题，NVMe 1.3的协议给出了解决方案，即shadow doorbell。

针对上面提到的另外一个性能瓶颈——内存副本，这里采用和vhost-user-scsi类似的方案。针对虚拟化场景，由于我们的后端存在高性能的物理NVMe控制器及SPDK本身的用户态NVMe驱动，因此对VM中下发的I/O命令，我们通过内存地址转换（Guest物理地址到主机虚拟地址）即可实现VM到NVMe设备端到端的数据零拷贝实现。

实现这个方案存在一个前提，由于物理的NVMe设备需要使用控制器内部的DMA引擎搬移数据，要求所有的I/O命令对应的数据区域都是物理内存连续的，因此这里我们需要使用Linux内核提供的hugetlbfs机制提供连续的物理内存页面。


#### 4.4.5 SPDK iSCSI Target
SPDK iSCSI Target从2013年开始被开发，最初的框架基于Linux SCSI TGT，但是随着整个项目的进展，为了更好地发挥快速存储设备 的性能，进而基于SPDK应用框架进行实现，以AIO、无锁化I/O数据路 径等为设计原则，和原来的Linux SCSI TGT有很大的区别。  
<u>SPDK iSCSI Target的设计和实现利用了SPDK库的以下模块:应用框架、网络、iSCSI、SCSI、JSON-RPC、块设备和SPDK的设备驱动程序。对于iSCSI Target而言，它使用应用框架启动，并解析相关配置文件以初始化，也能接收和处理JSON-RPC请求，然后构建不同的子系 统，如iSCSI、SCSI、块设备等子系统。对于I/O的处理，在网络接收到iSCSI的PDU包后，依次在iSCSI、SCSI、块设备层处理请求，最后由设备驱动程序处理。当I/O返回时，iSCSI Target程序将以相反的顺序处理，即块设备、SCSI、iSCSI、网络层。我们采用运行直到完成的模型，从而达到采用无锁化和异步处理I/O的方式的目的。</u>

##### 1.SPDK iSCSI Target加速设计和实现  
与其他常见的iSCSI Target实现(LIO、Linux SCSI TGT)相比，SPDK iSCSI Target使用以下几种方法来提高CPU单核的性能。  
* 1)模块化设计  
针对不同的功能模块，SPDK创建了多个子系统目录。对于SPDK iSCSI Target，SPDK创建了iSCSI模块，路径为 spdk/lib/event/subsystem/iscsi和spdk/lib/iscsi，该模块定义了所有和iSCSI相关的函数和数据结构。在SPDK iSCSI Target运行之前，iSCSI子系统 先会被初始化。  

* 2)每个CPU核处理一组iSCSI的连接  
根据SPDK应用框架，每个CPU上启动一个Reactor不断地去执行两 组Poller，一组基于timer的Poller的列表和一组普通Poller列表。为此 SPDK的iSCSI Target在每个core的Reactor上都创建了一个polling group， 用于处理这个组里面的所有iSCSI连接。对应于每个polling group，会有 两组Poller，它们分别执行spdk_iscsi_poll_group_poll和 spdk_iscsi_poll_group_handle_nop。  

* 3)基于简单的负载平衡算法  
当iSCSI Target使用多个CPU核启动的时候，根据SPDK的应用程序 框架，会有多个Reactor，每个Reactor上都会有Poller。因为监听网络事 件的acceptor默认运行在一个Reactor的Poller上，所以每个新进入的 iSCSI连接都会在acceptor所在的Reactor上运行。如此一来，就会导致所 有的CPU core处理的iSCSI连接不均衡。  

为此我们设计了一个算法。因为iSCSI的连接有状态的变化，所以 当连接从login状态转化为FFPlogin状态FFP(Full Feature Phase)的时候，我们会对iSCSI连接进行迁移，也就是从一个Reactor上执行转入另一个Reactor。没有进入FFP的iSCSI连接不用进行迁移，因为这些iSCSI 连接很快会断掉，而且不涉及对后端I/O数据的处理，为此不需要进行迁移。我们会设计一个简单的算法来计算每个Reactor上的iSCSI connection连接数目，然后根据对应的连接的会话等信息，选择一个新的Reactor。迁移的过程相对来讲还是比较复杂的，我们首先会将这个iSCSI连接从当前的polling group中去除(包括有关网络事件的监听)， 然后加入另外Reactor的polling group中(通过SPDK应用框架提供的线程间通信机制)。  

* 4)零拷贝支持  
对于iSCSI读取命令，我们利用零拷贝方法，这意味着缓冲区在 SPDK Bdev层中进行分配，并且在将iSCSI datain响应pdus发送到iSCSI 启动器后，此缓冲区将被释放。在所有iSCSI读取处理过程中，不存在从存储模块到网络模块的数据复制。

* 5)iSCSI数据包处理优化  
SPDK对读和写的数据包处理都有64KB的限制。当处理读请求大于64KB的时候，SPDK就会创建DATAIN任务队列，同时会设置DATAIN 任务数的最大值为64KB。SPDK创建的每个DATAIN任务大小都是64KB。针对写命令，SPDK定义了MaxBustLength为64KB乘以connection的DATAOUT缓冲数。所以在发送R2T时，在R2T中设置的可以接收的数据大小为MaxBustLength和剩余待传输数据中的最小值，以保证对方发过来的数据包符合协议的需求。

* 6)TCP/IP协议栈优化  
SPDK库对TCP/IP的网络处理进行了相应的API封装，这样就可以整 合不同的TCP/IP协议栈。目前SPDK库既可以使用内核的TCP/IP协议栈，也可以使用用户态的TCP/IP协议栈进行矢量包处理(V ector Packet Processing，VPP)。    
VPP是思科VPP技术的开源版本，一个高性能包处理栈，完全运行 于用户态。作为一个可扩展的平台框架，VPP能够提供随时可用的产品 级的交换机或路由器功能。    
SPDK主要使用了VPP的socket处理，包括socket的创建、监听、连 接、接收和关闭。SPDK也会调用VPP的epoll API来创建socket group。    
在配置SPDK的时候指定VPP的目录路径，就可以使用VPP。<u>所以对SPDK的iSCSI Target来讲，**网络的优化可以选择VPP提供的用户态TCP/IP协议栈，然后使用DPDK提供的PMD网卡**，就可以实现从网络到后端数据处理的完全零拷贝解决方案。</u>  


##### 2.在Linux环境下配置SPDK iSCSI Target示例  

#### 4.4.6 SPDK NVMe-oF Target

NVMe协议制定了本机高速访问PCIe SSD的规范，相对于SATA、 SAS、AHCI等协议，NVMe协议在带宽、延迟、IOps等方面占据了极大 的优势，但是在价格上目前相对来讲还是比较贵的。不过不可否认的是，配置PCIe SSD的服务器已经在各种应用场景中出现，并成为业界的一种趋势。  

<u>此外为了把本地高速访问的优势暴露给远端应用，诞生了NVMe-oF协议。NVMe-oF Target是NVMe协议在不同传输网络(transport)上面的延伸。</u> NVMe-oF协议中的transport可以多种多样，如以太网、光纤通道、Infiniband等。<u>当前比较流行的transport实现是基于RDMA的Ethernet transport、Linux Kernel和SPDK的NVMe-oF Target等</u>，另外对于光纤通道的transport，NetApp基于SPDK NVMe-oF Target的代码，实现了基于光纤通道的transport。  

NVMe-oF Target严格来讲不是必需品，在没有该软件的时候，我们可以使用iSCSI Target或其他解决方案来替换。由于iSCSI Target比较成 熟和流行，我们有必要把NVMe-oF Target与iSCSI Target进行对比，如表4-2所示。

从表4-2中我们可以获得如下信息。
· 目前NVMe-oF Target在以太网上的实现，需要有支持RDMA功 能的网卡，如支持RoCE或iWARP。相比较而言，iSCSI Target更加通用，有没有RDMA功能支持关系不是太大。  

· 标准的NVMe-oF Target主要是为了导出PCIe SSD(并不是说不能导出其他块设备)，iSCSI Target则可以导出任意的块设备。从这一方面来讲，iSCSI Target的设计目的无疑更加通用。  

· NVMe-oF Target是NVMe协议在网络上的扩展，毫无疑问的是如果访问远端的NVMe盘，使用NVMe-oF协议更加轻量级，直接是NVMe-oF→NVMe协议到盘，相反如果使用iSCSI Target，则需要iSCSI→SCSI→NVMe协议到盘。显然在搭载了RNIC + PCIe SSD的情况下，NVMe-oF能发挥更大的优势。

总体而言iSCSI Target更加通用，NVMe-oF Target的设计初衷是考虑性能问题。当然在兼容性和通用性方面，NVMe-oF Target也在持续进步。  

· 兼容已有的网卡: NVMe-oF新的规范中已经加入了基于TCP/IP的支持，这样NVMe-oF就可以运行在没有RDMA支持的网卡上了。已有的网卡就可以兼容支持iSCSI及NVMe-oF协议，意味着当用户从iSCSI迁移到NVMe-oF上时，可以继续使用旧设备。当然从性能方面来讲，必然没有RDMA网卡支持有优势。  

· 后端存储虚拟化: NVMe-oF协议一样可以导出非PCIe SSD，使得整个方案兼容。比如SPDK的NVMe-oF Target提供了后端存储的简单抽象，可以虚拟出相应的NVMe盘。在SPDK中可以用malloc的块设备或基于libaio的块设备来模拟出NVMe盘，把NVMe协议导入SPDK通用块设备的语义中。当然远端看到的依然是NVMe盘，这只是协议上的兼 容，性能上自然不能和真实的相匹配，但是这解决了通用性的问题。

如此NVMe-oF协议可以做到与iSCSI一样的通用性。当然在长时间内，NVMe-oF和iSCSI还是长期并存的局面。iSCSI目前已经非常成熟，而NVMe-oF则刚刚开始发展，需要不断地完善，并且借鉴iSCSI协议的一些功能，以支持更多的功能。

SPDK在2016年7月发布了第一款NVMe-oF Target的代码，遵循了NVMe over fabrics相关的规范。SPDK的NVMe-oF Target实现要早于Linux Kernel NVMe-oF Target的正式发布。当然在新Linux发行版都自带 NVMe-oF Target的时候，大家就会有一个疑问，我们为什么要使用SPDK的NVMe-oF Target。
SPDK的NVMe-oF Target和内核相比，在单核的性能 (Performance/per CPU core)上有绝对的优势。  

· SPDK的NVMe-oF Target可以直接使用SPDK NVMe用户态驱动封装的块设备，相对于内核所使用的NVMe驱动更具有优势。  
· SPDK NVMe-oF Target完全使用了SPDK提供的编程框架，在所有I/O的路径上都采用了无锁的机制，为此极大地提高了性能。  
· 对RDMA Ethernet transport的高效利用。  

SPDK NVMe-oF Target的主程序位于spdk/app/nvmf_tgt。因为 NVMe-oF和iSCSI一样都有相应的subsystem(代码位于 spdk/lib/event/subsystems/nvmf)，只有在配置文件或RPC接口中调用了相应的函数，才会触发相应的初始化工作。<u>这部分代码最重要的函数是nvmf_tgt_advance_state，主要通过状态机的形式来初始化和运行整个NVMe-oF Target系统。另外一部分代码位于spdk/lib/nvmf，主要是处理来自远端的NVMe-oF请求，包括transport层的抽象，以及实际基于RDMA transport的实现。如果读者希望学习SPDK NVMe-oF Target的细节，可以从spdk/lib/event/subsystems/nvmf目录的nvmf_tgt.c中的 spdk_nvmf_subsystem_init函数入手。</u>


#### 4.4.7 SPDK RPC
<u>SPDK软件库实现了一个基于JSON-RPC 2.0的服务，以允许外部的管理工具动态地配置基于SPDK软件的服务器应用，或使用监控工具动态地获取SPDK应用的运行状态。</u>目前，JSON-RPC是SPDK软件库中最主要的监控管理工具，SPDK软件库中包含的各个组件均有一些相应的RPC方法供用户调用。未来SPDK软件库会增添更多的RPC方法，来提高管理SPDK各个子系统或模块的灵活性，并减少对静态配置的依赖。  

##### 1.RPC
当把整个应用程序散布在互相通信的多个进程中时，一种方式是各个进程可以进行显式的网络编程，通过socket或进程间通信的API来编写交互过程。另一种方式则是使用隐式网络编程，即使用过程调用来完成，这样调用过程中涉及的网络I/O处理或进程间通信的细节，对于开发者而言基本上是透明的，可以省去部分工作量，并提高了开发软件应用的速度。   

RPC是指一个应用程序调用不在自己地址空间中的子程序的过程。 RPC是一种Client-Server的交互方式，调用发起者为客户端，子程序执行者为Server端，通常通过消息机制完成请求与响应的传递。RPC调用两端的进程既可以处于不同的主机上通过网络来传递消息，也可以在同一主机上通过进程间通信来传递消息

为了提供多样的Server端应用，基于不同服务的实现细节差异，各个RPC系统互不兼容，比如网络文件系统的RPC接口与SUN RPC互不兼容。<u>同时，为了允许不同的客户端访问Server端的应用，实现跨平台服务，目前已有许多标准化的RPC系统出现，比如JSON-RPC协议既可以为Java应用服务，也可以为C、Python等编程语言的应用服务。</u>

##### 2.JSON
JSON(JavaScript Object Notation)是一种轻量级的数据交换格式。既易于人阅读和编写，也易于机器解析和生成。<u>JSON包括4个基本类型String、Numbers、Booleans和Null，以及两个结构化类型Objects和Arrays的数据。</u>  

典型的JSON语法规则如下。   
· JSON名/值对:JSON数据的书写格式是“名/值”对。如，"firstName" : "John"。   
· JSON对象:JSON对象在花括号中书写，对象可以包含多个名/值对。

```json
  { "firstName":"John","lastName":"Doe" }。
```

· JSON数组:JSON数组在方括号中书写，数组可以包含多个对象。在下面的例子中，对象 "employees" 是包含2个对象的数组，每个对象代表一条关于某个人(包括姓和名)的记录。  
```json
{
	"employees":[
		{"firstName":"John", "lastName":"Doe"},
		{"firstName":"Anna", "lastName":"Smith"}
	]
}
```

##### 3.JSON-RPC
JSON-RPC是RPC的一种规范，一个无状态且轻量级的协议，实现及使用简单。JSON-RPC规范主要定义了一些数据结构及其相关的处理规则，它被允许运行在基于socket、HTTP等许多不同消息传输环境的进程中，并使用JSON作为数据格式。  
##### 4.SPDK JSON-RPC
<u>使用SPDK库中的RPC需要首先在SPDK的应用启动时使用“-r”参数 指定RPC Server的监听地址，
默认地址为“/var/tmp/spdk.sock”。使用SPDK提供的客户端命令行工具“scripts/rpc.py”，
可以方便地向SPDK Server端发起RPC调用。</u> 使用方法如下:

```
$ rpc.py [-h] [-s SERVER_ADDR] [-p PORT] [-v] <command> [parameters list]
```
其中，“-s”“-p”参数分别指定SPDK RPC Server端的监听地址和端口，“command”“parameters list”分别指定具体的RPC命令和对应的参数。

获取command命令所需要的对应参数可以通过如下命令:
```
$ rpc.py <command> -h
```

##### 5.SPDK JSON-RPC运行机制  
在启动SPDK的编程框架时，SPDK将会初始化RPC所需的功能。一 个RPC专用的socket文件会被创建在相应的路径上，然后SPDK会绑定并监听它。接下来，SPDK会在Master Core的Reactor上为RPC注册一个Poller，此后RPC所有的功能都会在这个Poller里执行，也就是说，所有SPDK的RPC Server端服务是执行在Master Core上的。    

当一个SPDK RPC客户端发出RPC调用请求后，RPC Poller在轮询过程中，接受该连接，接下来接收该连接上客户端发出的请求内容，并解析为JSON请求。在SPDK中已经注册的所有RPC方法中通过逐个对比后，找出对应的方法并执行进入。在RPC的方法中，首先将JSON请求解析成函数执行过程中需要的参数，完成相应的功能。在完成功能后， 根据需要填写JSON响应，并将它加入发送队列。RPC Poller在轮询过程中，将该JSON响应发送给SPDK RPC客户端。

SPDK中JSON-RPC所依赖的代码主要分布在以下4个部分。     
· lib/jsonrpc:接收发送网络数据，解析和流化JSON请求。   
· lib/rpc:将JSON-RPC适配到SPDK编程框架中。  
· lib/json:具体的数据流与JSON结构的解析与流化方法。  
· <module>_rpc.c:各个组件中以rpc结尾的C文件，如 bdev_malloc_rpc.c，定义并注册具体的SPDK RPC方法。  


#### 4.4.8 SPDK生态工具介绍