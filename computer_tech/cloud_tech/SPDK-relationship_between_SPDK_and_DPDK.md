## Assigning CPU Cores to the NVMe over Fabrics Target  
SPDK uses the DPDK Environment Abstraction Layer to gain access to hardware resources such as huge memory pages and CPU core(s). DPDK EAL provides functions to assign threads to specific cores. To ensure the SPDK NVMe-oF target has the best performance, configure the NICs and NVMe devices to be located on the same NUMA node.

SPDK的NVMe-Of主要使用DPDK中EAL(DPDK Environment Abstraction Layer)的：  
1、线程亲核性；  
2、大页内存；  
3、设置NICs和NVMe驱动运行在同一个NUMA上；
4、...

## SPDK 助力加速 NVMe 硬盘
构建 SPDK 之前，必须首先安装数据平面开发套件 (DPDK)，因为 SPDK 依赖 DPDK 中的内存管理和队列功能。 DPDK 是比较成熟的库，常用于网络数据包处理，而且经过高度优化，能够以较低的延迟管理内存和队列数据。

以下命令可设置 hugepage 并解除内核驱动程序对 NVMe 和 I/OAT 设备的绑定：
sudo scripts/setup.sh
使用 hugepage 对性能至关重要，因为相比于默认的 4KiB 页面大小，它们的大小只有 2MiB，并能降低转换后备缓冲区 (TLB) 缺失的几率。 TLB 是 CPU 的一个组件，负责将虚拟地址转换成物理内存地址，因此使用较大的页面 (hugepage) 有助于高效使用 TLB。

[SPDK 助力加速 NVMe 硬盘](https://software.intel.com/content/www/cn/zh/develop/articles/accelerating-your-nvme-drives-with-spdk.html)