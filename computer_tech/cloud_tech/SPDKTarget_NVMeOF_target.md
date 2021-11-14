## What's NVMe over Fabrics
The NVMe over Fabrics specification defines subsystems that can be exported over different transports. 
(NVMe over Fabrics规范 定义了可以在不同的传输上导出（暴露）的子系统)
(NVMe over Fabrics是一个规范、协议)

## What's SPDK NVMe over Fabrics target ?
The SPDK NVMe over Fabrics target is a user space application that presents block devices over a fabrics such as Ethernet, Infiniband or Fibre Channel. SPDK currently supports RDMA and TCP transports. 
(SPDK NVMe over Fabrics target是一个用户空间应用程序，它在诸如以太网，InfiniBand或光纤通道之类的网络上呈现块设备)

## what's target ?
The NVMe over Fabrics specification defines subsystems that can be exported over different transports. SPDK has chosen to call the software that exports these subsystems a "target", which is the term used for iSCSI(这是用于iSCSI的术语).

## what's host/initiator ?
The specification refers to the "client" that connects to the target as a "host". Many people will also refer to the host as an "initiator", which is the equivalent thing in iSCSI parlance. SPDK will try to stick to the terms "target" and "host" to match the specification.

## Linux kernel implements NVMe-oF target and host
The Linux kernel also implements an NVMe-oF target and host, and SPDK is tested for interoperability with the Linux kernel implementations.

## Both the Linux kernel and SPDK implement an NVMe over Fabrics host.
当前我司的nvmf的host是采用的linux kernel提供的driver支持，因为spdk提供的用户态host driver打开block设备和chunk打开nvme本地盘的方法一样，公司里面除了我们其他人也不会用，难以维护，所以采用内核提供的host访问后端nvmf target。

[nvmf](https://spdk.io/doc/nvmf.html)