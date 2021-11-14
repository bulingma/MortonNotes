物理机器是由CPU，内存和I/O设备等一组资源构成的实体。  
虚拟机也一样，由虚拟CPU，虚拟内存和虚拟I/O设备等组成。  
VMM(VM Monitor)按照与传统OS并发执行用户进程的相似方式，仲裁对所有共享资源的访问。  

在虚拟化的平台上，虚拟机（guest VM）所使用的多个虚拟CPU（以下称vCPU）可能是共享同一个物理CPU（以下称pCPU）的。  
VMM负责vCPU的调度，当一个vCPU被调度到获得pCPU的使用权后，基于该vCPU运行的guest OS又可以调度OS中的各个线程/进程了。  
也就是说，guest OS中的各个线程/进程分时复用了vCPU，而各个vCPU又分时复用了pCPU。

为了从硬件上提供对vCPU调度和切换的支持，Intel推出了被称为VT-x(Virtualization Technology for x86)的CPU虚拟化扩展技术，  
用户可通过VMXON/VMXOFF指令打开/关闭这个功能。和Intel亦敌亦友的AMD也推出了被称为AMD-V的对应技术。

在Linux中，从用户空间trap到内核空间可以通过system call或者interrupt/exception。
以system call基于x86的实现为例，早期x86提供的trap方法是int 0x80这样的software interrupt机制，
而后改成了SYSENTER/SYSEXIT的指令对，现在则已经被速度更快的SYSCALL/SYSRET取代了。
类似地，在VT-x中，从guest VM进入VMM（这个过程被称为VM exit）通常有三种方式：
1、执行VMCALL指令，这种方式被称为hyper call，跟执行SYSCALL指令实现的system call原理差不多。
2、发生了硬件中断或软件异常。
3、guest VM执行了一些敏感指令。有一些敏感指令并不会产生VM exit，比如SYSENTER；
有一些敏感指令则可以根据下面将要介绍的VM executation control fields配置来选择是否产生VM exit。
进入VMM就意味着从non-root mode进入了root mode（参考这篇文章），反之，从VMM返回guest VM（这个过程被称为VM entry）
则是重新回到了non-root mode，mode的切换意味着上下文（context）的保存和恢复。

**上下文**其实是个难以定义的概念，它是从CPU的角度引出的，
简单地说，上下文就是程序（进程/中断）运行时所需要的寄存器的最小集合，这些寄存器的后面可能代表着程序运行的一类资源。
**上下文切换**是指程序从一种状态切换到另一种状态（比如从用户态切换到内核态），
或者从一个程序切换到另一个程序（比如进程切换）时，导致上下文相关寄存器的值变化的行为。
对于上下文切换时不需要改变的寄存器，也可以说它不是该程序的上下文。

* VMCS
在Linux中，一个进程的相关信息保存在task_struct中。
虚拟机的上下文比进程的上下文更为复杂，
在VT-x中，由VMCS(Virtual-Machine Control data Structures)负责保存vCPU需要的相关状态和上下文信息。
VMCS在使用时需要和pCPU绑定。一个pCPU可以对应多个vCPU，而一个vCPU对应一个VMCS，但在任意给定时刻，
一个pCPU上只能运行一个vCPU（就像在多线程调度中，某一时刻，一个CPU上只能运行一个线程一样）。