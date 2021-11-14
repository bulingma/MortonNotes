## Introduction
**<u>A vhost target provides a local storage service as a process running on a local machine. It is capable of exposing virtualized block devices to QEMU instances or other arbitrary（任意） processes.</u>**

## Supported Guest Operating Systems
The guest OS must contain virtio-scsi or virtio-blk drivers. Most Linux and FreeBSD distributions include virtio drivers. Windows virtio drivers must be installed separately. The SPDK vhost target has been tested with recent versions of Ubuntu, Fedora, and Windows

## Qemu和Vhost的关系
Userspace vhost-scsi target support was added to upstream QEMU in v2.10.0.
Userspace vhost-blk target support was added to upstream QEMU in v2.12.0. 

## SPDK Configuration
### 1、Create bdev (block device)
SPDK bdevs are block devices which will be exposed to the guest OS. 
For vhost-scsi, bdevs are exposed as SCSI LUNs on SCSI devices attached to the vhost-scsi controller in the guest OS. 
For vhost-blk, bdevs are exposed directly as block devices in the guest OS and are not associated at all with SCSI.

SPDK supports several different types of storage backends, including NVMe, Linux AIO, malloc ramdisk and Ceph RBD. 
Refer to Block Device User Guide for additional information on configuring SPDK storage backends.

### 2、Create a vhost device
#### 2.1、Vhost-SCSI
The following RPC will create a vhost-scsi controller which can be accessed by QEMU via /var/tmp/vhost.0. 

#### 2.2、Vhost-BLK
The following RPC will create a vhost-blk device exposing Malloc0 bdev. The device will be accessible to QEMU via /var/tmp/vhost.1

#### 2.3、Vhost-NVMe (experimental) can only be supported with latest Linux kernel
Vhost-NVMe target was designed for one new feature of NVMe 1.3 specification, Doorbell Buffer Config Admin command, which is used for emulated(模仿的) NVMe controller only. Linux 4.12 added this feature, so a new Guest kernel later than 4.12 is required to test this feature.

[vhost Target](https://spdk.io/doc/vhost.html)  
[vhost：一种 virtio 高性能的后端驱动实现](https://cloud.tencent.com/developer/article/1075600)
[Virtualized I/O with Vhost-user](https://spdk.io/doc/vhost_processing.html)
[Design of Vhost-pci](http://www.linux-kvm.org/images/5/55/02x07A-Wei_Wang-Design_of-Vhost-pci.pdf)  
[Virtio and Vhost Architecture - Part 1](https://insujang.github.io/2021-03-10/virtio-and-vhost-architecture-part-1/)