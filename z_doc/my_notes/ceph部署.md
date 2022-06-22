
<!-- TOC -->

- [ceph介绍(INTRO TO CEPH)](#ceph介绍intro-to-ceph)
    - [**Monitors**](#monitors)
    - [**Managers**](#managers)
    - [**Ceph OSDs(最复杂和重要的模块)**](#ceph-osds最复杂和重要的模块)
    - [**MDSs**](#mdss)
- [ceph部署（INSTALLING CEPH）](#ceph部署installing-ceph)
  - [推荐的两种方式（RECOMMENDED METHODS）](#推荐的两种方式recommended-methods)
    - [Cephadm](#cephadm)
    - [Rook](#rook)
  - [其他方式（OTHER METHODS）](#其他方式other-methods)

<!-- /TOC -->


## ceph介绍(INTRO TO CEPH)

**高屋建瓴地总结了Ceph的特点：**

<u>Ceph采用对象存储存储数据。Ceph使用CRUSH算法，用户的数据（Object）被计算属于那个PG(placement group)，PG被计算属于那个OSD。CRUSH算法使得Ceph集群具备扩容、重平衡和动态恢复的能力。</u>

<u>**Ceph stores data as objects within logical storage pools. Using the [CRUSH](https://docs.ceph.com/en/quincy/glossary/#term-CRUSH) algorithm, Ceph calculates which placement group (PG) should contain the object, and which OSD should store the placement group. The CRUSH algorithm enables the Ceph Storage Cluster to scale, rebalance, and recover dynamically.**</u>



1. 想要部署Ceph集群必须了解[Ceph Node](https://docs.ceph.com/en/quincy/glossary/#term-Ceph-Node)这个概念；

2. Ceph集群必须的四个组件：

   one Ceph Monitor, Ceph Manager, and Ceph OSD (Object Storage Daemon). The Ceph Metadata Server（required when running Ceph File System clients）

Whether you want to provide Ceph Object Storage and/or Ceph Block Device services to Cloud Platforms, deploy a Ceph File System or use Ceph for another purpose, <u>all Ceph Storage Cluster  deployments begin with setting up each [Ceph Node](https://docs.ceph.com/en/quincy/glossary/#term-Ceph-Node)</u>, your network, and the Ceph Storage Cluster. <u>A Ceph Storage Cluster requires at least **one Ceph Monitor, Ceph Manager, and Ceph OSD (Object Storage Daemon)**. The Ceph Metadata Server is also required when running Ceph File System clients.</u>



#### **Monitors**

Monitors是Ceph maps(Ceph各种服务拓扑结构)的管理者，至少需要3个Monitors（高可用&冗余）。

<u>A [Ceph Monitor](https://docs.ceph.com/en/quincy/glossary/#term-Ceph-Monitor) (`ceph-mon`) maintains maps of the cluster state, including the monitor map, manager map, the OSD map, the MDS map, and the CRUSH map</u>.  These maps are critical cluster state required for Ceph daemons to coordinate with each other. Monitors are also responsible for managing authentication between daemons and clients. <u>At least three monitors are normally required</u> for redundancy and high availability.



#### **Managers**

Managers是Ceph集群运行信息的管理者，包括存储利用率，性能指标，系统负载。同时manager还提供基于Python的集群信息展示功能，包括基于Web的DashBoard和Rest Api。部署时至少需要2个managers（高可用）

<u>A [Ceph Manager](https://docs.ceph.com/en/quincy/glossary/#term-Ceph-Manager) daemon (`ceph-mgr`) is responsible for keeping track of runtime metrics and the current state of the Ceph cluster, including storage utilization, current performance metrics, and system load. The Ceph Manager daemons also host python-based modules to manage and expose Ceph cluster information, including a web-based [Ceph Dashboard](https://docs.ceph.com/en/quincy/mgr/dashboard/#mgr-dashboard) and [REST API](https://docs.ceph.com/en/quincy/mgr/restful). At least two managers are normally required for high availability.</u>



#### **Ceph OSDs(最复杂和重要的模块)**

一个对象存储服务，用来存储数据，进行数据复制、数据恢复、重平衡等。至少需要3个OSDs（可用性和冗余）

<u>An Object Storage Daemon ([Ceph OSD](https://docs.ceph.com/en/quincy/glossary/#term-Ceph-OSD), `ceph-osd`) stores data, handles data replication, recovery, rebalancing, and provides some monitoring information to Ceph Monitors and Managers by checking other Ceph OSD Daemons for a heartbeat. At least three Ceph OSDs are normally required for redundancy and high availability.</u>



#### **MDSs**

MDSs用来实现Ceph文件存储，存储metadata，支持POSIX文件系统命令。

A [Ceph Metadata Server](https://docs.ceph.com/en/quincy/glossary/#term-Ceph-Metadata-Server) (MDS, `ceph-mds`) stores metadata on behalf of the [Ceph File System](https://docs.ceph.com/en/quincy/glossary/#term-Ceph-File-System) (i.e., Ceph Block Devices and Ceph Object Storage do not use MDS). Ceph Metadata Servers allow POSIX file system users to execute basic commands (like `ls`, `find`, etc.) without placing an enormous burden on the Ceph Storage Cluster.





## ceph部署（INSTALLING CEPH）

There are several different ways to install Ceph. Choose the method that best suits your needs.

### 推荐的两种方式（RECOMMENDED METHODS）

#### Cephadm

[Cephadm](https://docs.ceph.com/en/quincy/cephadm/#cephadm) installs and manages a Ceph cluster using <u>containers and systemd</u>, with tight integration with the CLI and dashboard GUI.

- cephadm only supports Octopus and newer releases.
- cephadm is fully integrated with the new orchestration API and fully supports the new CLI and dashboard features to manage cluster deployment.
- <u>cephadm requires container support (podman or docker) and Python 3.</u>

#### Rook

<u>We recommend Rook as the way to run Ceph in Kubernetes or to connect an existing Ceph storage cluster to Kubernetes.</u>



### 其他方式（OTHER METHODS）

* ceph-ansible

  [ceph-ansible](https://docs.ceph.com/ceph-ansible/) deploys and manages Ceph clusters using Ansible.

* ceph-deploy— is no longer actively maintained

  [ceph-deploy](https://docs.ceph.com/projects/ceph-deploy/en/latest/) is a tool for quickly deploying clusters.

* ceph-salt

  [ceph-salt](https://github.com/ceph/ceph-salt) installs Ceph using Salt and cephadm.

* jaas.ai/ceph-mon

  [jaas.ai/ceph-mon](https://jaas.ai/ceph-mon) installs Ceph using Juju.

* puppet-ceph

  [github.com/openstack/puppet-ceph](https://github.com/openstack/puppet-ceph) installs Ceph via Puppet.

* Windows安装

  For Windows installations, please consult this document: [Windows installation guide](https://docs.ceph.com/en/quincy/install/windows-install).