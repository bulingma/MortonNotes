

## 概念
* Host Channel Adapter (HCAs)    

Channel adapter refers an end node in the infiniband network. It is equivalent of Ethernet network interface card (NIC), but with more features regarding Infiniband and RDMA.(The most important feature is address translation mechanism)      

HCAs provide the point at which an IB end node (for example, a server) connects to an IB network. These are the equivalent of the Ethernet (NIC) card but they do much more. HCAs provide address translation mechanism under the control of the operating system which allows an application to access the HCA directly. The same address translation mechanism is the means by which an HCA accesses memory on behalf of a user level application. The application refers to virtual addresses while the HCA has the ability to translate these addresses into physical addresses in order to affect the actual message transfer. 

* Queue Pair (QP), a set of a Send Queue (SQ), a Receive Queue (RQ), and a Completion Queue (CQ)

HCAs communicate with each other using work queues. Three types of the queues are:   
(1) send queue (SQ)  
(2) receive queue (RQ)  
(3) a completion queue (CQ)  
**SQ and RQ are always grouped and managed as a queue pair (QP).**  
We can **post a work request (WR) by generating a work queue entry (WRE) into the work queue,** 
e.g.   
(1) posting a send work request into the SQ to send some data to a remote node;  
(2) posting a receive work request into the RQ to receive data from a remote node;  
etc.   
Posted work requests are directly handled by hardware (HCA). Once a request is completed, the hardware posts a Work Completion (WC) into a completion queue (CQ). 
**Programming Interface provides flexibility that we can specify distinct completion queues to the SQ and the RQ, or use one CQ for the entire QP.**    
In short, programming RDMA program is roughly easy: generate a QP and a CQ (and other required data structures for this operation), connect the QP to the remote node, and generate a work request (WR) and post it into the QP. Then the HCA transfers your orders to the connected counterpart.


[RDMA Aware Networks Programming User Manual](https://www.mellanox.com/related-docs/prod_software/RDMA_Aware_Programming_user_manual.pdf)
[Introduction to Programming Infiniband RDMA](https://insujang.github.io/2020-02-09/introduction-to-programming-infiniband/)