## 虚拟化

### 虚拟化相关名词
* 虚拟化  
虚拟化的含义很广泛，将任何一种形式的资源抽象成另一种形式的技术都是虚拟化技术。如：
进程级别的虚拟地址空间：就是把物理内存虚拟成多个内存空间；    
机器级别的虚拟化：虚拟机所抽象的是整个物理机，包括CPU、内存和I/O设备等；  

* VMM(Virtual Machine Monitor)/guest VM
提供虚拟化的平台被称为VMM，在其上运行的虚拟机被称为guest VM（客户机）。  
根据VMM支持的虚拟机制的不同，guset VM的运行模式可分为完全虚拟化（Full Virtualization）和类虚拟化（Para Virtualization）。

* 完全虚拟化
如果一个基于硬件运行（native）的OS不需要修改任何代码就可以直接跑在VMM上，也就是guest OS根本感知不到自己运行在一个虚拟化环境中（可以说VMM是骗术高明的），
这种就被称为“完全虚拟化”。在这种模式下，VMM需要正确处理guest所有可能的指令。
最简单直接的方法就是，VMM对guest运行过程中的每一条指令都进行解释和执行，模拟出这条指令执行的效果，这种方法既适用于和VMM相同体系结构的guest，也能用于模拟和VMM不同体系结构的guest（比如物理CPU是x86的，而guest是基于ARM的），其缺点也很明显，就是性能太差。

* 类虚拟化
像x86这种CISC架构的系统，指令繁杂，其中一些指令是难以虚拟化的，如果使用完全虚拟化，就需要通过二进制代码翻译（binary translation），扫描并修改guest的二进制代码，将难以虚拟化的指令转换成支持虚拟化的指令（ABI级），就像打补丁一样。
如果能直接修改guest的操作系统内核代码（API级），就可以使得内核避免产生这些难以虚拟化的指令，这就是“类虚拟化”。  
**类虚拟化技术需要修改直接基于native运行的OS代码，以便和VMM更好的配合，其好处就是guest的运行性能可以接近物理机的性能。**  

### 三种主流的虚拟化技术的实现架构  
1. Hypervisor模型   
<u>**在hypervisor模型中，VMM是一个完备的操作系统，它除了具备传统操作系统的功能，还具备虚拟化功能。**</u>  
包括CPU、内存和I/O设备在内的所有物理资源都归VMM所有，因此VMM不仅要负责虚拟机环境的创建和管理，还承担着管理物理资源的责任。
这种方式是比较高效的，然而I/O设备种类繁多，管理所有设备就意味着大量的驱动开发工作。  
在实际的产品中，厂商会根据产品定位，有选择的支持一些I/O设备，而不是对所有的I/O设备都提供支持。  

2. Host模型（宿主机）  
在host模型中，物理资源由host OS管理，host OS是传统操作系统（比如Linux），这些传统操作系统并不是为虚拟化而设计的，因此本身并不具备虚拟化功能，实际的虚拟化功能由VMM来提供。
<u>**VMM作为host OS中一个独立的内核模块，通过调用host OS的服务来获得资源，实现CPU、内存和I/O设备的虚拟化。**</u>  
<u>**VMM创建出虚拟机之后，通常将虚拟机作为host OS的一个进程参与调度。**</u>   
Host模型最大的优点就是可以充分利用现有操作系统的设备驱动程序，VMM不需要为各种I/O设备重新实现驱动，可以专注于物理资源的虚拟化；  
**缺点在于，由于VMM是借助host OS的服务来操作硬件，而不是直接操作硬件，因此受限于host OS服务的支持，可能导致硬件利用的不充分。**  
从架构上看，由Qumranet公司开发的KVM（Kernel-based Virtual Machine）就是属于host模型的，kernel-based，顾名思义就是基于操作系统内核。
KVM于2007年被集成到Linux内核2.6.20版本，并于2008年被Red Hat收购。
随着越来越多的虚拟化功能被加入到Linux内核当中，Linux已经越来越像一个hypervisor了，从这个角度看，KVM也可以算是hypervisor模型了。

3. 混合模型
在混合模型中，VMM依然位于最底层，拥有所有的物理资源，
<u>**但为了利用现有操作系统的I/O设备驱动程序，VMM会将大部分的I/O设备交由一个运行在特权级别的虚拟机操作系统（Service OS）来处理，自己则主要负责CPU管理和内存管理。**</u>
混合模型可以说是结合了上述两种模型的优点，既不需要另外开发I/O设备驱动程序，又可以通过直接控制CPU和内存实现对这些物理资源的充分利用，以提高效率。
**但它也是存在缺点的，当来自guest OS的I/O请求发送到VMM后，VMM需要将这些请求转发到service OS，这无疑增加了上下文的开销。**  
混合模型的代表有Xen，Intel最近推出的Acrn，以及我国工程师写的minos。


### 三种虚拟化平台（VMM）总结
1、操作系统任务：管cpu、管内存、管i/o设备  
2、物理资源虚拟化任务：虚拟机（核心数、内存大小等）创建、管理 
* Hypervisor：以上1、2都需要做  
* Host模型（典型的有KVM）：1由传统操作系统完成，2由VMM完成（VMM作为host OS中的一个内核模块存在，创建出来的虚拟机作为host OS的一个进程参与调度）    
* 混合模型（典型的有Xen,Acrn,minos）：1中的I/O设备交给运行在特权级别的虚拟机操作系统（Service OS）处理，1中的CPU、内存、2均由VMM完成