**RoCE技术可通过普通以太网交换机实现，但服务器需要支持RoCE网卡，网络侧需要支持无损以太网络，这是由于IB的丢包处理机制中，任意一个报文的丢失都会造成大量的重传，严重影响数据传输性能。**

## 无损以太网关键特性
在RoCE网络中，我们需要构建无损以太网用于保证网络传输过程中不丢包。构建无损以太网需支持以下关键特性：

* (必选）PFC（Priority-based Flow Control，基于优先级的流量控制）  
逐跳提供基于优先级的流量控制，能够实现在以太网链路上运行多种类型的流量而互不影响。
* (必选）ECN（Explicit Congestion Notification，显示拥塞通知）  
设备发生拥塞时，通过对报文IP头中ECN域的标识，由接收端向发送端发出降低发送速率的CNP（Congestion Notification Packet，拥塞通知报文），实现端到端的拥塞管理，减缓拥塞扩散恶化。
* (建议）DCBX（Data Center Bridging Exchange Protocol，数据中心桥能力交换协议）  
使用LLDP自动协商DCB能力参数，包括PFC和ETS等。一般用在接入交换机连接服务器的端口，与服务器网卡进行能力协商。
* (可选）ETS（Enhanced Transmission Selection，增强传输选择）  
将流量按服务类型分组，在提供不同流量的最小带宽保证的同时提高链路利用率，保证重要流量的带宽百分比。需要逐跳提供。


## PFC简介
PFC是构建无损以太网的必选手段之一，能够逐跳提供基于优先级的流量控制。设备在进行报文转发时，根据报文的优先级进入对应映射关系的队列中进行调度转发。当某一优先级报文发送速率超过接收速率，导致接收方可用数据缓冲空间不足时，设备通过PFC PAUSE帧反馈给上一跳设备，上一跳设备收到PAUSE帧报文后停止发送本优先级报文，直到再收到PFC XON帧或经过一定的老化时间后才能恢复流量发送。通过使用PFC功能，使得某种类型的流量拥塞不会影响其他类型流量的正常转发，从而达到同一链路上不同类型的报文互不影响。


## ECN简介
ECN是构建无损以太网的必选手段之一。ECN定义了一种基于IP层及传输层的流量控制及端到端拥塞通知机制。ECN功能利用IP报文头中的DS域来标记报文传输路径上的拥塞状态。支持该功能的终端设备可以通过报文内容判断出传输路径上发生了拥塞，从而调整报文的发送方式，避免拥塞加剧。

* ECN工作机制
ECN功能对IP报文头中DS域的最后两个比特位（称为ECN域）进行了如下定义：
比特位6用于标识发送端设备是否支持ECN功能，称为ECT位（ECN-Capable Transport）
比特位7用于标识报文在传输路径上是否经历过拥塞，称为CE位（Congestion Experienced）